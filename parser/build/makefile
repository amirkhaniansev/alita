parser: ../../include/parser.hpp parser.o
		g++ parser.o -shared -fPIC -pthread -lmyhtml -o /usr/lib/libparser.so

parser.o: ../../include/parser.hpp ../parser.cpp packages
		g++ ../parser.cpp -c -fPIC -o parser.o

packages:
		git clone https://github.com/lexborisov/myhtml
		cd ./myhtml && make && make test && sudo make install
		sudo rm -rf ./myhtml