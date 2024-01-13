#include <EbyteLib.h>

#include "Control.h"
// #include "GSComm.h"

void setup(){
  Serial.begin(57600);

  Serial.println("Testing Gama Ground Station communication system with EByte32 rf module");

  initE32();
  Serial.println("CONTROL:Device initiated successfully");

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
  
  // printConfiguration();
  setNormalMode();
  // setReceiveCallback(onReceive);
}

// String receiving_serial = "";
// String received_serial = "";
// bool serial_received = false;

bool state_sending = false;
uint8_t send_tx = 0;

unsigned long transmission_timer = 100;
unsigned long next_transmission = 0;

void loop(){
  updateRFComm();
  checkControl();

  if(millis() >= next_transmission){
    next_transmission += transmission_timer;
     if(state_request_status){
      startRequestStatusProtocol();
     } else if(send_request_status > 0){
      send_request_status--;
      startRequestStatusProtocol();
     } else if(state_request_imaging){
      startRequestImagingDataProtocol();
     } else if(send_request_imaging > 0){
      send_request_imaging--;
      startRequestImagingDataProtocol();
     } else if(state_command){
      startSetOperationProtocol();
    } else if(send_command > 0){
      send_command--;
      startSetOperationProtocol();
    }
  }
}
