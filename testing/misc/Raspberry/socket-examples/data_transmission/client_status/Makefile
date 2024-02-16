all: main.out

HomeFolder := $(shell pwd)/..

StatusDataClient.o: StatusDataClient.cpp
	g++ -c StatusDataClient.cpp -o StatusDataClient.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

main.out: main.o StatusDataClient.o
	g++ -o main.out main.o StatusDataClient.o

clean:
	rm *.o *.out
