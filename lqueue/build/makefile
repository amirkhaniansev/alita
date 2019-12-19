lqueue.lib: lqueue.o ../../include/lqueue.hpp
	g++ lqueue.o -shared -pthread -o /usr/lib/liblqueue.so

lqueue.o: ../../include/lqueue.hpp ../lqueue.cpp
	g++ ../lqueue.cpp -fPIC -c -o lqueue.o 