all: SatTTC

LibFolder := /mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/libraries

LoRa.o: $(LibFolder)/sx1278-LoRa-RaspberryPi/LoRa.c
	gcc -c $(LibFolder)/sx1278-LoRa-RaspberryPi/LoRa.c -o $(LibFolder)/sx1278-LoRa-RaspberryPi/LoRa.o -lpigpio -lrt -pthread -lm

CommunicationProtocol.o: CommunicationProtocol.h
	g++ -c CommunicationProtocol.h -o CommunicationProtocol.o

Logger.o: Logger.cpp
	g++ -c Logger.cpp -o Logger.o -lpigpio -lrt -pthread -lm

RFModule.o: RFModule.cpp
	g++ -c RFModule.cpp -o RFModule.o -lpigpio -lrt -pthread -lm

SatTTC.o: SatTTC.cpp
	g++ -c SatTTC.cpp -o SatTTC.o -lpigpio -lrt -pthread -lm

SatTTC.out: LoRa.o Logger.o RFModule.o SatTTC.o
	g++ -o SatTTC.out SatTTC.o ./libs/sx1278-LoRa-RaspberryPi/LoRa.o RFModule.o SatTTC.o -lpigpio -lrt -pthread -lm

clean:
	rm *.o *.out SatTTC.out

