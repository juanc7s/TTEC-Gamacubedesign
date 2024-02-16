#include <iostream>
#include <thread>
#include <filesystem>
#include <iostream>
#include <sys/statvfs.h>
#include <cstring>
using namespace std;

#include "Logger.h"
#include "RFModemOffline.h"
#include "RFModule.h"
#include "Timing.h"

#include "StatusServer.cpp"
#include "ImagingServer.cpp"

unsigned long int status_write_period = 500;
unsigned long int imaging_write_period = 250;
unsigned long int status_write_time = 0;
unsigned long int imaging_write_time = 0;

bool enable_writing = false;

bool running = true;

ImagingFIFO imaging_fifo;
StatusFIFO status_fifo;

ImagingDataServer server_1(&imaging_fifo, 8080);
StatusDataServer server_2(&status_fifo, 8081);

void run_server_1(){
  server_1.run_server();
}

void run_server_2(){
  server_2.run_server();
}

void read_fifos(){
  while(imaging_fifo.available()){
    ImagingData imagingPacket = imaging_fifo.read();
    logger.writeSatImagingDataPacket(imagingPacket);
    // cout << "Imaging FIFO: " << endl;
    // for(int i = 0; i < 5; i++){
    //   cout << "Index: " << newPacket.lightnings[i].index << endl;
    //   cout << "Duration: " << newPacket.lightnings[i].duration << endl;
    //   cout << "Size: " << newPacket.lightnings[i].radius << endl;
    //   cout << "x: " << newPacket.lightnings[i].x << endl;
    //   cout << "y: " << newPacket.lightnings[i].y << endl << endl;
    // }
    // cout << endl;
  }
  while(status_fifo.available()){
    HealthData statusPacket = status_fifo.read();
    // cout << "Packet bytes: " << endl;
    // for(int i = 0; i < sizeof(HealthData); i++){
    //   cout << (int)(((uint8_t*)&statusPacket)[i]) << " ";
    // }
    // cout << endl; 
    logger.writeSatStatusPacket(statusPacket);
  }
}

uint8_t serial_buffer[256];
unsigned int serial_buffer_writing_pointer = 0;
unsigned int serial_buffer_reading_pointer = 0;
unsigned int serial_buffer_size = 0;

void serial_receiving_thread(){
  char c;
  while(running){
    std::cin >> c;
    if(c=='e'){
      running = false;
    } else{
      serial_buffer[serial_buffer_writing_pointer++] = c;
      if(serial_buffer_writing_pointer==256){
        serial_buffer_writing_pointer = 0;
      }
      serial_buffer_size++;
    }
  }
}

uint8_t serial_available(){
  return serial_buffer_size;
}

uint8_t serial_read(){
  serial_buffer_size--;
  uint8_t b = serial_buffer[serial_buffer_reading_pointer++];
  if(serial_buffer_reading_pointer==256){
    serial_buffer_reading_pointer = 0;
  }
  return b;
}

void checkSerial(){
  char c;
  while(serial_available()){
    c = serial_read();
    GSPacket sending_packet;
    switch(c){
      case '0':
        std::cout << "Enabling writing" << std::endl;
        enable_writing = !enable_writing;
        break;
      case '1':
        std::cout << "Switching status file" << std::endl;
        logger.switch_status_file();
        break;
      case '2':
        std::cout << "Switching imaging data file" << std::endl;
        logger.switch_imaging_file();
        break;
      case '3':
        std::cout << "Sending status request" << std::endl;
        sending_packet.operation.protocol = PROTOCOL_STATUS;
        sending_packet.operation.operation = GS_STATUS_REQUEST;
        sending_packet.length = 2;
        // std::cout << "Sending GS packet with length " << (int)sending_packet.length << std::endl;
        // std::cout << "Protocol: " << sending_packet.operation.protocol << std::endl;
        // std::cout << "Operation: " << sending_packet.operation.operation << std::endl;
        modem_write((uint8_t*)&sending_packet, sending_packet.length);
        break;
      case '4':
        std::cout << "Sending status start transmission" << std::endl;
        sending_packet.operation.protocol = PROTOCOL_STATUS;
        sending_packet.operation.operation = GS_STATUS_START_TRANSMISSION;
        sending_packet.length = 2;
        // std::cout << "Sending GS packet with length " << (int)sending_packet.length << std::endl;
        // std::cout << "Protocol: " << sending_packet.operation.protocol << std::endl;
        // std::cout << "Operation: " << sending_packet.operation.operation << std::endl;
        // std::cout << "Writing to rx buffer: ";
        // for(int i = 0; i < sending_packet.length; i++){
        //   std::cout << (int)((uint8_t*)&sending_packet)[i] << " ";
        // }
        // std::cout << std::endl;
        modem_write((uint8_t*)&sending_packet, sending_packet.length);
        break;
      case '5':
        std::cout << "Sending resend status" << std::endl;
        sending_packet.operation.protocol = PROTOCOL_STATUS;
        sending_packet.operation.operation = GS_STATUS_RESEND_PACKET;
        for(unsigned int i = 0; i < 32; i++){
          sending_packet.data.resend.packets[i] = 0;
        }
        sending_packet.data.resend.packets[0] |= 0x2;
        // sending_packet.data.resend.packets[10] |= 0x10;
        // sending_packet.data.resend.packets[23] |= 0x8;
        gsPacket.data.resend.isDone = false;
        sending_packet.length = 35;
        modem_write((uint8_t*)&sending_packet, sending_packet.length);
        break;
      case '6':
        std::cout << "Sending status done" << std::endl;
        sending_packet.operation.protocol = PROTOCOL_STATUS;
        sending_packet.operation.operation = GS_STATUS_DONE;
        sending_packet.length = 2;
        modem_write((uint8_t*)&sending_packet, sending_packet.length);
        break;
      case '7':
        std::cout << "Sending imaging request" << std::endl;
        sending_packet.operation.protocol = PROTOCOL_IMAGING_DATA;
        sending_packet.operation.operation = GS_IMAGING_REQUEST;
        sending_packet.length = 2;
        modem_write((uint8_t*)&sending_packet, sending_packet.length);
        break;
      case '8':
        std::cout << "Sending status start transmission" << std::endl;
        sending_packet.operation.protocol = PROTOCOL_IMAGING_DATA;
        sending_packet.operation.operation = GS_IMAGING_START_TRANSMISSION;
        sending_packet.length = 2;
        modem_write((uint8_t*)&sending_packet, sending_packet.length);
        break;
      case '9':
        std::cout << "Sending resend status" << std::endl;
        sending_packet.operation.protocol = PROTOCOL_IMAGING_DATA;
        sending_packet.operation.operation = GS_IMAGING_RESEND_STATUS;
        for(unsigned int i = 0; i < 32; i++){
          gsPacket.data.resend.packets[i] = 0;
        }
        gsPacket.data.resend.packets[2] |= 0x2;
        gsPacket.data.resend.packets[10] |= 0x10;
        gsPacket.data.resend.packets[23] |= 0x8;
        gsPacket.data.resend.isDone = false;
        sending_packet.length = 35;
        modem_write((uint8_t*)&sending_packet, sending_packet.length);
        break;
      case 'a':
        std::cout << "Sending status done" << std::endl;
        sending_packet.operation.protocol = PROTOCOL_IMAGING_DATA;
        sending_packet.operation.operation = GS_IMAGING_DONE;
        sending_packet.length = 2;
        modem_write((uint8_t*)&sending_packet, sending_packet.length);
        break;
    }
    // if(c=='\n'){
    //   received_serial = receiving_serial;
    //   receiving_serial = "";
    //   serial_received = true;
    // } else{
    //   receiving_serial += c;
    // }
  }

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
  // }
}

void loop(){
  checkSerial();
  updateRFComm();
  read_fifos();

  // unsigned long int t = millis();
  // if(t > status_write_time){
  //   status_write_time += status_write_period;
  //   if(enable_writing){
  //     std::cout << "Writing status data" << std::endl;
  //     write_status_data();
  //   }
  // }
  // if(t > imaging_write_time){
  //   imaging_write_time += imaging_write_period;
  //   if(enable_writing){
  //     std::cout << "Writing imaging data" << std::endl;
  //     // write_imaging_data();
  //   }
  // }
}

int main( int argc, char *argv[] ){
  std::cout << "Testing Raspberry Gama Satellite communication system with LoRa Ra-01 rf module\n";

  initRFModule();
  std::cout << "Device initiated successfully\n";

  std::thread thread_1 (run_server_1);
  std::thread thread_2 (run_server_2);
  
  // setReceiveCallback(onReceive);
  // for(int i = 0; i < 30; i++){
  //   write_status_data();
  // }
  // for(int i = 0; i < 30; i++){
  //   write_imaging_data();
  // }

  std::thread thread_object (serial_receiving_thread);
  while(running){
    loop();
  }

  cout << "Closing servers" << endl;
  server_1.close_server();
  server_2.close_server();
  thread_object.join();
  thread_1.join();
  thread_2.join();
  
  exit(1);

  // LoRa_end(&modem);
  return 0;
}