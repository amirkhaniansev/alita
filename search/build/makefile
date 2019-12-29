search.lib: ../../include/search.hpp ../../include/db.hpp ../search.cpp search.o
		g++ search.o -shared -pthread -lalitadb -o /usr/lib/libsearch.so

search.o: ../../include/search.hpp ../../include/db.hpp ../search.cpp
		g++ ../search.cpp -fPIC -c -o search.o 