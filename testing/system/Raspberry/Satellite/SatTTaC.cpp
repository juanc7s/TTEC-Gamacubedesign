#include <iostream>
using namespace std;

// #include "libs/sx1278-LoRa-RaspberryPi/LoRa.h"
#ifdef __cplusplus
extern "C" {
  #include "./libs/sx1278-LoRa-RaspberryPi/LoRa.h"
}
#endif
/*
#include <EbyteLib.h>
*/
// extern "C"{
// #include "LoRa.h"
// }
// #include "SDData.h"
// #include "SatComm.h"

// int running = 1;

void * rx_f(void *p){
    rxData *rx = (rxData *)p;
    printf("rx done \n");
    printf("CRC error: %d\n", rx->CRC);
    printf("Data size: %d\n", rx->size);
    printf("string: %s\n", rx->buf);//Data we'v received
    printf("RSSI: %d\n", rx->RSSI);
    printf("SNR: %f\n", rx->SNR);
    free(p);
    return NULL;
}

void initRFModule(){

    LoRa_ctl modem;

    //See for typedefs, enumerations and there values in LoRa.h header file
    modem.spiCS = 0;//Raspberry SPI CE pin number
    modem.rx.callback = rx_f;
    modem.eth.payloadLen = 5;//payload len used in implicit header mode
    modem.eth.preambleLen=6;
    modem.eth.bw = BW62_5;//Bandwidth 62.5KHz
    modem.eth.sf = SF12;//Spreading Factor 12
    modem.eth.ecr = CR8;//Error coding rate CR4/8
    modem.eth.CRC = 1;//Turn on CRC checking
    modem.eth.freq = 434800000;// 434.8MHz
    modem.eth.resetGpioN = 4;//GPIO4 on lora RESET pi
    modem.eth.dio0GpioN = 17;//GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
    modem.eth.outPower = OP20;//Output power
    modem.eth.powerOutPin = PA_BOOST;//Power Amplifire pin
    modem.eth.AGC = 1;//Auto Gain Control
    modem.eth.OCP = 240;// 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 1;//Implicit header mode
    modem.eth.syncWord = 0x12;
    //For detail information about SF, Error Coding Rate, Explicit header, Bandwidth, AGC, Over current protection and other features refer to sx127x datasheet https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf

    LoRa_begin(&modem);
    LoRa_receive(&modem);

    sleep(60);
    printf("end\n");
    LoRa_end(&modem);
}

int main(){
  std::cout << "Testing Raspberry Gama Satellite communication system with LoRa Ra-01 rf module\n";

  initRFModule();
  std::cout << "Device initiated successfully\n";

  // init_sd_logger();
  
  // setReceiveCallback(onReceive);

  // while(running){
  //   loop();
  // }

  return 0;
}
/*
unsigned long int status_write_period = 500;
unsigned long int imaging_write_period = 250;
unsigned long int status_write_time = 0;
unsigned long int imaging_write_time = 0;

bool enable_writing = false;

void loop(){
  // checkSerial();
  // updateRFComm();

  // unsigned long int t = millis();
  // if(t > status_write_time){
  //   status_write_time += status_write_period;
  //   if(enable_writing){
  //     std::cout << "Writing status data";
  //     // write_status_data();
  //   }
  // }
  // if(t > imaging_write_time){
  //   imaging_write_time += imaging_write_period;
  //   if(enable_writing){
  //     std::cout << "Writing imaging data";
  //     // write_imaging_data();
  //   }
  // }
}

void write_status_data(){
  uint8_t rasp_data[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a};
  // sdWriteSatStatusPacket(millis(), 3.3, 1.0, 1.1, 30.0, 40.0, 50.0, 1000, rasp_data);
}
*/
// void write_imaging_data(){
//   ImagingData imagingData;
//   imagingData.time = millis();
//   for(uint8_t i = 0; i < 5; i++){
//     imagingData->lightnings[i].x = 10 - i;
//     imagingData->lightnings[i].y = 7 + i;
//     imagingData->lightnings[i].radius = 20;
//     imagingData->lightnings[i].duration = i;
//   }
//   sdWriteSatImagingPacket(imagingData);
// }

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