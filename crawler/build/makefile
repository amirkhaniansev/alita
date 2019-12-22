all: ../../include/crawler.hpp ../../include/publisher.hpp ../../include/threadpool.hpp ../../include/lqueue.hpp ../../include/httpsc.hpp ../../include/parser.hpp crawler.o
		g++ ../crawler_main.cpp crawler.o -o crawler -lmqueue -lthreadpool -llqueue -lhttpsc -lssl -lcrypto -lparser
		
crawler.o: ../../include/crawler.hpp ../../include/publisher.hpp ../../include/threadpool.hpp ../../include/lqueue.hpp ../../include/httpsc.hpp ../../include/parser.hpp ../crawler.cpp
		g++ ../crawler.cpp -c -o crawler.o