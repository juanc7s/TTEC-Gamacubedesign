#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {
  Serial.begin(19200);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.print(counter);
  Serial.print(" ");

  // send packet
  LoRa.beginPacket();
  uint8_t msgLength = LoRa.print("Hello There!\nGeneral Kenobi! - ");
  msgLength += LoRa.print(counter);
  LoRa.endPacket();

  Serial.println(msgLength);

  counter++;

  delay(500);
}
