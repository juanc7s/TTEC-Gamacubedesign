all: client_imaging.out

HomeFolder := $(shell pwd)/..

client_imaging.o: client_imaging.cpp
	g++ -c client_imaging.cpp -o client_imaging.o

client_imaging.out: client_imaging.o
	g++ -o client_imaging.out client_imaging.o

clean:
	rm *.o *.out
