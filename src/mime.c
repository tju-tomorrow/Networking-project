#include "../include/mime.h"
#include <string.h>
#include <ctype.h>

// 获取文件扩展名（转换为小写）
static void get_file_extension(const char* file_path, char* ext, size_t ext_size) {
    const char* dot = strrchr(file_path, '.');
    if (!dot || dot == file_path) {
        ext[0] = '\0';
    } else {
        size_t i;
        for (i = 0; i < ext_size - 1 && dot[i+1]; i++) {
            ext[i] = tolower(dot[i+1]);
        }
        ext[i] = '\0';
    }
}

// 根据文件扩展名获取MIME类型
const char* get_mime_type(const char* file_path) {
    char ext[32] = {0};
    get_file_extension(file_path, ext, sizeof(ext));
    
    // 常见MIME类型映射
    if (strcmp(ext, "html") == 0 || strcmp(ext, "htm") == 0) {
        return "text/html";
    } else if (strcmp(ext, "css") == 0) {
        return "text/css";
    } else if (strcmp(ext, "js") == 0) {
        return "application/javascript";
    } else if (strcmp(ext, "png") == 0) {
        return "image/png";
    } else if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0) {
        return "image/jpeg";
    } else if (strcmp(ext, "gif") == 0) {
        return "image/gif";
    } else if (strcmp(ext, "ico") == 0) {
        return "image/x-icon";
    } else if (strcmp(ext, "svg") == 0) {
        return "image/svg+xml";
    } else if (strcmp(ext, "txt") == 0) {
        return "text/plain";
    } else if (strcmp(ext, "pdf") == 0) {
        return "application/pdf";
    } else if (strcmp(ext, "xml") == 0) {
        return "application/xml";
    } else if (strcmp(ext, "json") == 0) {
        return "application/json";
    }
    
    // 默认MIME类型
    return "application/octet-stream";
}
