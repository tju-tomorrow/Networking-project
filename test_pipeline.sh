#!/bin/bash

SERVER_HOST="localhost"
SERVER_PORT=9999
REQUEST_FILE="pipeline_requests.txt"
OUTPUT_FILE="pipeline_results.txt"

# 创建包含20个连续GET请求的文件，每个请求之间无间隔
echo -e "GET /index.html HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n" > $REQUEST_FILE
for i in {1..19}; do
    echo -e "GET /index.html HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n" >> $REQUEST_FILE
done

# 使用netcat发送所有请求并记录响应
echo "Sending 20 pipelined requests..."
cat $REQUEST_FILE | nc $SERVER_HOST $SERVER_PORT > $OUTPUT_FILE

# 统计成功响应的数量
SUCCESS_COUNT=$(grep -c "HTTP/1.1 200 OK" $OUTPUT_FILE)
echo "Number of successful responses (200 OK): $SUCCESS_COUNT"

# 检查是否有错误响应
ERROR_400=$(grep -c "HTTP/1.1 400" $OUTPUT_FILE)
ERROR_501=$(grep -c "HTTP/1.1 501" $OUTPUT_FILE)
ERROR_505=$(grep -c "HTTP/1.1 505" $OUTPUT_FILE)
echo "Number of 400 errors: $ERROR_400"
echo "Number of 501 errors: $ERROR_501"
echo "Number of 505 errors: $ERROR_505"