all: tx

LoRa.o: LoRa.c
	gcc -c LoRa.c -o LoRa.o -lpigpio -lrt -pthread -lm

tx.o: tx_test.cpp
	g++ -c tx_test.cpp -o tx.o -lpigpio -lrt -pthread -lm

tx: LoRa.o tx.o
	g++ -o tx tx.o LoRa.o -lpigpio -lrt -pthread -lm

clean:
	rm *.o tx