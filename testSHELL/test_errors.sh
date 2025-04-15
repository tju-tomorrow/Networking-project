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

# 测试用例1：不支持的HTTP方法 (应返回501)
print_test_case "不支持的HTTP方法 (应返回501)"
print_separator
echo "发送: PUT /index.html HTTP/1.1"
curl -v -X PUT http://localhost:9999/index.html
print_separator

# 测试用例2：不支持的HTTP版本 (应返回505)
print_test_case "不支持的HTTP版本 (应返回505)"
print_separator
echo "发送: 不支持的HTTP版本"
printf "GET /index.html HTTP/2.0\r\nHost: localhost\r\n\r\n" | nc localhost 9999
print_separator

# 测试用例3：格式错误的请求 (应返回400)
print_test_case "格式错误的请求 (应返回400)"
print_separator
echo "发送: 格式错误的请求"
printf "INVALID REQUEST\r\n\r\n" | nc localhost 9999
print_separator

# 测试用例4：请求不存在的文件 (应返回404)
print_test_case "请求不存在的文件 (应返回404)"
print_separator
echo "发送: GET /nonexistent.html HTTP/1.1"
curl -v http://localhost:9999/nonexistent.html
print_separator

echo -e "${GREEN}错误处理测试完成${NC}"
