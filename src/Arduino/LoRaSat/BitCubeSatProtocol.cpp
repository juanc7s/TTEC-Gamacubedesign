#include "BitCubeSatProtocol.h"
#include "Arduino.h"

// BitCubeSatProtocol::BitCubeSatProtocol(){
//   this->frequency = frequency;
// }

void BitCubeSatProtocol::begin(long int frequency){
  this->frequency = frequency;
  Serial.println("LoRa Receiver");

  if (!LoRa.begin(this->frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.crc();
  LoRa.setSPIFrequency(8e6);

  // this->receiving_command.reserve(sizeof(Command));
  // this->received_command.reserve(sizeof(Command));
  this->sending_telemetry.reserve(255); // Maximum LoRa package size
  // telemetry.message_1;
  // telemetry.message_2;
}

void BitCubeSatProtocol::main_loop(){
  int package_size = LoRa.parsePacket();
  if(package_size){
    // received a packet
    Serial.print("Received packet '");

    // read packet
    int package_length = LoRa.available();
    int k = 0;
    char c;
    while (LoRa.available()) {
      c = (char)LoRa.read();
      Serial.print(c);
      receiving_command[k++] = c;
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

void BitCubeSatProtocol::getCommand(){
  StaticJsonDocument<sizeof(Command)> command_doc;
  // deserializeJson(this->command_doc, this->received_command);
  command.cmd_1 = command_doc["1"];
  command.cmd_2 = command_doc["2"];
  command.cmd_3 = command_doc["3"];
}

long int t1;
void tic(){
  t1 = millis();
}

void toc(){
  Serial.print("toc: ");Serial.println(millis() - t1);
}

void BitCubeSatProtocol::sendTelemetry(){
    tic();
  for(int i = 0; i < 2; i++){
    sending_telemetry = "";
    prepareTelemetryToLoRa();
    // prepareTelemetryToLoRa();
    while(!LoRa.beginPacket()); // Wait until LoRa is ready to transmit
    LoRa.print(sending_telemetry);
    LoRa.endPacket(true); // true: Asyncronous transmission, must check if LoRa is ready before begin packet again
  }
    toc();
}

void BitCubeSatProtocol::prepareTelemetryToLoRa(){
  long int t1 = millis();
  telemetry.index++;
  telemetry.time = millis();
  telemetry_doc["1"] = telemetry.type;
  telemetry_doc["2"] = telemetry.message_1;
  telemetry_doc["3"] = telemetry.main_temperature;
  telemetry_doc["4"] = telemetry.message_2;
  telemetry_doc["5"] = telemetry.batt_voltage;
  telemetry_doc["6"] = telemetry.batt_current;
  telemetry_doc["7"] = telemetry.batt_charge;
  telemetry_doc["8"] = telemetry.batt_temperature;
  telemetry_doc["9"] = telemetry.index;
  telemetry_doc["10"] = telemetry.time;
  serializeJson(telemetry_doc, sending_telemetry);
  sending_telemetry += "\n";
  // Serial.println(telemetry_doc.memoryUsage());
  // Serial.println(sizeof(Telemetry));
  // Serial.println(JSON_OBJECT_SIZE(sizeof(Telemetry)));
  // Serial.println(JSON_OBJECT_SIZE(9) + 56);
  // Serial.println("");
  // Serial.println("");
  // Serial.println(sending_telemetry.length());// Serial.print(" ");
  // Serial.println(telemetry.index);
}

bool BitCubeSatProtocol::isCommandReceived(){
  return message_received;
}
