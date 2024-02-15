all: main.out

HomeFolder := $(shell pwd)/..

DataFIFO.o: DataFIFO.cpp
	g++ -c DataFIFO.cpp -o DataFIFO.o

server.o: server.cpp
	g++ -c server.cpp -o server.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

main.out: main.o server.o DataFIFO.o
	g++ -o main.out main.o server.o DataFIFO.o

clean:
	rm *.o *.out