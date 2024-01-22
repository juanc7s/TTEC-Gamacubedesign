Sim#include "BitCubeGSProtocol.h"
#include "Arduino.h"

BitCubeGSProtocol::BitCubeGSProtocol(){
  command = "Hello There!";
}

boolean begin(long unsigned int _frequency){
  frequency = _frequency;
  return LoRa.begin(frequency)
}

String BitCubeGSProtocol::transfer(String sending_data){
  // send packet
  send_command(sending_data);

  // Wait for sat processing
  delay(50);
  
  // Receive response
  return protocol.receive_data();
}

void BitCubeGSProtocol::send_command(String sending_data){
  LoRa.beginPacket();
  LoRa.print("hello");
//  LoRa.print(counter);
//  LoRa.print(command);
  LoRa.endPacket();
}

String BitCubeGSProtocol::receive_data(){
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    data_length = LoRa.available();
    int k = 0;
    char c;
    while (LoRa.available()) {
      c = (char)LoRa.read();
      Serial.print(c);
      data[k++] = c;
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

  return receiving_data;
}
