#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

// 日志级别
typedef enum {
    LOG_ERROR,  // 错误日志
    LOG_ACCESS  // 访问日志
} LogLevel;

// 初始化日志系统
int log_init(const char* error_log_path, const char* access_log_path);

// 写入错误日志 (Apache Error Log 格式)
void log_error(const char* format, ...);

// 写入访问日志 (Apache Common Log Format)
void log_access(const char* client_ip, int client_port, const char* method, 
               const char* uri, const char* version, int status_code, int content_length);

// 关闭日志系统
void log_close();

#endif /* LOGGER_H */
