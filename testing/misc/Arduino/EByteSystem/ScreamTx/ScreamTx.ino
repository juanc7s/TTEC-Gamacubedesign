#include <EbyteLib.h>

uint8_t txChan = 23;
uint8_t txAddh = 0xa1;
uint8_t txAddl = 0x06;
uint8_t rxChan = 10;
uint8_t rxAddh = 0x8f;
uint8_t rxAddl = 0xf7;

#define N 58
uint8_t message[N];

void setup(){
  Serial.begin(57600);

  Serial.println("Scream with EByte32 rf module");

  initE32();
  // Serial.println("CONTROL:Device initiated successfully");

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
  setTransmissionPower(TRANSMISSION_POWER_10dBm);
  setConfiguration();
  
  // printConfiguration();
  setNormalMode();
  // setReceiveCallback(onReceive);

  for(uint8_t i = 0; i < N; i++){
    message[i] = 0xa1;
  }
}

bool flag = false;

void loop(){
  while(Serial.available()){
    Serial.read();
    flag = !flag;
  }

  if(flag){
    if(getTransmissionMode()==FIXED_TRANSMISSION_MODE){
      writeFixedTransmission(txAddh, txAddl, txChan, message, N);
    } else{
      write(message, N);
    }
    if(getTransmissionResult(1000)){
      Serial.println("Message sent");
    } else{
      Serial.println("Message not sent");
    }
  }
}
