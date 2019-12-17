threadpool: ../../include/threadpool.hpp threadpool.o
		g++ threadpool.o -shared -fPIC -pthread -o /usr/lib/libthreadpool.so

threadpool.o: ../../include/threadpool.hpp ../threadpool.cpp
		g++ ../threadpool.cpp -c -fPIC -o threadpool.o