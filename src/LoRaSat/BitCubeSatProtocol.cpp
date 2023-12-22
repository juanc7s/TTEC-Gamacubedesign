#include "BitCubeSatProtocol.h"
#include "Arduino.h"

BitCubeSatProtocol::BitCubeSatProtocol(long int _frequency){
  frequency = _frequency;
  data = "Hello There!";
}

void begin(){
  Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void BitCubeSatProtocol::main_loop(){
  int package_size = LoRa.parsePackage();
  if(package_size){
    // received a packet
    Serial.print("Received packet '");

    // read packet
    int command_length = LoRa.available();
    int k = 0;
    char c;
    while (LoRa.available()) {
      c = (char)LoRa.read();
      Serial.print(c);
      command[k++] = c;
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

void BitCubeSatProtocol::set_data(String_data){
  data = _data;
}

String BitCubeSatProtocol::receive_command(String _command){
  command = _command
}

boolean BitCubeSatProtocol::is_message_received(){
  return message_received;
}
