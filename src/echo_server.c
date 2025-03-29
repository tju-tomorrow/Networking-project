#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include "parse.h"

#define ECHO_PORT 9999
#define BUF_SIZE 4096
#define HTTP_400 "HTTP/1.1 400 Bad request\r\n\r\n"
#define HTTP_501 "HTTP/1.1 501 Not Implemented\r\n\r\n"

int sock = -1, client_sock = -1;
char buf[BUF_SIZE];

// 构建 HTTP 响应函数 - 简单地回显请求内容
char* build_http_response(Request* request, char* original_request, int request_len, int* response_len) {
    if (request == NULL) {
        *response_len = strlen(HTTP_400);
        return strdup(HTTP_400);
    }
    
    // 检查 HTTP 方法
    if (strcmp(request->http_method, "GET") != 0 && 
        strcmp(request->http_method, "HEAD") != 0 && 
        strcmp(request->http_method, "POST") != 0) {
        *response_len = strlen(HTTP_501);
        return strdup(HTTP_501);
    }
    
    // 为 Echo 响应分配内存 - 需要足够的空间存放原始请求和响应头
    char* response = (char*)malloc(request_len + 256);
    if (response == NULL) {
        fprintf(stderr, "内存分配失败\n");
        *response_len = 0;
        return NULL;
    }
    
    // 构建响应头
    int offset = sprintf(response, "HTTP/1.1 200 OK\r\n\r\n");
    
    // 直接回显原始请求内容
    memcpy(response + offset, original_request, request_len);
    *response_len = offset + request_len;
    
    return response;
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
        
        while(1) {
            /* 接收客户端消息，解析 HTTP 请求并响应 */
            memset(buf, 0, BUF_SIZE);  // 清空缓冲区
            int readret = recv(client_sock, buf, BUF_SIZE, 0);  // 接收数据
            if (readret <= 0) break;   // 接收出错或连接关闭，跳出循环
            
            fprintf(stdout, "接收到数据（共 %d 字节）\n", readret);
            
            // 保存原始请求内容用于回显
            char original_request[BUF_SIZE];
            memcpy(original_request, buf, readret);
            
            // 解析 HTTP 请求
            Request* request = parse(buf, readret, client_sock);
            
            // 构建响应
            int response_len = 0;
            char* response = NULL;
            
            if (request == NULL) {
                // 解析失败，发送 400 Bad Request
                response = strdup(HTTP_400);
                response_len = strlen(response);
                fprintf(stdout, "请求格式错误，发送 400 Bad Request\n");
            } else {
                // 解析成功，根据请求方法构建响应
                response = build_http_response(request, original_request, readret, &response_len);
                
                if (response == NULL) {
                    fprintf(stderr, "构建响应失败\n");
                    break;
                }
                
                // 打印请求信息
                fprintf(stdout, "HTTP 方法: %s\n", request->http_method);
                fprintf(stdout, "HTTP URI: %s\n", request->http_uri);
                fprintf(stdout, "HTTP 版本: %s\n", request->http_version);
                
                // 释放请求资源
                free(request->headers);
                free(request);
            }
            
            // 发送响应
            if (send(client_sock, response, response_len, 0) < 0) {
                fprintf(stderr, "发送响应失败\n");
                free(response);
                break;
            }
            
            fprintf(stdout, "响应已发送到客户端\n");
            free(response);
            
            // HTTP 是无状态协议，每个请求处理完后关闭连接
            break;
        }
        
        /* 客户端关闭连接。服务器释放资源并再次监听 */
        if (close_socket(client_sock)) {
            close_socket(sock);
            fprintf(stderr, "关闭客户端套接字错误。\n");
            return EXIT_FAILURE;
        }
        
        fprintf(stdout, "已关闭来自 %s:%d 的连接\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    }
    
    close_socket(sock);
    return EXIT_SUCCESS;
}
