all: SatTTC.out

HomeFolder := $(shell pwd)/..
# HomeFolder := ~/gamasat/TTEC-Gamacubedesign
# HomeFolder := /mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign
LibFolder := $(HomeFolder)

LoraLib := $(LibFolder)/sx1278-LoRa-RaspberryPi

# RADIO

# LoRa.o: /LoRa.c
# 	gcc -c $(LoraLib)/LoRa.c -o $(LoraLib)/LoRa.o -lpigpio -lrt -pthread -lm

# CommunicationProtocol.o: CommunicationProtocol.h
# 	g++ -c CommunicationProtocol.h -o CommunicationProtocol.o

# Logger.o: Logger.cpp
# 	g++ -c Logger.cpp -o Logger.o -lpigpio -lrt -pthread -lm

# RFModem.o: RFModem.cpp
# 	g++ -c RFModem.cpp -o RFModem.o -lpigpio -lrt -pthread -lm -I $(LibFolder)

# RFModule.o: RFModule.cpp
# 	g++ -c RFModule.cpp -o RFModule.o -lpigpio -lrt -pthread -lm -I $(LibFolder)

# SatTTC.o: SatTTC.cpp
# 	g++ -c SatTTC.cpp -o SatTTC.o -lpigpio -lrt -pthread -lm -I $(LibFolder)

# SatTTC.out: $(LoraLib)/LoRa.o Logger.o SatTTC.o RFModule.o RFModem.o
# 	g++ -o SatTTC.out SatTTC.o $(LoraLib)/LoRa.o Logger.o RFModule.o RFModem.o -lpigpio -lrt -pthread -lm

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

SatTTC.o: SatTTC.cpp
	g++ -c SatTTC.cpp -o SatTTC.o -lrt -pthread -lm

SatTTC.out: Timing.o Logger.o RFModule.o RFModemOffline.o SatTTC.o
	g++ -o SatTTC.out SatTTC.o Timing.o Logger.o RFModule.o RFModemOffline.o -lrt -pthread -lm

clean:
	rm *.o *.out

