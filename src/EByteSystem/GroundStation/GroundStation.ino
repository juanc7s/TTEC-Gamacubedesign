#include <EbyteLib.h>

#include "Control.h"
#include "GSComm.h"

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
}

// String receiving_serial = "";
// String received_serial = "";
// bool serial_received = false;

bool state_sending = false;
uint8_t send_tx = 0;

unsigned long transmission_timer = 1500;
unsigned long next_transmission = 0;

void loop(){
  checkControl();
  updateRFComm();

  if(millis() >= next_transmission){
    next_transmission += transmission_timer;
    if(state_sending){
      unsigned long int t0 = millis();
      sendCommand();
      Serial.print("Communication took ");
      Serial.print(millis() - t0);
      Serial.println("ms");
    } else if(send_tx > 0){
      send_tx--;
      unsigned long int t0 = millis();
      sendCommand();
      Serial.print("Communication took ");
      Serial.print(millis() - t0);
      Serial.println("ms");
    }
  }
}
