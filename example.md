chenkaixuan@chenkaixuandeMacBook-Air webServerStartCodes-2025 % ./example samples/request_get
t:token_char G
token: Matched rule 1.
t:token_char E
token: Matched rule 2.
t:token_char T
token: Matched rule 2.
t:sp ' '; 
t:slash; 
text: Matched rule 1.
t:sp ' '; 
t:token_char H
text: Matched rule 1.
t:token_char T
OWS: Matched rule 1
text: Matched rule 2.
t:token_char T
OWS: Matched rule 1
text: Matched rule 2.
t:token_char P
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 1; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 1; 
OWS: Matched rule 1
text: Matched rule 2.
t:crlf; 
request_Line:
GET
/
HTTP/1.1
t:token_char H
token: Matched rule 1.
t:token_char o
token: Matched rule 2.
t:token_char s
token: Matched rule 2.
t:token_char t
token: Matched rule 2.
t:colon; 
OWS: Matched rule 1
t:sp ' '; 
OWS: Matched rule 2
t:token_char w
text: Matched rule 1.
t:token_char w
OWS: Matched rule 1
text: Matched rule 2.
t:token_char w
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:token_char c
OWS: Matched rule 1
text: Matched rule 2.
t:token_char s
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:token_char c
OWS: Matched rule 1
text: Matched rule 2.
t:token_char m
OWS: Matched rule 1
text: Matched rule 2.
t:token_char u
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char d
OWS: Matched rule 1
text: Matched rule 2.
t:token_char u
OWS: Matched rule 1
text: Matched rule 2.
t:crlf; 
OWS: Matched rule 1
request_Header:
Host
www.cs.cmu.edu
t:token_char C
token: Matched rule 1.
t:token_char o
token: Matched rule 2.
t:token_char n
token: Matched rule 2.
t:token_char n
token: Matched rule 2.
t:token_char e
token: Matched rule 2.
t:token_char c
token: Matched rule 2.
t:token_char t
token: Matched rule 2.
t:token_char i
token: Matched rule 2.
t:token_char o
token: Matched rule 2.
t:token_char n
token: Matched rule 2.
t:colon; 
OWS: Matched rule 1
t:sp ' '; 
OWS: Matched rule 2
t:token_char k
text: Matched rule 1.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char p
OWS: Matched rule 1
text: Matched rule 2.
t:token_char -
OWS: Matched rule 1
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char v
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:crlf; 
OWS: Matched rule 1
request_Header:
Connection
keep-alive
t:token_char A
token: Matched rule 1.
t:token_char c
token: Matched rule 2.
t:token_char c
token: Matched rule 2.
t:token_char e
token: Matched rule 2.
t:token_char p
token: Matched rule 2.
t:token_char t
token: Matched rule 2.
t:colon; 
OWS: Matched rule 1
t:sp ' '; 
OWS: Matched rule 2
t:token_char t
text: Matched rule 1.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char x
OWS: Matched rule 1
text: Matched rule 2.
t:token_char t
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:token_char h
OWS: Matched rule 1
text: Matched rule 2.
t:token_char t
OWS: Matched rule 1
text: Matched rule 2.
t:token_char m
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:separators ','
OWS: Matched rule 1
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:token_char p
OWS: Matched rule 1
text: Matched rule 2.
t:token_char p
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char c
OWS: Matched rule 1
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:token_char t
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char o
OWS: Matched rule 1
text: Matched rule 2.
t:token_char n
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:token_char x
OWS: Matched rule 1
text: Matched rule 2.
t:token_char h
OWS: Matched rule 1
text: Matched rule 2.
t:token_char t
OWS: Matched rule 1
text: Matched rule 2.
t:token_char m
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:token_char +
OWS: Matched rule 1
text: Matched rule 2.
t:token_char x
OWS: Matched rule 1
text: Matched rule 2.
t:token_char m
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:separators ','
OWS: Matched rule 1
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:token_char p
OWS: Matched rule 1
text: Matched rule 2.
t:token_char p
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char c
OWS: Matched rule 1
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:token_char t
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char o
OWS: Matched rule 1
text: Matched rule 2.
t:token_char n
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:token_char x
OWS: Matched rule 1
text: Matched rule 2.
t:token_char m
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:separators ';'
OWS: Matched rule 1
text: Matched rule 2.
t:token_char q
OWS: Matched rule 1
text: Matched rule 2.
t:separators '='
OWS: Matched rule 1
text: Matched rule 2.
t:digit 0; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 9; 
OWS: Matched rule 1
text: Matched rule 2.
t:separators ','
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char m
OWS: Matched rule 1
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:token_char g
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:token_char w
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char b
OWS: Matched rule 1
text: Matched rule 2.
t:token_char p
OWS: Matched rule 1
text: Matched rule 2.
t:separators ','
OWS: Matched rule 1
text: Matched rule 2.
t:token_char *
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:token_char *
OWS: Matched rule 1
text: Matched rule 2.
t:separators ';'
OWS: Matched rule 1
text: Matched rule 2.
t:token_char q
OWS: Matched rule 1
text: Matched rule 2.
t:separators '='
OWS: Matched rule 1
text: Matched rule 2.
t:digit 0; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 8; 
OWS: Matched rule 1
text: Matched rule 2.
t:crlf; 
OWS: Matched rule 1
request_Header:
Accept
text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
t:token_char U
token: Matched rule 1.
t:token_char s
token: Matched rule 2.
t:token_char e
token: Matched rule 2.
t:token_char r
token: Matched rule 2.
t:token_char -
token: Matched rule 2.
t:token_char A
token: Matched rule 2.
t:token_char g
token: Matched rule 2.
t:token_char e
token: Matched rule 2.
t:token_char n
token: Matched rule 2.
t:token_char t
token: Matched rule 2.
t:colon; 
OWS: Matched rule 1
t:sp ' '; 
OWS: Matched rule 2
t:token_char M
text: Matched rule 1.
t:token_char o
OWS: Matched rule 1
text: Matched rule 2.
t:token_char z
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 5; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 0; 
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:separators '('
text: Matched rule 2.
t:token_char X
OWS: Matched rule 1
text: Matched rule 2.
t:digit 1; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 1; 
OWS: Matched rule 1
text: Matched rule 2.
t:separators ';'
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:token_char L
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char n
OWS: Matched rule 1
text: Matched rule 2.
t:token_char u
OWS: Matched rule 1
text: Matched rule 2.
t:token_char x
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:token_char x
text: Matched rule 2.
t:digit 8; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 6; 
OWS: Matched rule 1
text: Matched rule 2.
t:token_char _
OWS: Matched rule 1
text: Matched rule 2.
t:digit 6; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 4; 
OWS: Matched rule 1
text: Matched rule 2.
t:separators ')'
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:token_char A
text: Matched rule 2.
t:token_char p
OWS: Matched rule 1
text: Matched rule 2.
t:token_char p
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char W
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char b
OWS: Matched rule 1
text: Matched rule 2.
t:token_char K
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char t
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 5; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 3; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 7; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 3; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 6; 
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:separators '('
text: Matched rule 2.
t:token_char K
OWS: Matched rule 1
text: Matched rule 2.
t:token_char H
OWS: Matched rule 1
text: Matched rule 2.
t:token_char T
OWS: Matched rule 1
text: Matched rule 2.
t:token_char M
OWS: Matched rule 1
text: Matched rule 2.
t:token_char L
OWS: Matched rule 1
text: Matched rule 2.
t:separators ','
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:token_char l
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char k
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:token_char G
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char c
OWS: Matched rule 1
text: Matched rule 2.
t:token_char k
OWS: Matched rule 1
text: Matched rule 2.
t:token_char o
OWS: Matched rule 1
text: Matched rule 2.
t:separators ')'
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:token_char C
text: Matched rule 2.
t:token_char h
OWS: Matched rule 1
text: Matched rule 2.
t:token_char r
OWS: Matched rule 1
text: Matched rule 2.
t:token_char o
OWS: Matched rule 1
text: Matched rule 2.
t:token_char m
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 3; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 9; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 0; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 2; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 1; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 7; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 1; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 9; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 9; 
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:token_char S
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:token_char f
OWS: Matched rule 1
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:token_char r
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:slash; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 5; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 3; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 7; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 3; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 6; 
OWS: Matched rule 1
text: Matched rule 2.
t:crlf; 
OWS: Matched rule 1
request_Header:
User-Agent
Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.99 Safari/537.36
t:token_char A
token: Matched rule 1.
t:token_char c
token: Matched rule 2.
t:token_char c
token: Matched rule 2.
t:token_char e
token: Matched rule 2.
t:token_char p
token: Matched rule 2.
t:token_char t
token: Matched rule 2.
t:token_char -
token: Matched rule 2.
t:token_char E
token: Matched rule 2.
t:token_char n
token: Matched rule 2.
t:token_char c
token: Matched rule 2.
t:token_char o
token: Matched rule 2.
t:token_char d
token: Matched rule 2.
t:token_char i
token: Matched rule 2.
t:token_char n
token: Matched rule 2.
t:token_char g
token: Matched rule 2.
t:colon; 
OWS: Matched rule 1
t:sp ' '; 
OWS: Matched rule 2
t:token_char g
text: Matched rule 1.
t:token_char z
OWS: Matched rule 1
text: Matched rule 2.
t:token_char i
OWS: Matched rule 1
text: Matched rule 2.
t:token_char p
OWS: Matched rule 1
text: Matched rule 2.
t:separators ','
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:token_char d
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char f
OWS: Matched rule 1
text: Matched rule 2.
t:token_char l
OWS: Matched rule 1
text: Matched rule 2.
t:token_char a
OWS: Matched rule 1
text: Matched rule 2.
t:token_char t
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:separators ','
OWS: Matched rule 1
text: Matched rule 2.
t:sp ' '; 
OWS: Matched rule 2
t:token_char s
text: Matched rule 2.
t:token_char d
OWS: Matched rule 1
text: Matched rule 2.
t:token_char c
OWS: Matched rule 1
text: Matched rule 2.
t:token_char h
OWS: Matched rule 1
text: Matched rule 2.
t:crlf; 
OWS: Matched rule 1
request_Header:
Accept-Encoding
gzip, deflate, sdch
t:token_char A
token: Matched rule 1.
t:token_char c
token: Matched rule 2.
t:token_char c
token: Matched rule 2.
t:token_char e
token: Matched rule 2.
t:token_char p
token: Matched rule 2.
t:token_char t
token: Matched rule 2.
t:token_char -
token: Matched rule 2.
t:token_char L
token: Matched rule 2.
t:token_char a
token: Matched rule 2.
t:token_char n
token: Matched rule 2.
t:token_char g
token: Matched rule 2.
t:token_char u
token: Matched rule 2.
t:token_char a
token: Matched rule 2.
t:token_char g
token: Matched rule 2.
t:token_char e
token: Matched rule 2.
t:colon; 
OWS: Matched rule 1
t:sp ' '; 
OWS: Matched rule 2
t:token_char e
text: Matched rule 1.
t:token_char n
OWS: Matched rule 1
text: Matched rule 2.
t:token_char -
OWS: Matched rule 1
text: Matched rule 2.
t:token_char U
OWS: Matched rule 1
text: Matched rule 2.
t:token_char S
OWS: Matched rule 1
text: Matched rule 2.
t:separators ','
OWS: Matched rule 1
text: Matched rule 2.
t:token_char e
OWS: Matched rule 1
text: Matched rule 2.
t:token_char n
OWS: Matched rule 1
text: Matched rule 2.
t:separators ';'
OWS: Matched rule 1
text: Matched rule 2.
t:token_char q
OWS: Matched rule 1
text: Matched rule 2.
t:separators '='
OWS: Matched rule 1
text: Matched rule 2.
t:digit 0; 
OWS: Matched rule 1
text: Matched rule 2.
t:dot; 
OWS: Matched rule 1
text: Matched rule 2.
t:digit 8; 
OWS: Matched rule 1
text: Matched rule 2.
t:crlf; 
OWS: Matched rule 1
request_Header:
Accept-Language
en-US,en;q=0.8
t:crlf; 
parsing_request: Matched Success.
Http Method GET
Http Version HTTP/1.1
Http Uri /
Request Header
Header name Host Header Value www.cs.cmu.edu
Request Header
Header name Connection Header Value keep-alive
Request Header
Header name Accept Header Value text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Request Header
Header name User-Agent Header Value Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.99 Safari/537.36
Request Header
Header name Accept-Encoding Header Value gzip, deflate, sdch
Request Header
Header name Accept-Language Header Value en-US,en;q=0.8