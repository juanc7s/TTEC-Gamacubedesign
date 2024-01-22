#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

#include "BitCubeSatProtocol.h"

BitCubeSatProtocol protocol;

long unsigned telemetry_timer = 500;
long unsigned next_telemetry_timer;

void setup() {
  Serial.begin(57600);
  while (!Serial);

  protocol.begin(433E6);
  next_telemetry_timer = millis() + telemetry_timer;
}

void loop() {
  // try to parse packet
  TTC_main_loop();
}

void TTC_main_loop(){
  if(millis() < next_telemetry_timer){
    // Serial.println("Not yet");
    return;
  }
  next_telemetry_timer += telemetry_timer;
  if(millis() > next_telemetry_timer){
    Serial.println("Telemetry lost!");
  }
  protocol.main_loop();
  if(protocol.isCommandReceived()){
    protocol.getCommand();
    parseCommand();

    
    Serial.print("Sending data packet: ");
    Serial.println(protocol.counter++);
  }
  protocol.sendTelemetry();
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