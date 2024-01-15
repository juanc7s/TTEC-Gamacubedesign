#include <EbyteLib.h>
// #include <Transmission.h>
// #include <Utils.h>
// #include <configurations.h>
// #include <debugging.h>
// #include <definitions.h>
// #include <modes.h>

#include "SDData.h"
#include "SatComm.h"

void setup(){
  Serial.begin(57600);

  Serial.println("Testing Gama Satellite communication system with EByte32 rf module");

  initE32();
  Serial.println("Device initiated successfully");

  setHEAD(SAVE_ON_POWER_DOWN);
  setADDH(rxAddh);
  setADDL(rxAddl);
  setChannel(rxChan);
  setParity(UART_PARITY_BIT_8N1);
  setBaudRate(TTL_UART_BAUD_RATE_57600);
  setAirDataRate(AIR_DATA_RATE_19200);
  setTransmissionMode(FIXED_TRANSMISSION_MODE);
  setIODriveMode(IO_DRIVE_MODE_PUSH_PULL);
  setWirelessWakeUpTime(WIRELESS_WAKE_UP_TIME_250ms);
  setFECSwitch(FEC_SWITCH_ON);
  setTransmissionPower(TRANSMISSION_POWER_20dBm);
  setConfiguration();
  
  setNormalMode();

  init_sd_logger();
  
  // setReceiveCallback(onReceive);
}

unsigned long int status_write_period = 500;
unsigned long int imaging_write_period = 250;
unsigned long int status_write_time = 0;
unsigned long int imaging_write_time = 0;

void loop(){
  checkSerial();
  updateRFComm();

  unsigned long int t = millis();
  if(t < status_write_time){
    status_write_time += status_write_period;
    // write_status_data();
    Serial.println("Writing status data");
  }
  if(t < imaging_write_time){
    imaging_write_time += imaging_write_period;
    Serial.println("Writing imaging data");
    // write_imaging_data();
  }
}

void write_status_data(){
  uint8_t rasp_data[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a};
  // sdWriteSatStatusPacket(millis(), 3.3, 1.0, 1.1, 30.0, 40.0, 50.0, 1000, rasp_data);
}

void write_imaging_data(){
}

void checkSerial(){
  // char c;
  // while(Serial.available() && !serial_received){
  //   c = Serial.read();
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
  // }
}

