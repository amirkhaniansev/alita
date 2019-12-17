mqueue: ../../include/publisher.hpp ../../include/subscriber.hpp cpub.o csub.o
		g++ cpub.o csub.o -shared -fPIC -lSimpleAmqpClient -o /usr/lib/libmqueue.so

cpub.o : ../../include/publisher.hpp ../publisher.cpp
		g++ ../publisher.cpp -c -fPIC -o cpub.o

csub.o : ../../include/subscriber.hpp ../subscriber.cpp
		g++ ../subscriber.cpp -c -fPIC -o csub.o