LoRa.o: ./libs/sx1278-LoRa-RaspberryPi/LoRa.c
	gcc -c ./libs/sx1278-LoRa-RaspberryPi/LoRa.c -o ./libs/sx1278-LoRa-RaspberryPi/LoRa.o -lpigpio -lrt -pthread -lm

tt.o: tt.c
	gcc -c tt.c -o tt.o -lpigpio -lrt -pthread -lm

tt: ./libs/sx1278-LoRa-RaspberryPi/LoRa.o tt.o
	gcc -o tt tt.o ./libs/sx1278-LoRa-RaspberryPi/LoRa.o -lpigpio -lrt -pthread -lm

clean:
	rm *.o tt
