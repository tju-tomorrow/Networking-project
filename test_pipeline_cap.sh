#!/bin/bash

SERVER_HOST="localhost"
SERVER_PORT=9999
REQUEST_FILE="pipeline_requests.txt"
OUTPUT_FILE="pipeline_results.txt"
LOG_FILE="test_log.txt"

# 确保服务器正在运行，如果没有则启动服务器
if ! nc -z $SERVER_HOST $SERVER_PORT; then
    echo "Server not running, starting server..."
    ./liso_server &
    SERVER_PID=$!
    sleep 2  # 等待服务器启动
fi

# 创建包含20个连续GET请求的文件，每个请求之间无间隔
echo "Creating request file with 20 pipelined requests..."
> $REQUEST_FILE
for i in {1..20}; do
    echo -e "GET /index.html HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n" >> $REQUEST_FILE
done

# 使用netcat发送所有请求并记录响应，同时记录处理时间
echo "Sending 20 pipelined requests and measuring time..." | tee -a $LOG_FILE
start_time=$(date +%s.%N)
cat $REQUEST_FILE | nc -w 3 $SERVER_HOST $SERVER_PORT > $OUTPUT_FILE
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)

# 统计成功响应的数量和错误响应
SUCCESS_COUNT=$(grep -c "HTTP/1.1 200 OK" $OUTPUT_FILE)
ERROR_400=$(grep -c "HTTP/1.1 400" $OUTPUT_FILE)
ERROR_501=$(grep -c "HTTP/1.1 501" $OUTPUT_FILE)
ERROR_505=$(grep -c "HTTP/1.1 505" $OUTPUT_FILE)

# 输出测试结果
echo "Test Results:" | tee -a $LOG_FILE
echo "Number of successful responses (200 OK): $SUCCESS_COUNT" | tee -a $LOG_FILE
echo "Number of 400 errors: $ERROR_400" | tee -a $LOG_FILE
echo "Number of 501 errors: $ERROR_501" | tee -a $LOG_FILE
echo "Number of 505 errors: $ERROR_505" | tee -a $LOG_FILE
echo "Total processing time for 20 requests: $duration seconds" | tee -a $LOG_FILE
echo "Average time per request: $(echo "$duration / 20" | bc -l) seconds" | tee -a $LOG_FILE

# 清理：如果启动了服务器，则关闭它
if [ -n "$SERVER_PID" ]; then
    kill $SERVER_PID
fi