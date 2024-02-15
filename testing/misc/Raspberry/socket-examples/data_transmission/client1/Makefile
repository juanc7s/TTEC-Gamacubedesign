all: client1.out

HomeFolder := $(shell pwd)/..

client1.o: client1.cpp
	g++ -c client1.cpp -o client1.o

client1.out: client1.o
	g++ -o client1.out client1.o

clean:
	rm *.o *.out