#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

#include "BitCubeSatProtocol.h"

BitCubeSatProtocol protocol;

void setup() {
  Serial.begin(19200);
  while (!Serial);

  protocol.begin(433E6);
}

void loop() {
  // try to parse packet
  TTC_main_loop();
}

void TTC_main_loop(){
  protocol.main_loop();
  if(protocol.isCommandReceived()){
    protocol.getCommand();
    parseCommand();

    
    Serial.print("Sending data packet: ");
    Serial.println(protocol.counter++);
  }
  protocol.sendTelemetry();
  delay(2000);
}


void parseCommand(){
  Serial.print("Command received.");
  Serial.print("\tCommand 1: ");
  Serial.println(protocol.command.cmd_1);
  Serial.print("\tCommand 2: ");
  Serial.println(protocol.command.cmd_2);
  Serial.print("\tCommand 3: ");
  Serial.println(protocol.command.cmd_3);
}