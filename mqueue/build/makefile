mqueue: ../../include/publisher.hpp ../../include/subscriber.hpp cpub.o csub.o
		g++ cpub.o csub.o -shared -fPIC -lSimpleAmqpClient -o /usr/lib/libmqueue.so

cpub.o : ../../include/publisher.hpp ../publisher.cpp packages
		g++ ../publisher.cpp -c -fPIC -o cpub.o

csub.o : ../../include/subscriber.hpp ../subscriber.cpp packages
		g++ ../subscriber.cpp -c -fPIC -o csub.o

packages: boost rabbitmqc rabbitmqcpp

boost:
		sudo apt-get install libboost-all-dev

rabbitmqc:
		git clone https://github.com/alanxz/rabbitmq-c
		cd ./rabbitmq-c && mkdir build && cd ./build && sudo cmake .. && sudo cmake --build . --target install
		sudo rm -rf ./rabbitmq-c

rabbitmqcpp:
		git clone https://github.com/alanxz/SimpleAmqpClient
		cd ./SimpleAmqpClient && mkdir build && cd ./build && sudo cmake .. && sudo cmake --build . --target install
		sudo rm -rf ./SimpleAmqpClient