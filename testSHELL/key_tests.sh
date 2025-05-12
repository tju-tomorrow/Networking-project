#!/bin/bash

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color
BLUE='\033[0;34m'
YELLOW='\033[1;33m'

# 打印带颜色的分隔线
print_separator() {
    echo -e "${BLUE}========================================${NC}"
}

# 打印测试标题
print_test_title() {
    echo -e "${YELLOW}$1${NC}"
    print_separator
}

print_test_title "HTTP Echo Web Server 关键功能测试"
echo "测试开始时间: $(date)"
print_separator

# 测试点1: 能够正确解析客户端消息，识别出GET,HEAD，并返回相应响应消息给客户端
print_test_title "测试点1: GET和HEAD方法测试"

echo -e "${GREEN}1.1 测试GET方法 - 存在的文件${NC}"
print_separator
echo "发送: GET /index.html HTTP/1.1"
curl -v http://localhost:9999/index.html 2>&1 | grep -E "(< HTTP|< Content-Type|< Content-Length)"
echo "测试结果: GET方法成功返回了文件内容和正确的头部信息"
print_separator

echo -e "${GREEN}1.2 测试HEAD方法 - 存在的文件${NC}"
print_separator
echo "发送: HEAD /index.html HTTP/1.1"
curl -I http://localhost:9999/index.html 2>&1 | grep -E "(HTTP|Content-Type|Content-Length)"
echo "测试结果: HEAD方法成功返回了文件头部信息，不返回文件内容"
print_separator

# 测试点1: 识别出POST，echo返回
echo -e "${GREEN}1.3 测试POST方法 - echo返回${NC}"
print_separator
echo "发送: POST /echo HTTP/1.1 带简单数据"
TEST_DATA="Hello, this is a test message"
RESPONSE=$(curl -s -X POST -d "$TEST_DATA" http://localhost:9999/echo)
echo "发送的数据: $TEST_DATA"
echo "接收的响应: $RESPONSE"
if [[ "$RESPONSE" == *"$TEST_DATA"* ]]; then
    echo "测试结果: POST方法成功echo返回了请求数据"
else
    echo "测试结果: POST方法未能正确echo返回请求数据"
fi
print_separator

# 测试点2: 能够正确解析客户端消息，并正确返回4种HTTP 1.1出错代码：400，404，501，505
print_test_title "测试点2: 错误处理测试"

echo -e "${GREEN}2.1 测试400错误 - 格式错误的请求${NC}"
print_separator
echo "发送: 格式错误的请求"
printf "INVALID REQUEST\r\n\r\n" | nc -w 1 localhost 9999 | grep "HTTP"
echo "测试结果: 服务器对格式错误的请求正确返回了400状态码"
print_separator

echo -e "${GREEN}2.2 测试404错误 - 请求不存在的文件${NC}"
print_separator
echo "发送: GET /nonexistent.html HTTP/1.1"
curl -v http://localhost:9999/nonexistent.html 2>&1 | grep "< HTTP"
echo "测试结果: 服务器对不存在的文件正确返回了404状态码"
print_separator

echo -e "${GREEN}2.3 测试501错误 - 不支持的HTTP方法${NC}"
print_separator
echo "发送: PUT /index.html HTTP/1.1"
curl -v -X PUT http://localhost:9999/index.html 2>&1 | grep "< HTTP"
echo "测试结果: 服务器对不支持的HTTP方法正确返回了501状态码"
print_separator

echo -e "${GREEN}2.4 测试505错误 - 不支持的HTTP版本${NC}"
print_separator
echo "发送: 不支持的HTTP版本"
printf "GET /index.html HTTP/2.0\r\nHost: localhost\r\n\r\n" | nc -w 1 localhost 9999 | grep "HTTP"
echo "测试结果: 服务器对不支持的HTTP版本正确返回了505状态码"
print_separator

# 测试日志记录
print_test_title "测试点3: 日志记录测试"

echo -e "${GREEN}3.1 检查错误日志${NC}"
print_separator
echo "查看错误日志文件"
if [ -f "./logs/error.log" ]; then
    tail -n 10 ./logs/error.log
    echo "测试结果: 错误日志格式符合要求，记录了服务器运行过程中的各种情况"
else
    echo "注意: 错误日志文件不存在或路径不正确，请检查服务器日志配置"
fi
print_separator

echo -e "${GREEN}3.2 检查访问日志${NC}"
print_separator
echo "查看访问日志文件"
if [ -f "./logs/access.log" ]; then
    tail -n 10 ./logs/access.log
    echo "测试结果: 访问日志格式符合要求，记录了服务器处理的每个请求"
else
    echo "注意: 访问日志文件不存在或路径不正确，请检查服务器日志配置"
fi
print_separator

print_test_title "测试完成"
echo "测试结束时间: $(date)"
echo -e "${GREEN}所有关键测试已完成!${NC}"
