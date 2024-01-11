#include "Arduino.h"
#include "GSComm.h"

uint8_t txChan = 23;
uint8_t txAddh = 0xa1;
uint8_t txAddl = 0x06;
uint8_t rxChan = 10;
uint8_t rxAddh = 0x8f;
uint8_t rxAddl = 0xf7;

bool talking = false;
unsigned long int communication_timeout;
const unsigned long int communication_timeout_limit = 1000;
bool telemetry_received = false;
uint8_t telemetry_state = 0;

bool switch_active_thermal_control = false;
bool switch_attitude_control = false;
bool switch_imaging = false;
bool switch_imaging_mode = false;
bool switch_stand_by_mode = false;

uint8_t rx_pointer = 0;
  
// TelemetryData telemetryData;
SatPacket satPacket;
GSPacket gsPacket;

void sendGSPacket(){
  Serial.print("Sending a message of length ");
  Serial.println(gsPacket.length);
  if(getTransmissionMode()==FIXED_TRANSMISSION_MODE){
    writeFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&gsPacket, gsPacket.length);
  } else{
    write((uint8_t*)&gsPacket, sizeof(gsPacket));
  }
  communication_timeout = millis() + communication_timeout_limit;
}

// void sendCommand(){
//   gsPacket.length = 2;
//   gsPacket.operation.protocol = GS_GREETING;
//   unsigned long int t0 = millis();
//   sendGSPacket();
//   if(!getTransmissionResult(500)){
//     Serial.println("Transmission failed.\nSkipping to next iteration.");
//   } else{
//     Serial.print("Communication took ");
//     Serial.println(millis() - t0);
    // Serial.println("Waiting for response");
    // if(listenForResponse(1000)){
    //   gsPacket.length = 1;
    //   gsPacket.operation = PROTOCOL_START_TELEMETRY_TRANSMISSION;
    //   t0 = millis();
    //   sendGSPacket();
    //   if(!getTransmissionResult(500)){
    //     Serial.println("Transmission failed.\nSkipping to next iteration.");
    //   } else{
    //     Serial.print("Communication took ");
    //     Serial.println(millis() - t0);
    //     Serial.println("Waiting for response");
    //     if(listenForResponse(1000)){
    //       Serial.println(satPacket.data.numberOfPackets);
    //       Serial.println(satPacket.data.telemetryPacket.index);
    //     } else{
    //       Serial.print("No response");
    //     }
    //   }
    // } else{
    //   Serial.println("No response");
    // }
//   }
// }

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
  uint8_t b;
  if(e32serial.available()){
    while(e32serial.available()){
      b = e32serial.read();
      Serial.println(b);
      ((uint8_t*)(&satPacket))[rx_pointer++] = b;
      if(rx_pointer>0 && rx_pointer==satPacket.length){
        telemetry_received = true;
        onReceive();
        rx_pointer = 0;
      }
      if(rx_pointer >= sizeof(satPacket)){
        rx_pointer = 0;
      }
    }
    communication_timeout = millis() + communication_timeout_limit;
  }
  if(talking){
    if(millis() > communication_timeout){
      Serial.println("Timeout");
      talking = false;
    }
  }
}

void startQueryStatusProtocol(){
  if(talking){
    return;
  }
  talking = true;
  sendGSPacket();
}

void startQueryImagingDataProtocol(){
  if(talking){
    return;
  }
  talking = true;
  sendGSPacket();
}

void startCommandProtocol(){
  if(talking){
    return;
  }
  gsPacket.operation.protocol = SET_OPERATION;
  gsPacket.operation.operation = GS_SEND_OPERATION;
  gsPacket.data.command.switch_active_thermal_control = switch_active_thermal_control;
  gsPacket.data.command.switch_attitude_control = switch_attitude_control;
  gsPacket.data.command.switch_imaging = switch_imaging;
  gsPacket.data.command.switch_imaging_mode = switch_imaging_mode;
  gsPacket.data.command.switch_stand_by_mode = switch_stand_by_mode;

  gsPacket.length = 3;

  talking = true;
  sendGSPacket();
}

void onReceive(){
  Serial.print("\n\nReceiving telemetry of length ");
  Serial.println(satPacket.length);
  Serial.print("Telemetry received!\n\n");
  // Serial.print("Packet index: ");Serial.println(telemetry.index);
  // Serial.print("Packet Data: ");Serial.write(telemetry.data, telemetry.length-2);
  Serial.println("");
  // Serial.print("Instrument 1: ");Serial.println(telemetry.instrument_1);
  // Serial.print("Instrument 2: ");Serial.println(telemetry.instrument_2);
  // Serial.print("Instrument 3: ");Serial.println(telemetry.instrument_3);
  // Serial.print("Instrument 4: ");Serial.println(telemetry.instrument_4);
  switch(satPacket.operation.protocol){
    case QUERY_STATUS:
      switchStatus();
      break;
    case QUERY_IMAGING_DATA:
      switchImagingData();
      break;
    case SET_OPERATION:
      switchSetOperation();
      break;
  }
}

void switchStatus(){
  switch(satPacket.operation.operation){
    case SATELLITE_SEND_STATUS:
      break;
    case SATELLITE_QUERY_STATUS_DONE:
      talking = false;
      break;
  }
}

void switchImagingData(){
  unsigned int k = 0;
  switch(satPacket.operation.operation){
    case SATELLITE_PACKETS_AVAILABLE:
      Serial.print("Number of packets available: ");
      Serial.println(satPacket.data.number_of_packets);
      for(unsigned int i = 0; i < 32; i++){
        for(unsigned int j = 0; j < 8; j++){
          if(k < satPacket.data.number_of_packets){
            bitSet(gsPacket.data.resend.packets[i],j);
          } else{
            bitClear(gsPacket.data.resend.packets[i],j);
          }
          k++;
        }
      }
      gsPacket.operation.operation = GS_START_TELEMETRY_TRANSMISSION;
      gsPacket.length = 2;
      sendGSPacket();
      break;
    case SATELLITE_SEND_PACKETS:
      Serial.print("Received packet ");
      Serial.println(satPacket.data.index);
      bitClear(gsPacket.data.resend.packets[satPacket.data.index>>3],satPacket.data.index&0x07);
      break;
    case SATELLITE_QUERY_IMAGING_DONE:
      Serial.println("Satellite done");
      gsPacket.length = 35;
      gsPacket.operation.operation = GS_RESEND_STATUS;
      gsPacket.data.resend.isDone = true;
      for(unsigned int i = 0; i < 32; i++){
        if(gsPacket.data.resend.packets[i]!=0){
          gsPacket.data.resend.isDone = false;
          break;
        }
      }
      if(gsPacket.data.resend.isDone){
        gsPacket.length = 2;
        gsPacket.operation.operation = GS_QUERY_IMAGING_DONE;
        talking = false;
      }
      sendGSPacket();
      break;
  }
}

void switchSetOperation(){
  switch(satPacket.operation.operation){
    case SATELLITE_REPEAT_OPERATION:
      break;
    case SATELLITE_SET_OPERATION_DONE:
      talking = false;
      break;
  }
}