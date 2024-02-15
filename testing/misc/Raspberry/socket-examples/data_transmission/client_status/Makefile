all: client_status.out

HomeFolder := $(shell pwd)/..

client_status.o: client_status.cpp
	g++ -c client_status.cpp -o client_status.o

client_status.out: client_status.o
	g++ -o client_status.out client_status.o

clean:
	rm *.o *.out
