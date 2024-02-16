all: SatTTC.out

HomeFolder := $(shell pwd)/..
# HomeFolder := ~/gamasat/TTEC-Gamacubedesign
# HomeFolder := /mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign
LibFolder := $(HomeFolder)

LoraLib := $(LibFolder)/sx1278-LoRa-RaspberryPi

# OFFLINE

CommunicationProtocol.o: CommunicationProtocol.h
	g++ -c CommunicationProtocol.h -o CommunicationProtocol.o

Timing.o: Timing.cpp
	g++ -c Timing.cpp -o Timing.o

Logger.o: Logger.cpp
	g++ -c Logger.cpp -o Logger.o -lrt -pthread -lm

RFModemOffline.o: RFModemOffline.cpp
	g++ -c RFModemOffline.cpp -o RFModemOffline.o -lrt -pthread -lm

RFModule.o: RFModule.cpp
	g++ -c RFModule.cpp -o RFModule.o -lrt -pthread -lm

StatusServer.o: StatusServer.cpp
	g++ -c StatusServer.cpp -o StatusServer.o

ImagingServer.o: ImagingServer.cpp
	g++ -c ImagingServer.cpp -o ImagingServer.o

DataFIFO.o: DataFIFO.cpp
	g++ -c DataFIFO.cpp -o DataFIFO.o

SatTTC.o: SatTTC.cpp
	g++ -c SatTTC.cpp -o SatTTC.o -lrt -pthread -lm

SatTTC.out: Timing.o Logger.o RFModule.o RFModemOffline.o SatTTC.o ImagingServer.o StatusServer.o DataFIFO.o
	g++ -o SatTTC.out SatTTC.o Timing.o Logger.o RFModule.o RFModemOffline.o ImagingServer.o StatusServer.o DataFIFO.o -lrt -pthread -lm

clean:
	rm *.o *.out

