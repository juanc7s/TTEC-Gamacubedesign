all: SatTTC.out

HomeFolder := $(shell pwd)/..
# HomeFolder := ~/gamasat/TTEC-Gamacubedesign
# HomeFolder := /mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign
LibFolder := $(HomeFolder)

LoraLib := $(LibFolder)/sx1278-LoRa-RaspberryPi

# MAIN

# CommunicationProtocol.o: CommunicationProtocol.h
# 	g++ -c CommunicationProtocol.h -o CommunicationProtocol.o

Timing.o: Timing.cpp
	g++ -c Timing.cpp -o Timing.o

Logger.o: Logger.cpp
	g++ -c Logger.cpp -o Logger.o -lrt -pthread -lm

# RFModemOffline.o: RFModemOffline.cpp
# 	g++ -c RFModemOffline.cpp -o RFModemOffline.o -lrt -pthread -lm

RFModem.o: RFModem.cpp
	g++ -c RFModem.cpp -o RFModem.o -lrt -pthread -lm

RFModule.o: RFModule.cpp
	g++ -c RFModule.cpp -o RFModule.o -lrt -pthread -lm

DataFIFO.o: DataFIFO.cpp
	g++ -c DataFIFO.cpp -o DataFIFO.o

SatTTC.o: SatTTC.cpp
	g++ -c SatTTC.cpp -o SatTTC.o -lpigpio -lrt -pthread -lm

SatTTC.out: Timing.o Logger.o RFModule.o RFModem.o SatTTC.o DataFIFO.o
	g++ -o SatTTC.out SatTTC.o Timing.o Logger.o RFModule.o RFModemOffline.o ImagingServer.o StatusServer.o DataFIFO.o -lpigpio -lrt -pthread -lm

clean:
	rm *.o *.out

