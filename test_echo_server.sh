#!/bin/bash

echo "Testing Echo Web Server"
echo "======================="

# Test 1: GET request (should echo back)
echo -e "\nTest 1: GET request"
echo -e "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n" | nc localhost 9999

# Test 2: HEAD request (should echo back)
echo -e "\nTest 2: HEAD request"
echo -e "HEAD / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n" | nc localhost 9999

# Test 3: POST request (should echo back)
echo -e "\nTest 3: POST request"
echo -e "POST / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 11\r\nConnection: close\r\n\r\nHello World" | nc localhost 9999

# Test 4: DELETE request (should return 501 Not Implemented)
echo -e "\nTest 4: DELETE request (should return 501)"
echo -e "DELETE / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n" | nc localhost 9999

# Test 5: Bad request format (should return 400 Bad request)
echo -e "\nTest 5: Bad request format (should return 400)"
echo -e "This is not a valid HTTP request\r\n" | nc localhost 9999

echo -e "\nAll tests completed"
