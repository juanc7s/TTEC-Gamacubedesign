#include <iostream>
#include <thread>
#include <filesystem>
#include <iostream>
#include <sys/statvfs.h>
#include <cstring>
using namespace std;

//#ifdef __cplusplus
//  extern "C" {
//    #include "../sx1278-LoRa-RaspberryPi/LoRa.h"
//  }
//#endif
#include "RFModem.h"

#include "Logger.h"
#include "RFModule.h"
#include "Timing.h"

#include "StatusServer.cpp"
#include "ImagingServer.cpp"

// #include "asynchronous_in.cpp"
#include "printing_utils.cpp"

unsigned long int status_write_period = 500;
unsigned long int imaging_write_period = 250;
unsigned long int status_write_time = 0;
unsigned long int imaging_write_time = 0;

bool enable_writing = false;

ImagingFIFO imaging_fifo;
StatusFIFO status_fifo;

StatusDataServer status_server(&status_fifo, 8081);
ImagingDataServer imaging_server(&imaging_fifo, 8080);

bool running = false;

void run_status_server(){
  status_server.run_server();
}

void run_imaging_server(){
  imaging_server.run_server();
}

void read_fifos(){
  while(imaging_fifo.available()){
    ImagingData imagingPacket = imaging_fifo.read();
    logger.writeSatImagingDataPacket(imagingPacket);
  }
  while(status_fifo.available()){
    HealthData statusPacket = status_fifo.read();
    logger.writeSatStatusPacket(statusPacket);
  }
}

/*void checkConsole(){
  char c;
  while(console.available() > 0){
    c = console.read();
//     GSPacket sending_packet;
    switch(c){
//       case '0':
//         std::cout << "Enabling writing" << std::endl;
//         enable_writing = !enable_writing;
//         break;
//       case '1':
//         std::cout << "Switching status file" << std::endl;
//         logger.switch_status_file();
//         break;
//       case '2':
//         std::cout << "Switching imaging data file" << std::endl;
//         logger.switch_imaging_file();
//         break;
//       case '3':
//         std::cout << "Sending status request" << std::endl;
//         sending_packet.operation.protocol = PROTOCOL_STATUS;
//         sending_packet.operation.operation = GS_STATUS_REQUEST;
//         sending_packet.length = 2;
//         // std::cout << "Sending GS packet with length " << (int)sending_packet.length << std::endl;
//         // std::cout << "Protocol: " << sending_packet.operation.protocol << std::endl;
//         // std::cout << "Operation: " << sending_packet.operation.operation << std::endl;
//         modem_write((uint8_t*)&sending_packet, sending_packet.length);
//         break;
//       case '4':
//         std::cout << "Sending status start transmission" << std::endl;
//         sending_packet.operation.protocol = PROTOCOL_STATUS;
//         sending_packet.operation.operation = GS_STATUS_START_TRANSMISSION;
//         sending_packet.length = 2;
//         // std::cout << "Sending GS packet with length " << (int)sending_packet.length << std::endl;
//         // std::cout << "Protocol: " << sending_packet.operation.protocol << std::endl;
//         // std::cout << "Operation: " << sending_packet.operation.operation << std::endl;
//         // std::cout << "Writing to rx buffer: ";
//         // for(int i = 0; i < sending_packet.length; i++){
//         //   std::cout << (int)((uint8_t*)&sending_packet)[i] << " ";
//         // }
//         // std::cout << std::endl;
//         modem_write((uint8_t*)&sending_packet, sending_packet.length);
//         break;
//       case '5':
//         std::cout << "Sending resend status" << std::endl;
//         sending_packet.operation.protocol = PROTOCOL_STATUS;
//         sending_packet.operation.operation = GS_STATUS_RESEND_PACKET;
//         for(unsigned int i = 0; i < 32; i++){
//           sending_packet.data.resend.packets[i] = 0;
//         }
//         sending_packet.data.resend.packets[0] |= 0x2;
//         // sending_packet.data.resend.packets[10] |= 0x10;
//         // sending_packet.data.resend.packets[23] |= 0x8;
//         gsPacket.data.resend.isDone = false;
//         sending_packet.length = 35;
//         modem_write((uint8_t*)&sending_packet, sending_packet.length);
//         break;
//       case '6':
//         std::cout << "Sending status done" << std::endl;
//         sending_packet.operation.protocol = PROTOCOL_STATUS;
//         sending_packet.operation.operation = GS_STATUS_DONE;
//         sending_packet.length = 2;
//         modem_write((uint8_t*)&sending_packet, sending_packet.length);
//         break;
//       case '7':
//         std::cout << "Sending imaging request" << std::endl;
//         sending_packet.operation.protocol = PROTOCOL_IMAGING_DATA;
//         sending_packet.operation.operation = GS_IMAGING_REQUEST;
//         sending_packet.length = 2;
//         modem_write((uint8_t*)&sending_packet, sending_packet.length);
//         break;
//       case '8':
//         std::cout << "Sending status start transmission" << std::endl;
//         sending_packet.operation.protocol = PROTOCOL_IMAGING_DATA;
//         sending_packet.operation.operation = GS_IMAGING_START_TRANSMISSION;
//         sending_packet.length = 2;
//         modem_write((uint8_t*)&sending_packet, sending_packet.length);
//         break;
//       case '9':
//         std::cout << "Sending resend status" << std::endl;
//         sending_packet.operation.protocol = PROTOCOL_IMAGING_DATA;
//         sending_packet.operation.operation = GS_IMAGING_RESEND_STATUS;
//         for(unsigned int i = 0; i < 32; i++){
//           gsPacket.data.resend.packets[i] = 0;
//         }
//         gsPacket.data.resend.packets[2] |= 0x2;
//         gsPacket.data.resend.packets[10] |= 0x10;
//         gsPacket.data.resend.packets[23] |= 0x8;
//         gsPacket.data.resend.isDone = false;
//         sending_packet.length = 35;
//         modem_write((uint8_t*)&sending_packet, sending_packet.length);
//         break;
//       case 'a':
//         std::cout << "Sending status done" << std::endl;
//         sending_packet.operation.protocol = PROTOCOL_IMAGING_DATA;
//         sending_packet.operation.operation = GS_IMAGING_DONE;
//         sending_packet.length = 2;
//         modem_write((uint8_t*)&sending_packet, sending_packet.length);
//         break;
      case 'b':
        cout << "Echo console" << endl;
        break;
    }
//     // if(c=='\n'){
//     //   received_serial = receiving_serial;
//     //   receiving_serial = "";
//     //   serial_received = true;
//     // } else{
//     //   receiving_serial += c;
//     // }
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
}*/

void loop(){
  //checkConsole();
  updateRFComm();
  read_fifos();
}

int main( int argc, char *argv[] ){
  std::cout << "Raspberry Gama Satellite communication system with LoRa Ra-01 rf module\n";

  initRFModule();
  std::cout << "Device initiated successfully\n";

  std::thread status_thread (run_status_server);
  std::thread imaging_thread (run_imaging_server);

  // std::thread async_in_thread = start_serial_thread();
  running = true;
  while(running){
    loop();
  }

  cout << "Closing servers" << endl;
  status_server.close_server();
  imaging_server.close_server();
  // async_in_thread.join();
  status_thread.join();
  imaging_thread.join();
  
  exit(1);

  modem_finish();
  return 0;
}
