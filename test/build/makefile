all: lqueue_enqueue.test lqueue_dequeue.test

lqueue_enqueue.test: ../../include/lqueue.hpp ../lqueue/lqueue_enqueue_test.cpp
		g++ ../lqueue/lqueue_enqueue_test.cpp -o lqueue_enqueue.test -llqueue

lqueue_dequeue.test: ../../include/lqueue.hpp ../lqueue/lqueue_dequeue_test.cpp
		g++ ../lqueue/lqueue_dequeue_test.cpp -o lqueue_dequeue.test -llqueue