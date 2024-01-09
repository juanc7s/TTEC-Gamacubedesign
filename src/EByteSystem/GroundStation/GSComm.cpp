#include "Arduino.h"
#include "GSComm.h"

uint8_t txChan = 23;
uint8_t txAddh = 0xa1;
uint8_t txAddl = 0x06;
uint8_t rxChan = 10;
uint8_t rxAddh = 0x8f;
uint8_t rxAddl = 0xf7;

// unsigned int telemetry_index = 0;
bool telemetry_received = false;

uint8_t telemetry_state = 0;
  
TelemetryData telemetryData;
RxPacket rxPacket;
TxPacket txPacket;

void sendTxPacket(){
  // telemetry_index = 0;
  Serial.print("Sending a message of length ");
  Serial.println(sizeof(txPacket));
  if(getTransmissionMode()==FIXED_TRANSMISSION_MODE){
    writeFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&txPacket, sizeof(txPacket));
  } else{
    write((uint8_t*)&txPacket, sizeof(txPacket));
  }
}

void sendCommand(){
  txPacket.length = 1;
  txPacket.operation = PROTOCOL_GREETING;
  unsigned long int t0 = millis();
  sendTxPacket();
  if(!getTransmissionResult(500)){
    Serial.println("Transmission failed.\nSkipping to next iteration.");
  } else{
    Serial.print("Communication took ");
    Serial.println(millis() - t0);
    Serial.println("Waiting for response");
    if(listenForResponse(1000)){
      txPacket.length = 1;
      txPacket.operation = PROTOCOL_START_TELEMETRY_TRANSMISSION;
      t0 = millis();
      sendTxPacket();
      if(!getTransmissionResult(500)){
        Serial.println("Transmission failed.\nSkipping to next iteration.");
      } else{
        Serial.print("Communication took ");
        Serial.println(millis() - t0);
        Serial.println("Waiting for response");
        if(listenForResponse(1000)){
          Serial.println(rxPacket.data.numberOfPackets);
          Serial.println(rxPacket.data.telemetryPacket.index);
        } else{
          Serial.print("No response");
        }
      }
    } else{
      Serial.println("No response");
    }
  }
}

bool listenForResponse(unsigned long int timeout){
  unsigned long int to = millis() + timeout;
  while(millis() < to){
    updateRFComm();
    if(telemetry_received){
      telemetry_received = false;
      return true;
    }
  }
  return false;
}

void updateRFComm(){
  if(packet_received){
    Serial.println("Processing");
    processPacket(onReceive);
  }
}

void onReceive(uint8_t* received_buffer, unsigned int size){
  telemetry_received = true;
  for(unsigned int i = 0; i < size; i++){
    ((uint8_t*)&rxPacket)[i] = received_buffer[i];
    if(i == 2){
      Serial.print("\n\nReceiving telemetry of length ");
      Serial.println(rxPacket.length);
    }
  }
  Serial.print("Telemetry received!\n\n");
  // Serial.print("Packet index: ");Serial.println(telemetry.index);
  // Serial.print("Packet Data: ");Serial.write(telemetry.data, telemetry.length-2);
  Serial.println("");
  // Serial.print("Instrument 1: ");Serial.println(telemetry.instrument_1);
  // Serial.print("Instrument 2: ");Serial.println(telemetry.instrument_2);
  // Serial.print("Instrument 3: ");Serial.println(telemetry.instrument_3);
  // Serial.print("Instrument 4: ");Serial.println(telemetry.instrument_4);
  switch(rxPacket.operation){
    case SATELLITE_PACKETS_AVAILABLE:
      Serial.print("Number of packets available: ");
      Serial.println(rxPacket.data.numberOfPackets);
      break;
  }
}