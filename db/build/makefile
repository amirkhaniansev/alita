db.lib: db.o ../../include/db.hpp
		g++ db.o -shared -pthread -lmysqlcppconn -o /usr/lib/libalitadb.so

db.o: ../../include/db.hpp ../api/alita_db.cpp mysql
		g++ ../api/alita_db.cpp -fPIC -c -o db.o

mysql:
		sudo apt-get install libmysqlcppconn-dev