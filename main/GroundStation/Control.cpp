#include "Arduino.h"
#include "Control.h"

const char PRINT_STR[] = "PRINT:";
const char CONTROL_STR[] = "CONTROL:";

bool serialFlag = false;

unsigned int message_data_index = 0;

bool state_request_status = false;
uint8_t send_request_status = 0;
bool state_request_imaging = false;
uint8_t send_request_imaging = 0;
bool state_command = false;
uint8_t send_command = 0;

void (*parsing_function)(uint8_t c) = parseSerial;

void checkControl(){
  uint8_t c;
  while(Serial.available()){
    c = Serial.read();
    if(serialFlag){
      parsing_function(c);
      serialFlag = false;
    } else if(c==13){
      serialFlag = true;
    } else if(c=='\n'){
      parsing_function = parseSerial;
    } else if (c==255){
      parsing_function = parseSerial;
    } else{
      parsing_function(c);
    }
  }
}

unsigned long number_parser_N = 0;
void (*number_parser_callback)();
void number_parser(uint8_t c){
  if(c==':'){
    number_parser_callback();
  } else{
    number_parser_N *= 10;
    number_parser_N += c-48;
  }
}

void parseSerial(uint8_t c){
  switch(c){
    case READ_RX_ADDH:
      Serial.print(CONTROL_STR);Serial.print("RX_ADDH:");
      Serial.println(rxAddh, HEX);
      break;
    case READ_RX_ADDL:
      Serial.print(CONTROL_STR);Serial.print("RX_ADDL:");
      Serial.println(rxAddl, HEX);
      break;
    case READ_TX_ADDH:
      Serial.print(CONTROL_STR);Serial.print("TX_ADDH:");
      Serial.println(txAddh, HEX);
      break;
    case READ_TX_ADDL:
      Serial.print(CONTROL_STR);Serial.print("TX_ADDL:");
      Serial.println(txAddl, HEX);
      break;
    case READ_BANDWIDTH:
      Serial.print(CONTROL_STR);Serial.print("BANDWIDTH:");
      Serial.println(bandwidth);
      // Serial.print(" Hz");
      break;
    case READ_FREQUENCY:
      Serial.print(CONTROL_STR);Serial.print("FREQUENCY:");
      Serial.println(frequency);
      // Serial.println(" MHz");
      break;
    case READ_SPI_FREQUENCY:
      Serial.print(CONTROL_STR);Serial.print("SPI_FREQUENCY:");
      Serial.println(spi_frequency);
      // Serial.println(" Hz");
      break;
    case READ_SPREADING_FACTOR:
      Serial.print(CONTROL_STR);Serial.print("SPREADING_FACTOR:");
      Serial.println(spreading_factor);
      break;
    case READ_TRANSMISSION_POWER:
      Serial.print(CONTROL_STR);Serial.print("TXPW:");
      Serial.println(tx_power);
      // Serial.println(" dBm");
      break;
    case READ_ALL:
      // printConfiguration();
      break;
    
    case SET_RX_ADDH:
      parsing_function = control_setRxAddh;
      break;
    case SET_RX_ADDL:
      parsing_function = control_setRxAddl;
      break;
    case SET_TX_ADDH:
      parsing_function = control_setTxAddh;
      break;
    case SET_TX_ADDL:
      parsing_function = control_setTxAddl;
      break;
    case SET_BANDWIDTH:
      parsing_function = control_setBandwidth;
      break;
    case SET_FREQUENCY:
      parsing_function = number_parser;
      number_parser_callback = control_setFrequency;
      break;
    case SET_SPI_FREQUENCY:
      parsing_function = number_parser;
      number_parser_callback = control_setSPIFrequency;
      break;
    case SET_SPREADING_FACTOR:
      parsing_function = control_setSpreadingFactor;
      break;
    case SET_TRANSMISSION_POWER:
      parsing_function = control_setTxPower;
      break;
    
    case FLUSH:
      // telemetry_index = 0;
      break;
    case SET_MESSAGE_DATA:
      break;
    
    case TOGGLE_QUERY_STATUS:
      parsing_function = control_toggleQueryStatus;
      break;
    case SEND_QUERY_STATUS:
      send_request_status++;
      break;
    case TOGGLE_QUERY_IMAGING:
      parsing_function = control_toggleQueryImaging;
      break;
    case SEND_QUERY_IMAGING:
      send_request_imaging++;
      break;
    case TOGGLE_COMMAND:
      parsing_function = control_toggleCommand;
      break;
    case SEND_COMMAND:
      send_command++;
      break;
    
    case SET_ACTIVE_THERMAL_CONTROL:
      parsing_function = setActiveThermalControl;
      break;
    case SET_ATTITUDE_CONTROL:
      parsing_function = setAttitudeControl;
      break;
    case SET_IMAGING:
      parsing_function = setImaging;
      break;
    case SET_IMAGING_MODE:
      parsing_function = setImagingMode;
      break;
    case SET_STAND_BY_MODE:
      parsing_function = setStandByMode;
      break;
  }
}

void control_setRxAddh(uint8_t c){
  rxAddh = c;
  parsing_function = parseSerial;
}

void control_setRxAddl(uint8_t c){
  rxAddl = c;
  parsing_function = parseSerial;
}

void control_setTxAddh(uint8_t c){
  txAddh = c;
  parsing_function = parseSerial;
}

void control_setTxAddl(uint8_t c){
  txAddl = c;
  parsing_function = parseSerial;
}

void control_setBandwidth(uint8_t c){
  bandwidth = c;
  long sbw = 125E3;
  switch(bandwidth){
    case 0:
      sbw = 7.8E3;
      break;
    case 1:
      sbw = 10.4E3;
      break;
    case 2:
      sbw = 15.6E3;
      break;
    case 3:
      sbw = 20.8E3;
      break;
    case 4:
      sbw = 31.25E3;
      break;
    case 5:
      sbw = 41.7E3;
      break;
    case 6:
      sbw = 62.5E3;
      break;
    case 7:
      sbw = 125E3;
      break;
    case 8:
      sbw = 250E3;
      break;
    case 9:
      sbw = 500E3;
      break;
  }
  LoRa.setSignalBandwidth(sbw);
}

void control_setFrequency(){
  frequency = number_parser_N;
  LoRa.setFrequency(frequency);
  parsing_function = parseSerial;
}

void control_setSPIFrequency(){
  LoRa.end();
  spi_frequency = number_parser_N;
  LoRa.setSPIFrequency(spi_frequency);
  if (!LoRa.begin(frequency)) {             // initialize ratio at set frequency
    Serial.print(PRINT_STR);
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  parsing_function = parseSerial;
}

void control_setSpreadingFactor(uint8_t c){
  spreading_factor = c;
  LoRa.setSpreadingFactor(spreading_factor);
  parsing_function = parseSerial;
}

void control_setTxPower(uint8_t c){
  tx_power = c;
  LoRa.setTxPower(tx_power);
  parsing_function = parseSerial;
}

void control_toggleQueryStatus(uint8_t c){
  state_request_status = c != 0;
  parsing_function = parseSerial;
}

void control_toggleQueryImaging(uint8_t c){
  state_request_imaging = c != 0;
  parsing_function = parseSerial;
}

void control_toggleCommand(uint8_t c){
  state_command = c != 0;
  parsing_function = parseSerial;
}

void setActiveThermalControl(uint8_t c){
  operation.switch_active_thermal_control = c==1;
}

void setAttitudeControl(uint8_t c){
  operation.switch_attitude_control = c==1;
}

void setImaging(uint8_t c){
  operation.switch_imaging = c==1;
}

void setImagingMode(uint8_t c){
  operation.switch_imaging_mode = c==1;
}

void setStandByMode(uint8_t c){
  operation.switch_stand_by_mode = c==1;
}

void control_print_status_packet(){
  Serial.print(PRINT_STR);
  Serial.print("Status: Received packet");
  Serial.println(satPacket.byte_data.index);
  Serial.print("STATUS_PACKET:");
  Serial.print(":Reading time:");Serial.print(satPacket.data.healthData.time);
  Serial.print(":Battery voltage:");Serial.print(satPacket.data.healthData.battery_voltage);
  Serial.print(":Battery current:");Serial.print(satPacket.data.healthData.battery_current);
  Serial.print(":Battery charge:");Serial.print(satPacket.data.healthData.battery_charge);
  Serial.print(":Battery temperature:");Serial.print(satPacket.data.healthData.battery_temperature);
  Serial.print(":Internal temperature:");Serial.print(satPacket.data.healthData.internal_temperature);
  Serial.print(":External temperature:");Serial.print(satPacket.data.healthData.external_temperature);
  Serial.print(":SD memory usage:");Serial.println(satPacket.data.healthData.sd_memory_usage);
  // for(uint8_t i = 0; i < 10; i++){
  //   Serial.println(newPacket.data.healthData.rasp_data[i]);
  // }
}

void control_print_imaging_packet(){
  Serial.print(PRINT_STR);
  Serial.print("Imaging: Received packet ");
  Serial.println(satPacket.byte_data.index);
  Serial.print("IMAGING_PACKET:");
  Serial.print("Index:");Serial.print(satPacket.data.imagingData.lightnings[0].index);
  Serial.print("Duration:");Serial.print(satPacket.data.imagingData.lightnings[0].duration);
  Serial.print("Radius:");Serial.print(satPacket.data.imagingData.lightnings[0].radius);
  Serial.print("X:");Serial.print(satPacket.data.imagingData.lightnings[0].x);
  Serial.print("Y:");Serial.println(satPacket.data.imagingData.lightnings[0].y);
}

void control_print_packet_info(){
  Serial.print(PRINT_STR);
  Serial.print("Receiving telemetry:Length:");
  Serial.print(satPacket.length);
  Serial.print(":Protocol:");Serial.print(satPacket.operation.protocol);
  Serial.print(":Operation:");Serial.print(satPacket.operation.operation);
  Serial.println(":Telemetry received!");
}