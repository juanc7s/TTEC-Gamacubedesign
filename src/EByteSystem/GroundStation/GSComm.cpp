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

// uint8_t telemetry_state = 0;
  
TelemetryData telemetryData;
RxPacket rxPacket;
TxPacket txPacket;

void sendCommand(){
  // telemetry_index = 0;
  Serial.print("Sending a message of length ");
  Serial.println(sizeof(txPacket));
  txPacket.operation = PROTOCOL_GREETING;
  if(getTransmissionMode()==FIXED_TRANSMISSION_MODE){
    asynchronousWriteFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&txPacket, sizeof(txPacket));
  } else{
    asynchronousWrite((uint8_t*)&txPacket, sizeof(txPacket));
  }
  if(!getTransmissionResult(500)){
    Serial.println("Transmission failed.\nSkipping to next iteration.");
  } else{
    // delay(3000);
    // Serial.println("Waiting for response");
    // if(listenForResponse(1000)){
    //   ;
    // }
  }
}

bool listenForResponse(unsigned long int timeout){

  unsigned long int to = millis() + timeout;
  while(millis() < to){
    // updateRFComm();
    if(telemetry_received){
      telemetry_received = false;
      return true;
    }
  }
  Serial.println("No response");
  return false;
}

void updateRFComm(uint8_t* received_buffer, unsigned int size){
  // char c;
  telemetry_received = true;
  for(unsigned int i = 0; i < size; i++){
    // if(e32serial.overflow()){
    //   Serial.println("Serial OVERFLOW");
    // }
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