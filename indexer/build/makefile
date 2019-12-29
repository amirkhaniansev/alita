all: ../../include/indexer.hpp ../../include/db.hpp ../../include/parser.hpp ../../include/subscriber.hpp indexer.o
		g++ ../indexer_main.cpp indexer.o -g -o indexer -lalitadb -lmqueue -lparser
		
indexer.o: ../../include/indexer.hpp ../../include/db.hpp ../../include/parser.hpp ../../include/subscriber.hpp ../indexer.cpp
		g++ ../indexer.cpp -c -o indexer.o