all: main.out

HomeFolder := $(shell pwd)/..

DataFIFO.o: DataFIFO.cpp
	g++ -c DataFIFO.cpp -o DataFIFO.o

ImagingServer.o: ImagingServer.cpp
	g++ -c ImagingServer.cpp -o ImagingServer.o

StatusServer.o: StatusServer.cpp
	g++ -c StatusServer.cpp -o StatusServer.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

main.out: main.o ImagingServer.o StatusServer.o DataFIFO.o
	g++ -o main.out main.o ImagingServer.o StatusServer.o DataFIFO.o

clean:
	rm *.o *.out