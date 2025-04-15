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

# 测试用例1：基本 GET 请求 - 存在的文件
print_test_case "GET 请求 - 存在的文件"
print_separator
echo "发送: GET /index.html HTTP/1.1"
curl -v http://localhost:9999/index.html
print_separator

# 测试用例2：GET 请求 - 不存在的文件 (应返回404)
print_test_case "GET 请求 - 不存在的文件 (应返回404)"
print_separator
echo "发送: GET /nonexistent.html HTTP/1.1"
curl -v http://localhost:9999/nonexistent.html
print_separator

# 测试用例3：GET 请求 - 根目录 (应返回默认页面)
print_test_case "GET 请求 - 根目录 (应返回默认页面)"
print_separator
echo "发送: GET / HTTP/1.1"
curl -v http://localhost:9999/
print_separator

echo -e "${GREEN}GET 方法测试完成${NC}"
