httpsclib: httpsc.o ../../include/httpsc.hpp
		g++ httpsc.o -shared -lcrypto -lssl -lcurl -o /usr/lib/libhttpsc.so

httpsc.o: ../../include/httpsc.hpp ../httpsc.cpp
		g++ ../httpsc.cpp -fPIC -c -o httpsc.o