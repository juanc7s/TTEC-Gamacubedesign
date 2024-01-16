#include "Arduino.h"
#include "SatComm.h"

uint8_t rxChan = 23;
uint8_t rxAddh = 0xa1;
uint8_t rxAddl = 0x06;
uint8_t txChan = 10;
uint8_t txAddh = 0x8f;
uint8_t txAddl = 0xf7;

bool talking = false;
unsigned long int communication_timeout;
const unsigned long int communication_timeout_limit = 1000;
bool telemetry_received = false;
uint8_t telemetry_state = 0;

uint8_t number_of_packets = 0;
const uint8_t N_status = 10;
const uint8_t N_imaging = 20;

Operation operation = {
  .switch_active_thermal_control = false,
  .switch_attitude_control = false,
  .switch_imaging = false,
  .switch_imaging_mode = false,
  .switch_stand_by_mode = false,
};

SatPacket satPacket;
GSPacket gsPacket;

uint8_t rx_pointer = 0;

void preStatusProtocol(){
  number_of_packets = reading_status_counter;
  if(number_of_packets > N_status){
    number_of_packets = N_status;
  }
}

void updateStatusPacket(uint8_t index){
  // for(unsigned int i = 0; i < sizeof(HealthData); i++){
  //   ((uint8_t*)(&satPacket.data.healthData))[i] = ((uint8_t*)(&(dataBuffer.statusData[index])))[i];
  // }
  sdReadSatStatusPacket(&satPacket.data.healthData, index);
}

void postStatusProtocol(){
  reading_status_file_pointer += sizeof(HealthData)*number_of_packets;
  reading_status_counter -= number_of_packets;
}

void preImagingDataProtocol(){
  number_of_packets = reading_status_counter;
  if(number_of_packets > N_imaging){
    number_of_packets = N_imaging;
  }
  // for(unsigned int i = 0; i < n; i++){
  //   sdReadSatStatusPacket(&(dataBuffer.statusData[i]));
  // }
}

void updateImagingDataPacket(uint8_t index){
  
}

void postImagingDataProtocol(){
  reading_imaging_file_pointer += sizeof(ImagingData)*number_of_packets;
  reading_imaging_counter -= number_of_packets;
}

void sendSatPacket(){
  Serial.print("S:");
  Serial.print(satPacket.length);Serial.print("    ");
  Serial.print(satPacket.operation.protocol);Serial.print("    ");
  Serial.println(satPacket.operation.operation);
  // asynchronousWriteFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&telemetry, sizeof(telemetry));
  writeFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&satPacket, satPacket.length);
}

void updateRFComm(){
  uint8_t b;
  if(e32serial.available()){
    if(millis() > communication_timeout){
      Serial.println("T:1");
      rx_pointer = 0;
      talking = false;
    }
    while(e32serial.available()){
      b = e32serial.read();
      Serial.println(b);
      ((uint8_t*)(&gsPacket))[rx_pointer++] = b;
      if(rx_pointer>0 && rx_pointer==gsPacket.length){
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
      Serial.println("T:2");
      rx_pointer = 0;
      talking = false;
    }
  }
}

void onReceive(){
  Serial.print("P:R:");
  Serial.println(gsPacket.length);
  switch(gsPacket.operation.protocol){
    case PROTOCOL_STATUS:
      Serial.println("P:S");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      switchCaseStatusProtocol();
      break;
    case PROTOCOL_IMAGING_DATA:
      Serial.println("P:I");
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      switchCaseImagingDataProtocol();
      break;
    case PROTOCOL_SET_OPERATION:
      Serial.println("P:O");
      satPacket.operation.protocol = PROTOCOL_SET_OPERATION;
      switchCaseSetOperationProtocol();
      break;
  }
}

void switchCaseStatusProtocol(){
  switch(gsPacket.operation.operation){
    case GS_STATUS_REQUEST:
      Serial.println("S:1");
      preStatusProtocol();
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_PACKETS_AVAILABLE;
      satPacket.data.number_of_packets = number_of_packets;
      satPacket.length = 4;
      sendSatPacket();
      break;
    case GS_STATUS_START_TRANSMISSION:
      Serial.println("S:2");
      for(unsigned int i = 0; i < number_of_packets; i++){
        updateStatusPacket(i);
        satPacket.operation.protocol = PROTOCOL_STATUS;
        satPacket.operation.operation = SATELLITE_STATUS_PACKET;
        satPacket.index = i;
        satPacket.length = 3+sizeof(HealthData);
        sendSatPacket();
      }
      Serial.println("S:3");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_PACKETS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      break;
    case GS_STATUS_RESEND_PACKET:
      Serial.println("S:4");
      if(!gsPacket.data.resend.isDone){
        Serial.println("Recalling packets");
        for(unsigned int i = 0; i < 32; i++){
          for(unsigned int j = 0; j < 8; j++){
            if(bitRead(gsPacket.data.resend.packets[i], j)){
              Serial.print("Resending packet ");
              Serial.println(i*8+j);
              updateImagingDataPacket(i*8+j);
              satPacket.operation.protocol = PROTOCOL_STATUS;
              satPacket.operation.operation = SATELLITE_STATUS_PACKET;
              satPacket.index = i*8 + j;
              satPacket.length = 3+sizeof(HealthData);
              sendSatPacket();
            }
          }
        }
      } else{
        Serial.println("S:A");
      }
      Serial.println("PD");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_PACKETS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      break;
    case GS_STATUS_DONE:
      Serial.println("S:D");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      postStatusProtocol();
      rx_pointer = 0;
      talking = false;
      break;
  }
}

void switchCaseImagingDataProtocol(){
  switch(gsPacket.operation.operation){
    case GS_IMAGING_REQUEST:
      Serial.println("I:1");
      preImagingDataProtocol();
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_PACKETS_AVAILABLE;
      satPacket.data.number_of_packets = number_of_packets;
      satPacket.length = 4;
      sendSatPacket();
      break;
    case GS_IMAGING_START_TRANSMISSION:
      Serial.println("I:2");
      for(unsigned int i = 0; i < number_of_packets; i++){
        updateImagingDataPacket(i);
        satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
        satPacket.operation.operation = SATELLITE_IMAGING_PACKET;
        satPacket.index = i;
        satPacket.length = 3+sizeof(ImagingData);
        sendSatPacket();
      }
      Serial.println("I:D");
      satPacket.length = 2;
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_PACKETS_DONE;
      sendSatPacket();
      break;
    case GS_IMAGING_RESEND_STATUS:
      Serial.println("I:3");
      if(!gsPacket.data.resend.isDone){
        Serial.println("I:R");
        for(unsigned int i = 0; i < 32; i++){
          for(unsigned int j = 0; j < 8; j++){
            if(bitRead(gsPacket.data.resend.packets[i], j)){
              Serial.print("I:R:");
              Serial.println(i*8+j);
              updateImagingDataPacket(i*8+j);
              satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
              satPacket.operation.operation = SATELLITE_IMAGING_PACKET;
              satPacket.index = i*8 + j;
              satPacket.length = 3+sizeof(ImagingData);
              sendSatPacket();
            }
          }
        }
      } else{
        Serial.println("I:A");
      }
      Serial.println("I:D");
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_PACKETS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      break;
    case GS_IMAGING_DONE:
      Serial.println("I:4");
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_DONE;
      satPacket.length = 2;
      sendSatPacket();
      postImagingDataProtocol();
      rx_pointer = 0;
      talking = false;
      // sendDone();
  }
}

void switchCaseSetOperationProtocol(){
  switch(gsPacket.operation.operation){
    case GS_SET_OPERATION:
      Serial.println("O:1");
      Serial.print("Active thermal control: ");Serial.println(gsPacket.data.operation.switch_active_thermal_control ? "ON" : "OFF");
      Serial.print("Attitude control: ");Serial.println(gsPacket.data.operation.switch_attitude_control ? "ON" : "OFF");
      Serial.print("Imaging: ");Serial.println(gsPacket.data.operation.switch_imaging ? "ON" : "OFF");
      Serial.print("Imaging mode: ");Serial.println(gsPacket.data.operation.switch_imaging_mode ? "Mode 2" : "Mode 1");
      Serial.print("Stand by: ");Serial.println(gsPacket.data.operation.switch_stand_by_mode ? "ON" : "OFF");
      satPacket.operation.protocol = PROTOCOL_SET_OPERATION;
      satPacket.operation.operation = SATELLITE_SET_OPERATION_ECHO;
      satPacket.data.operation_echo.switch_active_thermal_control = gsPacket.data.operation.switch_active_thermal_control;
      satPacket.data.operation_echo.switch_attitude_control = gsPacket.data.operation.switch_attitude_control;
      satPacket.data.operation_echo.switch_imaging = gsPacket.data.operation.switch_imaging;
      satPacket.data.operation_echo.switch_imaging_mode = gsPacket.data.operation.switch_imaging_mode;
      satPacket.data.operation_echo.switch_stand_by_mode = gsPacket.data.operation.switch_stand_by_mode;
      satPacket.length = 3+sizeof(GSOperation);
      sendSatPacket();
      break;
    case GS_SET_OPERATION_DONE:
      Serial.println("O:D");
      *((uint8_t*)&operation) = satPacket.data.byte;
      satPacket.operation.protocol = PROTOCOL_SET_OPERATION;
      satPacket.operation.operation = SATELLITE_SET_OPERATION_DONE;
      satPacket.length = 2;
      sendSatPacket();
      rx_pointer = 0;
      talking = false;
      break;
  }
}