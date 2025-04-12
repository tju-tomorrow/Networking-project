#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "parse.h"
#include "../include/logger.h"
#include "../include/mime.h"

#define ECHO_PORT 9999
#define BUF_SIZE 4096
#define MAX_HEADER_SIZE 8192

// 响应状态码定义 - 使用完全符合测试平台要求的格式
#define RESPONSE_400 "HTTP/1.1 400 Bad request\r\n\r\n"
#define RESPONSE_404 "HTTP/1.1 404 Not Found\r\n\r\n"
#define RESPONSE_501 "HTTP/1.1 501 Not Implemented\r\n\r\n"
#define RESPONSE_505 "HTTP/1.1 505 HTTP Version not supported\r\n\r\n"

// 静态文件根目录
#define STATIC_ROOT "./static_site"

int sock = -1, client_sock = -1;
char buf[BUF_SIZE];

// 检查是否为持久连接
int is_persistent_connection(Request* request) {
    if (request == NULL) return 0;
    
    // 检查 Connection 头部
    for (int i = 0; i < request->header_count; i++) {
        if (strcasecmp(request->headers[i].header_name, "Connection") == 0) {
            if (strcasecmp(request->headers[i].header_value, "close") == 0) {
                return 0; // 显式指定关闭连接
            }
            if (strcasecmp(request->headers[i].header_value, "keep-alive") == 0) {
                return 1; // 显式指定保持连接
            }
        }
    }
    
    // HTTP/1.1 默认为持久连接，HTTP/1.0 默认为非持久连接
    if (strcasecmp(request->http_version, "HTTP/1.1") == 0) {
        return 1;
    }
    
    return 0;
}

// 检查 HTTP 版本是否支持
int is_supported_http_version(const char* version) {
    return (strcasecmp(version, "HTTP/1.0") == 0 || 
            strcasecmp(version, "HTTP/1.1") == 0);
}

// 读取文件内容
char* read_file(const char* file_path, int* file_size) {
    struct stat st;
    if (stat(file_path, &st) == -1) {
        log_error("无法获取文件状态: %s (errno: %d)", file_path, errno);
        *file_size = 0;
        return NULL;
    }
    
    *file_size = st.st_size;
    
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        log_error("无法打开文件: %s (errno: %d)", file_path, errno);
        *file_size = 0;
        return NULL;
    }
    
    char* content = (char*)malloc(*file_size);
    if (content == NULL) {
        log_error("内存分配失败，文件大小: %d", *file_size);
        close(fd);
        *file_size = 0;
        return NULL;
    }
    
    ssize_t bytes_read = read(fd, content, *file_size);
    if (bytes_read != *file_size) {
        log_error("读取文件失败: %s (读取 %ld 字节，预期 %d 字节)", 
                file_path, bytes_read, *file_size);
        free(content);
        close(fd);
        *file_size = 0;
        return NULL;
    }
    
    close(fd);
    return content;
}

// 构建 HTTP 响应函数 - 处理 GET、HEAD 和 POST 请求
char* build_http_response(Request* request, char* original_request, int request_len, int* response_len, int* status_code) {
    if (request == NULL) {
        *status_code = 400; // Bad Request
        *response_len = strlen(RESPONSE_400);
        return strdup(RESPONSE_400);
    }
    
    // 检查 HTTP 版本
    if (!is_supported_http_version(request->http_version)) {
        *status_code = 505; // HTTP Version Not Supported
        *response_len = strlen(RESPONSE_505);
        return strdup(RESPONSE_505);
    }
    
    // 检查 HTTP 方法
    if (strcmp(request->http_method, "GET") != 0 && 
        strcmp(request->http_method, "HEAD") != 0 && 
        strcmp(request->http_method, "POST") != 0) {
        *status_code = 501; // Not Implemented
        *response_len = strlen(RESPONSE_501);
        return strdup(RESPONSE_501);
    }
    
    // 处理 POST 请求 - 简单回显
    if (strcmp(request->http_method, "POST") == 0) {
        *status_code = 200; // OK
        
        // 检查是否为持久连接
        int persistent = is_persistent_connection(request);
        
        // 为响应分配内存
        char* response = (char*)malloc(request_len + 256);
        if (response == NULL) {
            log_error("内存分配失败");
            *response_len = 0;
            return NULL;
        }
        
        // 构建响应头
        int offset = sprintf(response, "HTTP/1.1 200 OK\r\n");
        if (persistent) {
            offset += sprintf(response + offset, "Connection: keep-alive\r\n");
        } else {
            offset += sprintf(response + offset, "Connection: close\r\n");
        }
        offset += sprintf(response + offset, "Content-Type: text/plain\r\n");
        offset += sprintf(response + offset, "Content-Length: %d\r\n\r\n", request_len);
        
        // 添加响应体 (原始请求内容)
        memcpy(response + offset, original_request, request_len);
        *response_len = offset + request_len;
        
        return response;
    }
    
    // 处理 GET 和 HEAD 请求
    if (strcmp(request->http_method, "GET") == 0 || 
        strcmp(request->http_method, "HEAD") == 0) {
        // 构建文件路径
        char file_path[1024];
        
        // 默认请求 index.html
        if (strcmp(request->http_uri, "/") == 0) {
            snprintf(file_path, sizeof(file_path), "%s/index.html", STATIC_ROOT);
        } else {
            // 移除URI中的查询参数
            char uri_copy[1024];
            strncpy(uri_copy, request->http_uri, sizeof(uri_copy));
            uri_copy[sizeof(uri_copy) - 1] = '\0';
            
            char* query = strchr(uri_copy, '?');
            if (query) {
                *query = '\0';
            }
            
            snprintf(file_path, sizeof(file_path), "%s%s", STATIC_ROOT, uri_copy);
        }
        
        // 读取文件内容
        int file_size = 0;
        char* file_content = read_file(file_path, &file_size);
        
        if (file_content == NULL) {
            log_error("文件不存在: %s", file_path);
            *status_code = 404; // Not Found
            // 直接返回静态字符串
            *response_len = 28; // "HTTP/1.1 404 Not Found\r\n\r\n" 的长度
            return strdup("HTTP/1.1 404 Not Found\r\n\r\n");
        }
        
        // 获取文件的MIME类型
        const char* mime_type = get_mime_type(file_path);
        
        // 检查是否为持久连接
        int persistent = is_persistent_connection(request);
        
        // 构建响应头
        char* response_headers = (char*)malloc(1024);
        if (response_headers == NULL) {
            log_error("内存分配失败");
            free(file_content);
            *response_len = 0;
            return NULL;
        }
        
        int headers_len = 0;
        headers_len += sprintf(response_headers + headers_len, "HTTP/1.1 200 OK\r\n");
        if (persistent) {
            headers_len += sprintf(response_headers + headers_len, "Connection: keep-alive\r\n");
        } else {
            headers_len += sprintf(response_headers + headers_len, "Connection: close\r\n");
        }
        headers_len += sprintf(response_headers + headers_len, "Content-Type: %s\r\n", mime_type);
        headers_len += sprintf(response_headers + headers_len, "Content-Length: %d\r\n\r\n", file_size);
        
        *status_code = 200; // OK
        
        // 如果是 HEAD 请求，只返回头部
        if (strcmp(request->http_method, "HEAD") == 0) {
            *response_len = headers_len;
            free(file_content);
            return response_headers;
        }
        
        // 如果是 GET 请求，返回头部和文件内容
        char* response = (char*)malloc(headers_len + file_size);
        if (response == NULL) {
            log_error("内存分配失败");
            free(file_content);
            free(response_headers);
            *response_len = 0;
            return NULL;
        }
        
        // 复制头部
        memcpy(response, response_headers, headers_len);
        free(response_headers);
        
        // 复制文件内容
        memcpy(response + headers_len, file_content, file_size);
        free(file_content);
        
        *response_len = headers_len + file_size;
        return response;
    }
    
    // 不应该到达这里
    *status_code = 501; // Not Implemented
    *response_len = strlen(RESPONSE_501);
    return strdup(RESPONSE_501);
}

// 关闭套接字并处理错误的函数
int close_socket(int sock) {
    if (close(sock)) {
        fprintf(stderr, "关闭套接字失败。\n");
        return 1; // 如果关闭失败，返回错误代码
    }
    return 0; // 如果关闭成功，返回成功代码
}

// 处理终止信号的信号处理函数
void handle_signal(const int sig) {
    if (sock != -1) { // 检查套接字是否已打开
        fprintf(stderr, "\n收到信号 %d。正在关闭套接字。\n", sig);
        close_socket(sock); // 关闭套接字
    }
    exit(0); // 退出程序
}

// 处理 SIGPIPE（管道破裂）的信号处理函数
void handle_sigpipe(const int sig) {
    if (sock != -1) { // 检查套接字是否已打开
        return; // 如果套接字已打开，忽略信号
    }
    exit(0); // 如果套接字未打开，退出程序
}
int main(int argc, char *argv[]) {
    /* 初始化日志系统 */
    if (log_init("logs/error.log", "logs/access.log") != 0) {
        fprintf(stderr, "初始化日志系统失败\n");
        return EXIT_FAILURE;
    }
    
    log_error("服务器启动");
    
    /* 注册信号处理函数 */
    /* 处理程序终止信号 */
    signal(SIGTERM, handle_signal); // 终止信号
    signal(SIGINT, handle_signal);  // 中断信号（通常由Ctrl+C触发）
    signal(SIGSEGV, handle_signal); // 段错误信号
    signal(SIGABRT, handle_signal); // 异常终止信号
    signal(SIGQUIT, handle_signal); // 退出信号
    signal(SIGTSTP, handle_signal); // 终端停止信号（通常由Ctrl+Z触发）
    signal(SIGFPE, handle_signal);  // 浮点异常信号
    signal(SIGHUP, handle_signal);  // 挂起信号
    /* 正常I/O事件 */
    signal(SIGPIPE, handle_sigpipe); // 管道破裂信号（当写入到已关闭的管道或套接字时触发）
    
    socklen_t cli_size;             // 客户端地址结构大小
    struct sockaddr_in addr, cli_addr; // 服务器和客户端地址结构
    fprintf(stdout, "----- Echo Server -----\n");

    /* 所有网络程序必须创建套接字 */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "创建套接字失败。\n");
        return EXIT_FAILURE;
    }
    
    /* 设置套接字选项 SO_REUSEADDR 和 SO_REUSEPORT */
    /* 允许地址和端口重用，避免服务器重启时出现"地址已在使用"的错误 */
    int opt = 1;
    /* 先设置 SO_REUSEADDR */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        fprintf(stderr, "设置套接字选项 SO_REUSEADDR 失败。\n");
        return EXIT_FAILURE;
    }
    /* 再设置 SO_REUSEPORT */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        fprintf(stderr, "设置套接字选项 SO_REUSEPORT 失败。\n");
        /* 即使 SO_REUSEPORT 设置失败，我们也继续执行，因为 SO_REUSEADDR 已经设置成功 */
        fprintf(stderr, "继续执行，但端口可能无法被多个进程共享。\n");
    }

    addr.sin_family = AF_INET;         // 使用IPv4协议族
    addr.sin_port = htons(ECHO_PORT);  // 设置端口号（主机字节序转网络字节序）
    addr.sin_addr.s_addr = INADDR_ANY; // 接受任何网络接口的连接

    /* 服务器将套接字绑定到端口——通知操作系统它接受连接 */
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr))) {
        close_socket(sock);
        fprintf(stderr, "绑定套接字失败。\n");
        return EXIT_FAILURE;
    }

    /* 开始监听连接请求，队列长度为5 */
    if (listen(sock, 5)) {
        close_socket(sock);
        fprintf(stderr, "套接字监听错误。\n");
        return EXIT_FAILURE;
    }

    /* 最后，循环等待输入然后回写数据 */
    while (1) {
        /* 监听新连接 */
        cli_size = sizeof(cli_addr);
        fprintf(stdout, "等待连接...\n");
        client_sock = accept(sock, (struct sockaddr *) &cli_addr, &cli_size);
        if (client_sock == -1) {
            fprintf(stderr, "接受连接错误。\n");
            close_socket(sock);
            return EXIT_FAILURE;
        }
        
        fprintf(stdout, "来自 %s:%d 的新连接\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        
        int persistent_connection = 0;
        
        do {
            /* 接收客户端消息，解析 HTTP 请求并响应 */
            memset(buf, 0, BUF_SIZE);  // 清空缓冲区
            
            // 设置接收超时（10秒）
            struct timeval tv;
            tv.tv_sec = 10;
            tv.tv_usec = 0;
            setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
            
            int readret = recv(client_sock, buf, BUF_SIZE - 1, 0);  // 接收数据
            if (readret <= 0) {
                if (readret == 0) {
                    log_error("客户端关闭连接");
                } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    log_error("接收超时");
                } else {
                    log_error("接收错误: %d", errno);
                }
                break;   // 接收出错或连接关闭，跳出循环
            }
            
            // 检查请求头部大小
            if (readret > MAX_HEADER_SIZE) {
                log_error("请求头部过大: %d 字节", readret);
                send(client_sock, RESPONSE_400, strlen(RESPONSE_400), 0);
                log_access(inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), 
                          "UNKNOWN", "UNKNOWN", "UNKNOWN", 400, strlen(RESPONSE_400));
                break;
            }
            
            log_error("接收到数据（共 %d 字节）", readret);
            
            // 保存原始请求内容用于回显
            char original_request[BUF_SIZE];
            memcpy(original_request, buf, readret);
            buf[readret] = '\0'; // 确保字符串以 NULL 结尾
            
            // 解析 HTTP 请求
            Request* request = parse(buf, readret, client_sock);
            
            // 构建响应
            int response_len = 0;
            char* response = NULL;
            int status_code = 200;
            
            if (request == NULL) {
                // 解析失败，发送 400 Bad Request
                response = strdup(RESPONSE_400);
                response_len = strlen(response);
                status_code = 400;
                log_error("请求格式错误，发送 400 Bad Request");
                log_access(inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), 
                          "UNKNOWN", "UNKNOWN", "UNKNOWN", status_code, response_len);
            } else {
                // 检查是否为持久连接
                persistent_connection = is_persistent_connection(request);
                
                // 解析成功，根据请求方法构建响应
                response = build_http_response(request, original_request, readret, &response_len, &status_code);
                
                if (response == NULL) {
                    log_error("构建响应失败");
                    break;
                }
                
                // 记录访问日志
                log_access(inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), 
                          request->http_method, request->http_uri, request->http_version, 
                          status_code, response_len);
                
                // 打印请求信息
                log_error("HTTP 方法: %s, URI: %s, 版本: %s, 状态码: %d", 
                         request->http_method, request->http_uri, request->http_version, status_code);
                
                // 释放请求资源
                free(request->headers);
                free(request);
            }
            
            // 发送响应
            if (send(client_sock, response, response_len, 0) < 0) {
                log_error("发送响应失败: %d", errno);
                free(response);
                break;
            }
            
            log_error("响应已发送到客户端，状态码: %d, 大小: %d 字节", status_code, response_len);
            free(response);
            
        } while (persistent_connection);
        
        /* 客户端关闭连接。服务器释放资源并再次监听 */
        if (close_socket(client_sock)) {
            close_socket(sock);
            log_error("关闭客户端套接字错误");
            log_close();
            return EXIT_FAILURE;
        }
        
        log_error("已关闭来自 %s:%d 的连接", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    }
    
    close_socket(sock);
    log_error("服务器关闭");
    log_close();
    return EXIT_SUCCESS;
}
