all: SatTTC.out

HomeFolder := $(shell pwd)/..
# HomeFolder := ~/gamasat/TTEC-Gamacubedesign
# HomeFolder := /mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign
LibFolder := $(HomeFolder)

LoraLib := $(LibFolder)/sx1278-LoRa-RaspberryPi

LoRa.o: /LoRa.c
	gcc -c $(LoraLib)/LoRa.c -o $(LoraLib)/LoRa.o -lpigpio -lrt -pthread -lm

CommunicationProtocol.o: CommunicationProtocol.h
	g++ -c CommunicationProtocol.h -o CommunicationProtocol.o

Logger.o: Logger.cpp
	g++ -c Logger.cpp -o Logger.o -lpigpio -lrt -pthread -lm

RFModule.o: RFModule.cpp
	g++ -c RFModule.cpp -o RFModule.o -lpigpio -lrt -pthread -lm

StatusDataServer.o: StatusDataServer.cpp
	g++ -c StatusDataServer.cpp -o StatusDataServer.o

ImagingDataServer.o: ImagingDataServer.cpp
	g++ -c ImagingDataServer.cpp -o ImagingDataServer.o

DataFIFO.o: DataFIFO.cpp
	g++ -c DataFIFO.cpp -o DataFIFO.o

SatTTC.o: SatTTC.cpp
	g++ -c SatTTC.cpp -o SatTTC.o -lpigpio -lrt -pthread -lm -I $(LibFolder)

SatTTC.out: $(LoraLib)/LoRa.o Logger.o SatTTC.o ImagingDataServer.o StatusDataServer.o DataFIFO.o
	g++ -o SatTTC.out SatTTC.o $(LoraLib)/LoRa.o Logger.o ImagingDataServer.o StatusDataServer.o DataFIFO.o -lpigpio -lrt -pthread -lm

clean:
	rm *.o *.out

