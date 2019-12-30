all: ../../include/server.hpp ../../include/search.hpp ../../include/httplib.hpp ../../include/db.hpp server.o
		g++ ../server_main.cpp server.o -o server -lsearch -lpthread -lalitadb
		
server.o: ../../include/server.hpp ../../include/search.hpp ../../include/httplib.hpp ../../include/db.hpp
		g++ ../server.cpp -c -o server.o