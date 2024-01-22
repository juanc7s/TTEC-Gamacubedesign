all: SatTTaC

LoRa.o: ./libs/sx1278-LoRa-RaspberryPi/LoRa.c
	gcc -c ./libs/sx1278-LoRa-RaspberryPi/LoRa.c -o ./libs/sx1278-LoRa-RaspberryPi/LoRa.o -lpigpio -lrt -pthread -lm

SatTTaC.o: SatTTaC.cpp
	g++ -c SatTTaC.cpp -o SatTTaC.o -lpigpio -lrt -pthread -lm

SatTTaC: LoRa.o SatTTaC.o
	g++ -o SatTTaC SatTTaC.o ./libs/sx1278-LoRa-RaspberryPi/LoRa.o -lpigpio -lrt -pthread -lm

clean:
	rm *.o SatTTaC



# INCLUDES = -I./libs/sx1278-LoRa-RaspberryPi

# all: SatTTaC

# LoRa.o: ./libs/sx1278-LoRa-RaspberryPi/LoRa.c
# 	gcc -c ./libs/sx1278-LoRa-RaspberryPi/LoRa.c -o ./libs/sx1278-LoRa-RaspberryPi/LoRa.o -lpigpio -lrt -pthread -lm

# SatTTaC.o: SatTTaC.cpp
# 	g++ -c SatTTaC.cpp -o SatTTaC.o -lpigpio -lrt -pthread -lm

# SatTTaC: ./libs/sx1278-LoRa-RaspberryPi/LoRa.o SatTTaC.o
# 	g++ -o SatTTaC SatTTaC.o ./libs/sx1278-LoRa-RaspberryPi/LoRa.o -lpigpio -lrt -pthread -lm

# clean:
# 	rm *.o SatTTaC

# all: tt

