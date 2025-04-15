#!/bin/bash

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color
BLUE='\033[0;34m'

# 打印带颜色的分隔线
print_separator() {
    echo -e "${BLUE}----------------------------------------${NC}"
}

# 打印测试用例标题
print_test_case() {
    echo -e "${GREEN}测试用例: $1${NC}"
}

# 测试用例1：基本 HEAD 请求 - 存在的文件
print_test_case "HEAD 请求 - 存在的文件"
print_separator
echo "发送: HEAD /index.html HTTP/1.1"
curl -I http://localhost:9999/index.html
print_separator

# 测试用例2：HEAD 请求 - 不存在的文件 (应返回404)
print_test_case "HEAD 请求 - 不存在的文件 (应返回404)"
print_separator
echo "发送: HEAD /nonexistent.html HTTP/1.1"
curl -I http://localhost:9999/nonexistent.html
print_separator

# 测试用例3：HEAD 请求 - 根目录 (应返回默认页面头部)
print_test_case "HEAD 请求 - 根目录 (应返回默认页面头部)"
print_separator
echo "发送: HEAD / HTTP/1.1"
curl -I http://localhost:9999/
print_separator

echo -e "${GREEN}HEAD 方法测试完成${NC}"
