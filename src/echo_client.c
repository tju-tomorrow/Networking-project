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
    if (argc < 4)
    {
        fprintf(stderr, "用法: %s <服务器IP> <端口> <方法> [URI] [请求体]\n", argv[0]);
        fprintf(stderr, "方法可以是: GET, HEAD, POST, BAD (发送格式错误的请求), OTHER (发送未实现的方法)\n");
        fprintf(stderr, "示例:\n");
        fprintf(stderr, "  %s localhost 9999 GET /\n", argv[0]);
        fprintf(stderr, "  %s localhost 9999 POST / \"Hello World\"\n", argv[0]);
        fprintf(stderr, "  %s localhost 9999 BAD\n", argv[0]);
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

    // 创建套接字连接
    if((sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
        fprintf(stderr, "创建套接字失败\n");
        return EXIT_FAILURE;
    }
    
    if (connect(sock, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        fprintf(stderr, "连接失败\n");
        close(sock);
        return EXIT_FAILURE;
    }

    const char* method_str = argv[3];
    const char* uri = argc > 4 ? argv[4] : "/";
    const char* body = argc > 5 ? argv[5] : "";
    
    if (strcmp(method_str, "GET") == 0) {
        send_http_request(sock, GET, uri, "");
    } else if (strcmp(method_str, "HEAD") == 0) {
        send_http_request(sock, HEAD, uri, "");
    } else if (strcmp(method_str, "POST") == 0) {
        send_http_request(sock, POST, uri, body);
    } else if (strcmp(method_str, "BAD") == 0) {
        // 发送格式错误的请求
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
    } else if (strcmp(method_str, "OTHER") == 0) {
        send_http_request(sock, INVALID_METHOD, uri, "");
    } else {
        fprintf(stderr, "不支持的方法: %s\n", method_str);
        close(sock);
        return EXIT_FAILURE;
    }

    freeaddrinfo(servinfo);
    return EXIT_SUCCESS;
}
