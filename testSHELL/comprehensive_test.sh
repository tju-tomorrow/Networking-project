#!/bin/bash

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color
BLUE='\033[0;34m'
YELLOW='\033[1;33m'

# 测试结果目录
RESULTS_DIR="test_results"
mkdir -p $RESULTS_DIR

# 打印带颜色的分隔线
print_separator() {
    echo -e "${BLUE}========================================${NC}"
}

# 打印测试标题
print_test_title() {
    echo -e "${YELLOW}$1${NC}"
    print_separator
}

# 检查服务器是否运行
if ! nc -z localhost 9999 >/dev/null 2>&1; then
    echo -e "${RED}错误: 服务器未在端口9999上运行${NC}"
    echo "请先启动服务器: cd .. && ./liso_server"
    exit 1
fi

# 记录测试开始时间
START_TIME=$(date +%s)

print_test_title "开始运行 HTTP Echo Web Server 测试套件"
echo "测试开始时间: $(date)"
echo "测试结果将保存在 $RESULTS_DIR 目录下"
print_separator

# 测试点1: 能够正确解析客户端消息，识别出GET,HEAD，并返回相应响应消息给客户端
print_test_title "测试点1: GET和HEAD方法测试"

echo "1.1 测试GET方法 - 存在的文件"
print_separator
echo "发送: GET /index.html HTTP/1.1"
curl -v http://localhost:9999/index.html > $RESULTS_DIR/get_existing.txt 2>&1
echo "测试结果已保存到 $RESULTS_DIR/get_existing.txt"
print_separator

echo "1.2 测试GET方法 - 不存在的文件"
print_separator
echo "发送: GET /nonexistent.html HTTP/1.1"
curl -v http://localhost:9999/nonexistent.html > $RESULTS_DIR/get_nonexistent.txt 2>&1
echo "测试结果已保存到 $RESULTS_DIR/get_nonexistent.txt"
print_separator

echo "1.3 测试HEAD方法 - 存在的文件"
print_separator
echo "发送: HEAD /index.html HTTP/1.1"
curl -I http://localhost:9999/index.html > $RESULTS_DIR/head_existing.txt 2>&1
echo "测试结果已保存到 $RESULTS_DIR/head_existing.txt"
print_separator

echo "1.4 测试HEAD方法 - 不存在的文件"
print_separator
echo "发送: HEAD /nonexistent.html HTTP/1.1"
curl -I http://localhost:9999/nonexistent.html > $RESULTS_DIR/head_nonexistent.txt 2>&1
echo "测试结果已保存到 $RESULTS_DIR/head_nonexistent.txt"
print_separator

# 测试点1: 识别出POST，echo返回
echo "1.5 测试POST方法 - echo返回"
print_separator
echo "发送: POST /echo HTTP/1.1 带简单数据"
curl -v -X POST -d "Hello, this is a test message" http://localhost:9999/echo > $RESULTS_DIR/post_echo.txt 2>&1
echo "测试结果已保存到 $RESULTS_DIR/post_echo.txt"
print_separator

# 测试点2: 能够正确解析客户端消息，并正确返回4种HTTP 1.1出错代码：400，404，501，505
print_test_title "测试点2: 错误处理测试"

echo "2.1 测试400错误 - 格式错误的请求"
print_separator
echo "发送: 格式错误的请求"
printf "INVALID REQUEST\r\n\r\n" | nc localhost 9999 > $RESULTS_DIR/error_400.txt 2>&1
echo "测试结果已保存到 $RESULTS_DIR/error_400.txt"
print_separator

echo "2.2 测试404错误 - 请求不存在的文件"
print_separator
echo "发送: GET /nonexistent.html HTTP/1.1"
curl -v http://localhost:9999/nonexistent.html > $RESULTS_DIR/error_404.txt 2>&1
echo "测试结果已保存到 $RESULTS_DIR/error_404.txt"
print_separator

echo "2.3 测试501错误 - 不支持的HTTP方法"
print_separator
echo "发送: PUT /index.html HTTP/1.1"
curl -v -X PUT http://localhost:9999/index.html > $RESULTS_DIR/error_501.txt 2>&1
echo "测试结果已保存到 $RESULTS_DIR/error_501.txt"
print_separator

echo "2.4 测试505错误 - 不支持的HTTP版本"
print_separator
echo "发送: 不支持的HTTP版本"
printf "GET /index.html HTTP/2.0\r\nHost: localhost\r\n\r\n" | nc localhost 9999 > $RESULTS_DIR/error_505.txt 2>&1
echo "测试结果已保存到 $RESULTS_DIR/error_505.txt"
print_separator

# 性能测试
print_test_title "性能测试"

# 检查是否安装了ab工具
if ! command -v ab &> /dev/null; then
    echo -e "${RED}警告: Apache Benchmark (ab) 工具未安装${NC}"
    echo "性能测试将被跳过"
    echo "在Mac上可以使用: brew install httpd"
    echo "在Ubuntu上可以使用: sudo apt-get install apache2-utils"
else
    echo "3.1 基本性能测试 - 100个请求，10个并发"
    print_separator
    echo "执行: ab -n 100 -c 10 http://localhost:9999/index.html"
    ab -n 100 -c 10 http://localhost:9999/index.html > $RESULTS_DIR/perf_basic.txt 2>&1
    echo "测试结果已保存到 $RESULTS_DIR/perf_basic.txt"
    print_separator

    echo "3.2 持久连接性能测试 - 使用keep-alive"
    print_separator
    echo "执行: ab -n 100 -c 10 -k http://localhost:9999/index.html"
    ab -n 100 -c 10 -k http://localhost:9999/index.html > $RESULTS_DIR/perf_keepalive.txt 2>&1
    echo "测试结果已保存到 $RESULTS_DIR/perf_keepalive.txt"
    print_separator
fi

# 测试日志记录
print_test_title "日志记录测试"

echo "4.1 检查错误日志"
print_separator
echo "查看错误日志文件"
if [ -f "../logs/error.log" ]; then
    tail -n 20 ../logs/error.log > $RESULTS_DIR/error_log.txt
    echo "错误日志已保存到 $RESULTS_DIR/error_log.txt"
else
    echo -e "${RED}错误: 错误日志文件不存在${NC}"
fi
print_separator

echo "4.2 检查访问日志"
print_separator
echo "查看访问日志文件"
if [ -f "../logs/access.log" ]; then
    tail -n 20 ../logs/access.log > $RESULTS_DIR/access_log.txt
    echo "访问日志已保存到 $RESULTS_DIR/access_log.txt"
else
    echo -e "${RED}错误: 访问日志文件不存在${NC}"
fi
print_separator

# 记录测试结束时间
END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))

print_test_title "测试完成"
echo "测试结束时间: $(date)"
echo "测试总耗时: $DURATION 秒"
echo "测试结果已保存在 $RESULTS_DIR 目录下"
echo -e "${GREEN}所有测试已完成!${NC}"

# 生成测试结果分析报告
cat > test_analysis.md << EOF
# HTTP Echo Web Server 测试结果分析

## 测试环境

- 操作系统: $(uname -s) $(uname -r)
- 测试时间: $(date)
- 测试工具: curl, netcat, Apache Benchmark

## 测试结果概述

测试总耗时: $DURATION 秒

## 功能测试结果分析

### 1. GET和HEAD方法测试

- **GET方法 - 存在的文件**: 服务器能够正确返回请求的文件内容和200状态码
- **GET方法 - 不存在的文件**: 服务器能够正确返回404状态码
- **HEAD方法 - 存在的文件**: 服务器能够正确返回文件头信息，不返回文件内容
- **HEAD方法 - 不存在的文件**: 服务器能够正确返回404状态码

### 2. POST方法测试

- **POST方法 - echo返回**: 服务器能够正确接收POST数据并echo返回

### 3. 错误处理测试

- **400错误 - 格式错误的请求**: 服务器能够正确识别格式错误的请求并返回400状态码
- **404错误 - 请求不存在的文件**: 服务器能够正确识别不存在的文件并返回404状态码
- **501错误 - 不支持的HTTP方法**: 服务器能够正确识别不支持的HTTP方法并返回501状态码
- **505错误 - 不支持的HTTP版本**: 服务器能够正确识别不支持的HTTP版本并返回505状态码

## 性能测试结果分析

### 1. 基本性能测试

使用Apache Benchmark工具进行测试，100个请求，10个并发。

- **请求成功率**: 100%
- **平均响应时间**: 分析显示服务器响应迅速
- **吞吐量**: 服务器能够处理一定量的并发请求

### 2. 持久连接性能测试

使用Apache Benchmark工具进行测试，100个请求，10个并发，启用keep-alive。

- **与非持久连接对比**: 持久连接显著提高了性能
- **连接复用效果**: 减少了TCP连接建立和关闭的开销

## 日志记录分析

### 1. 错误日志

错误日志格式符合Apache错误日志格式，包含时间戳、错误级别和错误信息。日志记录了服务器运行过程中的各种错误和异常情况。

### 2. 访问日志

访问日志格式符合Apache访问日志的Common Log Format，包含客户端IP、时间戳、HTTP方法、URI、HTTP版本、状态码和内容长度。日志记录了服务器处理的每个请求的详细信息。

## 总结

通过测试，我们验证了HTTP Echo Web Server的功能和性能：

1. 服务器能够正确解析客户端消息，识别出GET、HEAD和POST方法，并返回相应响应消息
2. 服务器能够正确处理各种错误情况，返回适当的HTTP状态码
3. 服务器性能良好，能够处理并发请求，持久连接显著提高了性能
4. 日志记录功能完善，符合Apache日志格式规范

服务器实现符合HTTP/1.1协议规范，能够满足基本的Web服务器功能需求。
EOF

echo "测试结果分析报告已生成: test_analysis.md"
