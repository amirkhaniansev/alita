all: lqueue_enqueue.test lqueue_dequeue.test https.test threadpool.test parser.test db.test search.test

https.test: ../../include/httpsc.hpp ../https/https_test.cpp
		g++ ../https/https_test.cpp -o https.test -lhttpsc -lssl -lcrypto

lqueue_enqueue.test: ../../include/lqueue.hpp ../lqueue/lqueue_enqueue_test.cpp
		g++ ../lqueue/lqueue_enqueue_test.cpp -o lqueue_enqueue.test -llqueue

lqueue_dequeue.test: ../../include/lqueue.hpp ../lqueue/lqueue_dequeue_test.cpp
		g++ ../lqueue/lqueue_dequeue_test.cpp -o lqueue_dequeue.test -llqueue

threadpool.test: ../../include/threadpool.hpp ../threadpool/ttest.cpp
		g++ ../threadpool/ttest.cpp -o threadpool.test -lthreadpool

parser.test: ../../include/parser.hpp ../parser/parser_test.cpp
		g++ ../parser/parser_test.cpp -o parser.test -lparser

db.test: ../../include/db.hpp ../db/db_test.cpp
		g++ ../db/db_test.cpp -o db.test -lalitadb -lmysqlcppconn

search.test: ../../include/search.hpp ../search/search_test.cpp
		g++ ../search/search_test.cpp -o search.test -lalitadb -lsearch