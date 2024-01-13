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

Operation operation = {
  .switch_active_thermal_control = false,
  .switch_attitude_control = false,
  .switch_imaging = false,
  .switch_imaging_mode = false,
  .switch_stand_by_mode = false,
};

// TelemetryData telemetryData = {
//   .length = sizeof(TelemetryData),
//   .data = {0xa1, 0xa1}
// };
SatPacket satPacket;
GSPacket gsPacket;

uint8_t rx_pointer = 0;

void preStatusProtocol(){

}

void updateStatusPacket(uint8_t index){
  satPacket.data.healthData.battery_voltage = 3.3;
  satPacket.data.healthData.battery_current = 50.0;
  satPacket.data.healthData.battery_charge = -10.0;
  satPacket.data.healthData.battery_temperature = 1000.8;
  satPacket.data.healthData.internal_temperature = index*3.3;
  satPacket.data.healthData.external_temperature = 3.3;
  satPacket.data.healthData.sd_memory_usage = 50.0;
  for(uint8_t i = 0; i < 10; i++){
    satPacket.data.healthData.rasp_data[i] = 0xa1;
  }
}

void postStatusProtocol(){

}

void preImagingDataProtocol(){
  
}

void updateImagingDataPacket(uint8_t index){
  for(uint8_t i = 0; i < 5; i++){
    satPacket.data.imagingData.lightnings[i].x = index*10 - i;
    satPacket.data.imagingData.lightnings[i].y = index*7 + i;
    satPacket.data.imagingData.lightnings[i].radius = index*20;
  }
}

void postImagingDataProtocol(){

}

void sendSatPacket(){
  Serial.print("Sending packet of length ");
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
      Serial.println("Timeout");
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
      Serial.println("Timeout");
      rx_pointer = 0;
      talking = false;
    }
  }
}

void onReceive(){
  Serial.print("Received packet with length ");
  Serial.println(gsPacket.length);
  switch(gsPacket.operation.protocol){
    case PROTOCOL_STATUS:
      Serial.println("Protocol: Status");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      switchCaseStatusProtocol();
      break;
    case PROTOCOL_IMAGING_DATA:
      Serial.println("Protocol: Imaging data");
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      switchCaseImagingDataProtocol();
      break;
    case PROTOCOL_SET_OPERATION:
      Serial.println("Protocol: Set operation");
      satPacket.operation.protocol = PROTOCOL_SET_OPERATION;
      switchCaseSetOperationProtocol();
      break;
  }
}

void switchCaseStatusProtocol(){
  switch(gsPacket.operation.operation){
    case GS_STATUS_REQUEST:
      Serial.println("Status: Request");
      preStatusProtocol();
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_PACKETS_AVAILABLE;
      satPacket.data.number_of_packets = 10;
      satPacket.length = 4;
      sendSatPacket();
      break;
    case GS_STATUS_START_TRANSMISSION:
      Serial.println("Status: Start transmission");
      for(unsigned int i = 0; i < 10; i++){
        updateStatusPacket(i);
        satPacket.operation.protocol = PROTOCOL_STATUS;
        satPacket.operation.operation = SATELLITE_STATUS_PACKET;
        satPacket.index = i;
        satPacket.length = 3+sizeof(HealthData);
        sendSatPacket();
      }
      Serial.println("Status: Packets done");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_PACKETS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      break;
    case GS_STATUS_RESEND_PACKET:
      Serial.println("Status: Resend packets");
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
        Serial.println("All packets transmitted");
      }
      Serial.println("Protocol done");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_PACKETS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      break;
    case GS_STATUS_DONE:
      Serial.println("Status: Done");
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
      Serial.println("Imaging data: Request");
      preImagingDataProtocol();
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_PACKETS_AVAILABLE;
      satPacket.data.number_of_packets = 10;
      satPacket.length = 4;
      sendSatPacket();
      break;
    case GS_IMAGING_START_TRANSMISSION:
      Serial.println("Imaging data: Start transmission");
      for(unsigned int i = 0; i < 10; i++){
        updateImagingDataPacket(i);
        satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
        satPacket.operation.operation = SATELLITE_IMAGING_PACKET;
        satPacket.index = i;
        satPacket.length = 3+sizeof(ImagingData);
        sendSatPacket();
      }
      Serial.println("Protocol done");
      satPacket.length = 2;
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_PACKETS_DONE;
      sendSatPacket();
      break;
    case GS_IMAGING_RESEND_STATUS:
      Serial.println("Imaging data: Resend status");
      if(!gsPacket.data.resend.isDone){
        Serial.println("Recalling packets");
        for(unsigned int i = 0; i < 32; i++){
          for(unsigned int j = 0; j < 8; j++){
            if(bitRead(gsPacket.data.resend.packets[i], j)){
              Serial.print("Resending packet ");
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
        Serial.println("All packets transmitted");
      }
      Serial.println("Protocol done");
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_PACKETS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      break;
    case GS_IMAGING_DONE:
      Serial.println("Protocol done");
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
      Serial.println("Set operation");
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
      Serial.println("Set operation: Done");
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