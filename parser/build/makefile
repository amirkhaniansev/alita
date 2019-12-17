parser: ../../include/parser.hpp parser.o
		g++ parser.o -shared -fPIC -pthread -lmyhtml -o /usr/lib/libparser.so

parser.o: ../../include/parser.hpp ../parser.cpp
		g++ ../parser.cpp -c -fPIC -o parser.o