#include <iostream>
using namespace std;

#include "Logger.h"
#include "RFModemOffline.h"
#include "RFModule.h"
#include "Timing.h"

unsigned long int status_write_period = 500;
unsigned long int imaging_write_period = 250;
unsigned long int status_write_time = 0;
unsigned long int imaging_write_time = 0;

bool enable_writing = false;

void write_status_data(){
  HealthData he = {
    .index = writing_status_counter,
    .time = 1,
    .battery_voltage = 3.7,
    .battery_current = 2.0,
    .battery_charge = 100.0,
    .battery_temperature = 33.0,
    .internal_temperature = 37.9,
    .external_temperature = 51.2,
    .sd_memory_usage = (unsigned long)3,
    .rasp_data = {0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1},
  };
  logger.writeSatStatusPacket(he);
}
void write_imaging_data(){
  ImagingData im;
  for(int i = 0 ; i < 5; i++){
    im.lightnings[i].index = writing_imaging_counter;
    im.lightnings[i].duration = 5;
    im.lightnings[i].radius = 1;
    im.lightnings[i].x = i*10;
    im.lightnings[i].y = i*20;
  }
  logger.writeSatImagingDataPacket(im);
}

void loop(){
  // checkSerial();
  updateRFComm();

  unsigned long int t = millis();
  if(t > status_write_time){
    status_write_time += status_write_period;
    if(enable_writing){
      std::cout << "Writing status data";
      write_status_data();
    }
  }
  if(t > imaging_write_time){
    imaging_write_time += imaging_write_period;
    if(enable_writing){
      std::cout << "Writing imaging data";
      // write_imaging_data();
    }
  }
}

/*
void checkSerial(){
  char c;
  while(Serial.available()){
    c = Serial.read();
    if(c=='0'){
      enable_writing = !enable_writing;
    } else if(c=='1'){
      switch_status_file();
    } else if(c=='2'){
      switch_imaging_file();
    }
    // if(c=='\n'){
    //   received_serial = receiving_serial;
    //   receiving_serial = "";
    //   serial_received = true;
    // } else{
    //   receiving_serial += c;
    // }
  // }

  // if(serial_received){
  //   serial_received = false;
  //   if(received_serial.length()==0){
  //     // state_sending = !state_sending;
  //     Serial.println("Sending telemetry");
  //     sendTelemetry();
  //     // Serial.print("Toggled communication ");
  //     // Serial.println(state_sending ? "ON":"OFF");
  //   } else if(received_serial[0] == 'T'){
  //     if(received_serial.startsWith("Tset tx chan ")){
  //       txChan = received_serial.substring(13).toInt();
  //       Serial.print("Set tx channel to ");
  //       Serial.println(txChan);
  //     } else if(received_serial.startsWith("Tset tx addh ")){
  //       txAddh = received_serial.substring(13).toInt();
  //       Serial.print("Set tx addh to ");
  //       Serial.println(txAddh);
  //     } else if(received_serial.startsWith("Tset tx addl ")){
  //       txAddl = received_serial.substring(13).toInt();
  //       Serial.print("Set tx addl to ");
  //       Serial.println(txAddl);
  //     }
  //   } else if(received_serial[0] == 'G'){
  //     if(received_serial.startsWith("GAddh")){
  //       printADDH();
  //     } else if(received_serial.startsWith("GAddl")){
  //       printADDL();
  //     } else if(received_serial.startsWith("GParity")){
  //       printParity();
  //     } else if(received_serial.startsWith("GAirDataRate")){
  //       printAirDataRate();
  //     } else if(received_serial.startsWith("GBaudRate")){
  //       printBaudRate();
  //     } else if(received_serial.startsWith("GChannel")){
  //       printChannel();
  //     } else if(received_serial.startsWith("GTxPower")){
  //       printTransmissionPower();
  //     }
  //   } else if(received_serial[0] == 'S'){
  //     parseMessage(received_serial.substring(1));
  //   } else if(received_serial[0] == 'M'){
  //     Serial.println("Set telemetry index to 0");
  //     comm_pointer = 0;
  //   }
  }
}

*/

int main(){
  std::cout << "Testing Raspberry Gama Satellite communication system with LoRa Ra-01 rf module\n";

  initRFModule();
  std::cout << "Device initiated successfully\n";
  
  // setReceiveCallback(onReceive);

  // while(running){
    loop();
  // }

  // LoRa_end(&modem);
  return 0;
}