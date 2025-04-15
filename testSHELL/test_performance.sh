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

# 检查是否安装了ab工具
if ! command -v ab &> /dev/null; then
    echo -e "${RED}错误: Apache Benchmark (ab) 工具未安装${NC}"
    echo "请安装Apache Benchmark工具后再运行此测试"
    echo "在Mac上可以使用: brew install httpd"
    echo "在Ubuntu上可以使用: sudo apt-get install apache2-utils"
    exit 1
fi

# 测试用例1：基本性能测试 - 100个请求，10个并发
print_test_case "基本性能测试 - 100个请求，10个并发"
print_separator
echo "执行: ab -n 100 -c 10 http://localhost:9999/index.html"
ab -n 100 -c 10 http://localhost:9999/index.html
print_separator

# 测试用例2：高并发性能测试 - 500个请求，50个并发
print_test_case "高并发性能测试 - 500个请求，50个并发"
print_separator
echo "执行: ab -n 500 -c 50 http://localhost:9999/index.html"
ab -n 500 -c 50 http://localhost:9999/index.html
print_separator

# 测试用例3：持久连接性能测试 - 使用keep-alive
print_test_case "持久连接性能测试 - 使用keep-alive"
print_separator
echo "执行: ab -n 100 -c 10 -k http://localhost:9999/index.html"
ab -n 100 -c 10 -k http://localhost:9999/index.html
print_separator

echo -e "${GREEN}性能测试完成${NC}"
