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
    echo -e "${GREEN}测试用例 $1: $2${NC}"
}

# 等待服务器启动
sleep 1

# 测试用例1：基本 GET 请求
print_test_case "1.1" "基本 GET 请求"
print_separator
echo "发送: GET / HTTP/1.1"
./echo_client localhost 9999 GET /
print_separator

# 测试用例1.2：带路径的 GET 请求
print_test_case "1.2" "带路径的 GET 请求"
print_separator
echo "发送: GET /test/path HTTP/1.1"
./echo_client localhost 9999 GET /test/path
print_separator

# 测试用例2：HEAD 请求
print_test_case "2" "HEAD 请求"
print_separator
echo "发送: HEAD / HTTP/1.1"
./echo_client localhost 9999 HEAD /
print_separator

# 测试用例3.1：基本 POST 请求
print_test_case "3.1" "基本 POST 请求"
print_separator
echo "发送: POST / 带简单消息"
./echo_client localhost 9999 POST / "Hello, World!"
print_separator

# 测试用例3.2：POST 请求带表单数据
print_test_case "3.2" "POST 请求带表单数据"
print_separator
echo "发送: POST /submit 带表单数据"
./echo_client localhost 9999 POST /submit "username=test&password=123"
print_separator

# 测试用例4.1：未实现的方法（返回501）
print_test_case "4.1" "未实现的方法 (应返回501)"
print_separator
echo "发送: 未实现的HTTP方法"
./echo_client localhost 9999 OTHER /
print_separator

# 测试用例4.2：另一个未实现的方法
print_test_case "4.2" "另一个未实现的方法 (应返回501)"
print_separator
echo "发送: 另一个未实现的方法"
./echo_client localhost 9999 OTHER /test
print_separator

# 测试用例5.1：格式错误的请求（返回400）
print_test_case "5.1" "格式错误的请求 (应返回400)"
print_separator
echo "发送: 格式错误的请求"
./echo_client localhost 9999 BAD
print_separator

# 测试用例5.2：另一个格式错误的请求
print_test_case "5.2" "另一个格式错误的请求 (应返回400)"
print_separator
echo "发送: 另一个格式错误的请求"
./echo_client localhost 9999 BAD
print_separator

echo -e "${GREEN}所有测试用例执行完成${NC}"
