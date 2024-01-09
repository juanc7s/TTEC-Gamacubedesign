#include "Arduino.h"
#include "SatComm.h"

#include <EbyteLib.h>

uint8_t rxChan = 23;
uint8_t rxAddh = 0xa1;
uint8_t rxAddl = 0x06;
uint8_t txChan = 10;
uint8_t txAddh = 0x8f;
uint8_t txAddl = 0xf7;

// bool tx_received = false;
// unsigned int tx_pointer = 0;

// Greeting greeting;
// Resend resend;
// Protocol protocol;

// #define N 127
TelemetryData telemetryData = {
  .length = sizeof(TelemetryData),
  .data = {0xa1, 0xa1}
};
RxPacket rxPacket;
TxPacket txPacket;

void sendTelemetry(){
  Serial.print("Sending packet of length ");
  Serial.println(sizeof(rxPacket));
  // asynchronousWriteFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&telemetry, sizeof(telemetry));
  writeFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&rxPacket, sizeof(rxPacket));
  // printTransmissionResult(5000);
}

void updateRFComm(){
  if(packet_received){
    processPacket(onReceive);
  }
  // char c;
  // while(e32serial.available()){
  //   if(e32serial.overflow()){
  //     Serial.println("Serial OVERFLOW");
  //   }
  //   ((uint8_t*)&message)[comm_pointer++] = (uint8_t)e32serial.read();
  //   if(comm_pointer < 2){
      
  //   } else if(comm_pointer == 2){
  //     Serial.print("\n\nReceiving message of length ");
  //     Serial.println(message.length);
  //   } else if(comm_pointer==message.length){
  //     delay(200);
  //     Serial.print("Responding with a telemetry of length ");
  //     Serial.println(sizeof(telemetry));
  //     sendTelemetry();
  //     comm_pointer = 0;
  //     Serial.print("Message received!\n\n");
  //     Serial.print("Message type: ");Serial.println(message.type);
  //     Serial.print("Message: ");Serial.println(message.message);
  //     Serial.print("Actuator 1: ");Serial.println(message.actuator_1);
  //     Serial.print("Actuator 2: ");Serial.println(message.actuator_2);
  //     Serial.print("Actuator 3: ");Serial.println(message.actuator_3);
  //     Serial.print("Actuator 4: ");Serial.println(message.actuator_4);
  //   }
  // }
}

void onReceive(uint8_t* received_buffer, unsigned int size){
  Serial.print("Received packet with length ");
  Serial.println(size);
  // if(size < 2){
  //   return;
  // }
  for(unsigned int i = 0; i < size; i++){
    ((uint8_t*)(&txPacket))[i] = received_buffer[i];
    // Serial.println(received_buffer[i], HEX);
  }
  Serial.println(txPacket.operation);
  switch(txPacket.operation){
    case PROTOCOL_GREETING:
      Serial.println("Greeting");
      rxPacket.operation = SATELLITE_PACKETS_AVAILABLE;
      rxPacket.data.numberOfPackets = 1;
      sendTelemetry();
      break;
    case PROTOCOL_START_TELEMETRY_TRANSMISSION:
      // current_struct = &protocol;
      break;
    case PROTOCOL_RESEND_STATUS:
      // current_struct = &resend;
      break;
  }
}