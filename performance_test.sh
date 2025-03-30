#!/bin/bash

# 性能测试脚本

# 颜色定义
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

# 测试参数
REQUESTS=100  # 每种请求的测试次数
TEST_DATA="Hello, World!"  # POST请求的测试数据

# 计时函数
time_request() {
    local start=$(date +%s%N)
    $@
    local end=$(date +%s%N)
    echo $(( (end - start) / 1000000 )) # 转换为毫秒
}

echo -e "${BLUE}开始性能测试...${NC}"
echo "每种请求测试 $REQUESTS 次"
echo

# 测试 GET 请求
echo -e "${GREEN}测试 GET 请求性能${NC}"
total_time=0
for i in $(seq 1 $REQUESTS); do
    time_ms=$(time_request ./echo_client localhost 9999 GET / 2>/dev/null)
    total_time=$((total_time + time_ms))
    echo -ne "\r进度: $i/$REQUESTS"
done
echo
avg_time=$(echo "scale=2; $total_time / $REQUESTS" | bc)
echo "GET 请求平均响应时间: ${avg_time}ms"
echo

# 测试 HEAD 请求
echo -e "${GREEN}测试 HEAD 请求性能${NC}"
total_time=0
for i in $(seq 1 $REQUESTS); do
    time_ms=$(time_request ./echo_client localhost 9999 HEAD / 2>/dev/null)
    total_time=$((total_time + time_ms))
    echo -ne "\r进度: $i/$REQUESTS"
done
echo
avg_time=$(echo "scale=2; $total_time / $REQUESTS" | bc)
echo "HEAD 请求平均响应时间: ${avg_time}ms"
echo

# 测试 POST 请求
echo -e "${GREEN}测试 POST 请求性能${NC}"
total_time=0
for i in $(seq 1 $REQUESTS); do
    time_ms=$(time_request ./echo_client localhost 9999 POST / "$TEST_DATA" 2>/dev/null)
    total_time=$((total_time + time_ms))
    echo -ne "\r进度: $i/$REQUESTS"
done
echo
avg_time=$(echo "scale=2; $total_time / $REQUESTS" | bc)
echo "POST 请求平均响应时间: ${avg_time}ms"
echo

# 测试错误请求处理
echo -e "${GREEN}测试错误请求处理性能${NC}"
total_time=0
for i in $(seq 1 $REQUESTS); do
    time_ms=$(time_request ./echo_client localhost 9999 BAD 2>/dev/null)
    total_time=$((total_time + time_ms))
    echo -ne "\r进度: $i/$REQUESTS"
done
echo
avg_time=$(echo "scale=2; $total_time / $REQUESTS" | bc)
echo "错误请求平均响应时间: ${avg_time}ms"
echo
