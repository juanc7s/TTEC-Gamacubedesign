#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

#include "BitCubeSatProtocol.h"

struct Telemetry {
    char type[5];
    char message_1[8];
    float temperature_1;
    float temperature_2;
    char message_2[8];
    float battery_voltage;
    float battery_charge;
} telemetry;

int counter = 0;
BitCubeSatProtocol protocol(915E6);

String command;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  protocol.begin();
}

void loop() {
  // try to parse packet
  protocol.main_loop();
  if(protocol.is_message_received()){
    command = protocol.read_command();
    parse_command(command);
    Serial.print("Command received: ");
    Serial.println(command);
    protocol.send_data(data);
    counter++;
    Serial.print("Sending data: ");
    Serial.println(data);
  }
}

void parse_command(String command){
  
}
