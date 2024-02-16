all: main.out

HomeFolder := $(shell pwd)/..

ImagingDataClient.o: ImagingDataClient.cpp
	g++ -c ImagingDataClient.cpp -o ImagingDataClient.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

main.out: main.o ImagingDataClient.o
	g++ -o main.out main.o ImagingDataClient.o

clean:
	rm *.o *.out
