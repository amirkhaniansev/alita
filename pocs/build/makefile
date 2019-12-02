all: http_s parser rabbit dbconnect

http_s: http https

rabbit: publisher subscriber

http: ../../include/httplib.hpp ../https/http.cpp
		g++ ../https/http.cpp -o http_poc

https: ../../include/httplib.hpp ../https/https.cpp
		g++ ../https/https.cpp -o https_poc -lcrypto -lssl

parser: ../parser/parser.cpp
		g++ ../parser/parser.cpp -o parser

publisher: ../rabbit/publisher.cpp
		g++ ../rabbit/publisher.cpp -o publisher -lSimpleAmqpClient

subscriber: ../rabbit/subscriber.cpp
		g++ ../rabbit/subscriber.cpp -o subscriber -lSimpleAmqpClient

dbconnect: ../dbconnect/dbconnect.cpp
		g++ ../dbconnect/dbconnect.cpp -o dbconnect -lmysqlcppconn