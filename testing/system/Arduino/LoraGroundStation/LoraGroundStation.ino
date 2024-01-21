#include <SPI.h>
#include <LoRa.h>

#include "BitCubeGSProtocol.h"

int counter = 0;
BitCubeGSProtocol protocol(915E6);

String command_data;
String telemetry_data = "NOP";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");
  if (!protocol.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  ttc_main_loop();

  delay(5000);
}

void ttc_main_loop(){
  Serial.print("Sending packet: ");
  Serial.println(counter);
  
  telemetry_data = protocol.transfer(command_data);
  Serial.print("Received response: ");
  Serial.println(telemetry_data);

  counter++;
}
