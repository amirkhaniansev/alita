all: http_s parser rabbit dbconnect

http_s: http https

rabbit: publisher subscriber

http: ../../include/httplib.hpp ../https/http.cpp packages
		g++ ../https/http.cpp -o http_poc

https: ../../include/httplib.hpp ../https/https.cpp packages
		g++ ../https/https.cpp -o https_poc -lcrypto -lssl -lcurl

parser: ../parser/parser.cpp packages
		g++ ../parser/parser.cpp -o parser

publisher: ../rabbit/publisher.cpp packages
		g++ ../rabbit/publisher.cpp -o publisher -lSimpleAmqpClient

subscriber: ../rabbit/subscriber.cpp packages
		g++ ../rabbit/subscriber.cpp -o subscriber -lSimpleAmqpClient

dbconnect: ../dbconnect/dbconnect.cpp packages
		g++ ../dbconnect/dbconnect.cpp -o dbconnect -lmysqlcppconn

packages: mysql

mysql:
		sudo apt-get install libmysqlcppconn-dev