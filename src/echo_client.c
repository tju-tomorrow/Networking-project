/******************************************************************************
* echo_client.c                                                               *
*                                                                             *
* Description: This file contains the C source code for an HTTP client.  The   *
*              client connects to an arbitrary <host,port> and sends HTTP      *
*              requests to test the Echo Web Server.                          *
*                                                                             *
* Authors: Athula Balachandran <abalacha@cs.cmu.edu>,                         *
*          Wolf Richter <wolf@cs.cmu.edu>                                     *
*          Modified for HTTP requests                                         *
*                                                                             *
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define ECHO_PORT 9999
#define BUF_SIZE 4096

// HTTP请求类型
typedef enum {
    GET,
    HEAD,
    POST,
    INVALID_METHOD
} HttpMethod;

// 构建HTTP请求
char* build_http_request(HttpMethod method, const char* uri, const char* body) {
    char* request = (char*)malloc(BUF_SIZE);
    if (request == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return NULL;
    }
    
    // 根据方法构建请求头
    switch (method) {
        case GET:
            sprintf(request, "GET %s HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n", uri);
            break;
        case HEAD:
            sprintf(request, "HEAD %s HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n", uri);
            break;
        case POST:
            sprintf(request, "POST %s HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", 
                   uri, strlen(body), body);
            break;
        case INVALID_METHOD:
            sprintf(request, "INVALID %s HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n", uri);
            break;
        default:
            free(request);
            return NULL;
    }
    
    return request;
}

// 发送HTTP请求并接收响应
void send_http_request(int sock, HttpMethod method, const char* uri, const char* body) {
    char* request = build_http_request(method, uri, body);
    if (request == NULL) {
        fprintf(stderr, "构建HTTP请求失败\n");
        return;
    }
    
    // 打印请求信息
    fprintf(stdout, "\n发送HTTP请求:\n%s\n", request);
    
    // 发送请求
    send(sock, request, strlen(request), 0);
    
    // 接收响应
    char response[BUF_SIZE];
    int bytes_received = recv(sock, response, BUF_SIZE - 1, 0);
    
    if (bytes_received > 0) {
        response[bytes_received] = '\0';
        fprintf(stdout, "\n接收到响应 (%d 字节):\n%s\n", bytes_received, response);
    } else {
        fprintf(stderr, "接收响应失败\n");
    }
    
    free(request);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "用法: %s <服务器IP> <端口>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int status, sock;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    struct addrinfo *servinfo; // 将指向结果
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP流套接字
    hints.ai_flags = AI_PASSIVE; // 自动填充我的IP

    if ((status = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) 
    {
        fprintf(stderr, "getaddrinfo错误: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }

    printf("=== HTTP Echo Client 测试程序 ===\n");
    printf("1. 测试 GET 请求\n");
    printf("2. 测试 HEAD 请求\n");
    printf("3. 测试 POST 请求\n");
    printf("4. 测试未实现的方法 (501 Not Implemented)\n");
    printf("5. 测试错误的请求格式 (400 Bad Request)\n");
    printf("0. 退出\n");
    
    int choice;
    do {
        printf("\n请选择测试类型 (0-5): ");
        scanf("%d", &choice);
        getchar(); // 消耗换行符
        
        if (choice == 0) break;
        
        // 创建新的套接字连接
        if((sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
        {
            fprintf(stderr, "创建套接字失败\n");
            continue;
        }
        
        if (connect(sock, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
        {
            fprintf(stderr, "连接失败\n");
            close(sock);
            continue;
        }
        
        switch (choice) {
            case 1: // GET 请求
                send_http_request(sock, GET, "/", "");
                break;
                
            case 2: // HEAD 请求
                send_http_request(sock, HEAD, "/", "");
                break;
                
            case 3: { // POST 请求
                char body[1024];
                printf("请输入POST请求体: ");
                fgets(body, sizeof(body), stdin);
                body[strcspn(body, "\n")] = 0; // 移除换行符
                send_http_request(sock, POST, "/", body);
                break;
            }
                
            case 4: // 未实现的方法
                send_http_request(sock, INVALID_METHOD, "/", "");
                break;
                
            case 5: { // 错误的请求格式
                const char* bad_request = "这不是一个有效的HTTP请求\r\n";
                fprintf(stdout, "\n发送无效请求:\n%s\n", bad_request);
                send(sock, bad_request, strlen(bad_request), 0);
                
                // 接收响应
                char response[BUF_SIZE];
                int bytes_received = recv(sock, response, BUF_SIZE - 1, 0);
                
                if (bytes_received > 0) {
                    response[bytes_received] = '\0';
                    fprintf(stdout, "\n接收到响应 (%d 字节):\n%s\n", bytes_received, response);
                } else {
                    fprintf(stderr, "接收响应失败\n");
                }
                break;
            }
                
            default:
                printf("无效的选择\n");
        }
        
        close(sock);
        
    } while (choice != 0);

    freeaddrinfo(servinfo);
    return EXIT_SUCCESS;
}
