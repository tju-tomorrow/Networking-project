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

# 测试用例1：基本 POST 请求
print_test_case "POST 请求 - 简单数据"
print_separator
echo "发送: POST /echo HTTP/1.1 带简单数据"
curl -v -X POST -d "Hello, this is a test message" http://localhost:9999/echo
print_separator

# 测试用例2：POST 请求带表单数据
print_test_case "POST 请求 - 表单数据"
print_separator
echo "发送: POST /submit HTTP/1.1 带表单数据"
curl -v -X POST -d "username=test&password=123" http://localhost:9999/submit
print_separator

# 测试用例3：POST 请求带JSON数据
print_test_case "POST 请求 - JSON数据"
print_separator
echo "发送: POST /api HTTP/1.1 带JSON数据"
curl -v -X POST -H "Content-Type: application/json" -d '{"name":"test","value":123}' http://localhost:9999/api
print_separator

echo -e "${GREEN}POST 方法测试完成${NC}"
