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

# 设置执行权限
chmod +x test_get.sh test_head.sh test_post.sh test_errors.sh test_performance.sh

# 检查服务器是否运行
if ! nc -z localhost 9999 >/dev/null 2>&1; then
    echo -e "${RED}错误: 服务器未在端口9999上运行${NC}"
    echo "请先启动服务器: cd .. && ./liso_server"
    exit 1
fi

# 运行所有测试
print_test_title "开始运行 HTTP Echo Web Server 测试套件"

print_test_title "1. GET 方法测试"
./test_get.sh

print_test_title "2. HEAD 方法测试"
./test_head.sh

print_test_title "3. POST 方法测试"
./test_post.sh

print_test_title "4. 错误处理测试"
./test_errors.sh

print_test_title "5. 性能测试"
./test_performance.sh

print_test_title "所有测试完成"
echo -e "${GREEN}测试结束。请查看上面的输出以了解测试结果。${NC}"
