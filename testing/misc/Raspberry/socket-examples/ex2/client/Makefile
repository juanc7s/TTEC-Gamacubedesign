all: client.out

HomeFolder := $(shell pwd)/..

client.o: client.c
	g++ -c client.c -o client.o

client.out: client.o
	g++ -o client.out client.o

clean:
	rm *.o *.out