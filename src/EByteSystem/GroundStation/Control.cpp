#include "Arduino.h"
#include "Control.h"

#include <EbyteLib.h>
#include <Transmission.h>
#include <Utils.h>
#include <configurations.h>
#include <debugging.h>
#include <definitions.h>
#include <modes.h>

bool serialFlag = false;

void checkControl(){
  char c;
  while(Serial.available() && !serial_received){
    c = Serial.read();
    Serial.println((uint8_t)c);
    if(serialFlag){
      received_serial += c;
    } else if(c==13){
      serialFlag = true;
    } else if(c=='\n'){
      serial_received = true;
    } else{
      received_serial += c;
    }
  }

  if(serial_received){
    // decodeSerial();
    parseSerial();
    received_serial = "";
  }
}

// void decodeSerial(){

// }

void parseSerial(){
  serial_received = false;
  switch(received_serial[0]){
    case TOGGLE_COMMUNICATION:
      state_sending = ((uint8_t)received_serial[1]) != 0;
      break;
    case SET_TX_CHAN:
      txChan = received_serial[1];
      break;
    case SET_TX_ADDH:
      txAddh = received_serial[1];
      break;
    case SET_TX_ADDL:
      txAddl = received_serial[1];
      break;
    case READ_ADDH:
      Serial.print("CONTROL:ADDH:");
      Serial.print(getADDH(), HEX);
      break;
    case READ_ADDL:
      Serial.print("CONTROL:ADDL:");
      Serial.print(getADDL(), HEX);
      break;
    case READ_CHAN:
      Serial.print("CONTROL:CHAN:");
      Serial.print(getChannel(), HEX);
      break;
    case READ_PARITY:
      Serial.print("CONTROL:PARITY:");
      printParity();
      break;
    case READ_UART_BAUD_RATE:
      Serial.print("CONTROL:UART_BAUD_RATE:");
      printBaudRate();
      break;
    case READ_AIR_DATA_RATE:
      Serial.print("CONTROL:AIR_DATA_RATE:");
      printAirDataRate();
      break;
    case READ_TRANSMISSION_POWER:
      Serial.print("CONTROL:TXPW:");
      printTransmissionPower();
      break;
    case READ_TRANSMISSION_MODE:
      Serial.print("CONTROL:TXMO:");
      printTransmissionMode();
      break;
    case READ_ALL:
      printConfiguration();
    case SET_ADDH:
      setADDH((uint8_t)received_serial[1]);
      setConfiguration();
      break;
    case SET_ADDL:
      setADDL((uint8_t)received_serial[1]);
      setConfiguration();
      break;
    case SET_CHAN:
      setChannel((uint8_t)received_serial[1]);
      setConfiguration();
      break;
    case SET_PARITY:
      switch(received_serial[1]){
        case 'N':
          setParity(UART_PARITY_BIT_8N1);
          break;
        case 'O':
          setParity(UART_PARITY_BIT_8O1);
          break;
        case 'E':
          setParity(UART_PARITY_BIT_8E1);
          break;
      }
      setConfiguration();
      break;
    case SET_UART_BAUD_RATE:
      switch(((unsigned int)received_serial[1])<<8 + (unsigned int)received_serial[2]){
        case 12:
          setBaudRate(TTL_UART_baud_rate_1200);
          break;
        case 24:
          setBaudRate(TTL_UART_baud_rate_2400);
          break;
        case 48:
          setBaudRate(TTL_UART_baud_rate_4800);
          break;
        case 96:
          setBaudRate(TTL_UART_baud_rate_9600);
          break;
        case 192:
          setBaudRate(TTL_UART_baud_rate_19200);
          break;
        case 384:
          setBaudRate(TTL_UART_baud_rate_38400);
          break;
        case 576:
          setBaudRate(TTL_UART_baud_rate_57600);
          break;
        case 1152:
          setBaudRate(TTL_UART_baud_rate_115200);
          break;
      }
      setConfiguration();
      break;
    case SET_AIR_DATA_RATE:
      switch((uint8_t)received_serial[1]){
        case 3:
          setAirDataRate(Air_Data_Rate_300);
          break;
        case 12:
          setAirDataRate(Air_Data_Rate_1200);
          break;
        case 24:
          setAirDataRate(Air_Data_Rate_2400);
          break;
        case 48:
          setAirDataRate(Air_Data_Rate_4800);
          break;
        case 96:
          setAirDataRate(Air_Data_Rate_9600);
          break;
        case 192:
          setAirDataRate(Air_Data_Rate_19200);
          break;
      }
      setConfiguration();
      break;
    case SET_TRANSMISSION_POWER:
      switch((uint8_t)received_serial[1]){
        case 10:
          setTransmissionPower(TRANSMISSION_POWER_10dBm);
          break;
        case 14:
          setTransmissionPower(TRANSMISSION_POWER_14dBm);
          break;
        case 17:
          setTransmissionPower(TRANSMISSION_POWER_17dBm);
          break;
        case 20:
          setTransmissionPower(TRANSMISSION_POWER_20dBm);
          break;
      }
      setConfiguration();
      break;
    case SET_TRANSMISSION_MODE:
      switch((uint8_t)received_serial[1]){
        case 0:
          setTransmissionMode(TRANSPARENT_TRANSMISSION_MODE);
          break;
        case 1:
          setTransmissionMode(FIXED_TRANSMISSION_MODE);
          break;
      }
      setConfiguration();
    case FLUSH:
      telemetry_index = 0;
      break;
    case SET_MESSAGE_DATA:
      break;
  }
}
