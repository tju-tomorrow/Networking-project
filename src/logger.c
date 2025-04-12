#include "../include/logger.h"

static FILE* error_log = NULL;
static FILE* access_log = NULL;

int log_init(const char* error_log_path, const char* access_log_path) {
    // 创建日志目录
    mkdir("logs", 0755);
    
    // 打开错误日志文件
    error_log = fopen(error_log_path, "a");
    if (error_log == NULL) {
        fprintf(stderr, "无法打开错误日志文件: %s\n", error_log_path);
        return -1;
    }
    
    // 打开访问日志文件
    access_log = fopen(access_log_path, "a");
    if (access_log == NULL) {
        fprintf(stderr, "无法打开访问日志文件: %s\n", access_log_path);
        fclose(error_log);
        error_log = NULL;
        return -1;
    }
    
    return 0;
}

void log_error(const char* format, ...) {
    if (error_log == NULL) return;
    
    // 获取当前时间
    time_t now;
    struct tm* timeinfo;
    char timestamp[64];
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "[%a %b %d %H:%M:%S %Y]", timeinfo);
    
    // 写入时间戳
    fprintf(error_log, "%s ", timestamp);
    
    // 写入错误信息
    va_list args;
    va_start(args, format);
    vfprintf(error_log, format, args);
    va_end(args);
    
    // 添加换行符并刷新缓冲区
    fprintf(error_log, "\n");
    fflush(error_log);
}

void log_access(const char* client_ip, int client_port, const char* method, 
               const char* uri, const char* version, int status_code, int content_length) {
    if (access_log == NULL) return;
    
    // 获取当前时间
    time_t now;
    struct tm* timeinfo;
    char timestamp[64];
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "[%d/%b/%Y:%H:%M:%S %z]", timeinfo);
    
    // 写入访问日志 (Apache Common Log Format)
    // 格式: client_ip - - [timestamp] "method uri version" status_code content_length
    fprintf(access_log, "%s - - %s \"%s %s %s\" %d %d\n",
            client_ip, timestamp, method, uri, version, status_code, content_length);
    fflush(access_log);
}

void log_close() {
    if (error_log != NULL) {
        fclose(error_log);
        error_log = NULL;
    }
    
    if (access_log != NULL) {
        fclose(access_log);
        access_log = NULL;
    }
}
