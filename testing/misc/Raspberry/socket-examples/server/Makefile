all: server.out

HomeFolder := $(shell pwd)/..

server.o: server.c
	g++ -c server.c -o server.o

server.out: server.o
	g++ -o server.out server.o

clean:
	rm *.o *.out