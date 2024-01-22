#include "Arduino.h"
#include "Control.h"

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
    // Serial.println((uint8_t)c);
    if(serialFlag){
      parsing_function(c);
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

// void decodeSerial(){

// }

void parseSerial(uint8_t c){
  switch(c){
    case SET_TX_CHAN:
      parsing_function = control_setTxChannel;
      break;
    case SET_TX_ADDH:
      parsing_function = control_setTxAddh;
      break;
    case SET_TX_ADDL:
      parsing_function = control_setTxAddl;
      break;
    case READ_ADDH:
      Serial.print(CONTROL_STR);Serial.print("ADDH:");
      Serial.println(getADDH(), HEX);
      break;
    case READ_ADDL:
      Serial.print(CONTROL_STR);Serial.print("ADDL:");
      Serial.println(getADDL(), HEX);
      break;
    case READ_CHAN:
      Serial.print(CONTROL_STR);Serial.print("CHAN:");
      Serial.println(getChannel(), HEX);
      break;
    case READ_PARITY:
      Serial.print(CONTROL_STR);Serial.print("PARITY:");
      switch(getParity()){
        case UART_PARITY_BIT_8N1:
          Serial.println("8N1");
          break;
        case UART_PARITY_BIT_8O1:
          Serial.println("8O1");
          break;
        case UART_PARITY_BIT_8E1:
          Serial.println("8E1");
          break;
        case UART_PARITY_BIT_8N1_11:
          Serial.println("8N1");
          break;
      }
      break;
    case READ_UART_BAUD_RATE:
      Serial.print(CONTROL_STR);Serial.print("UART_BAUD_RATE:");
      Serial.print(getBaudRate());
      Serial.println(" bps");
      break;
    case READ_AIR_DATA_RATE:
      Serial.print(CONTROL_STR);Serial.print("AIR_DATA_RATE:");
      Serial.print(getAirDataRate());
      Serial.println(" bps");
      break;
    case READ_TRANSMISSION_POWER:
      Serial.print(CONTROL_STR);Serial.print("TXPW:");
      switch(getTransmissionPower()){
        case TRANSMISSION_POWER_20dBm:
          Serial.println("20 dBm");
          break;
        case TRANSMISSION_POWER_17dBm:
          Serial.println("17 dBm");
          break;
        case TRANSMISSION_POWER_14dBm:
          Serial.println("14 dBm");
          break;
        case TRANSMISSION_POWER_10dBm:
          Serial.println("10 dBm");
          break;
      }
      break;
    case READ_TRANSMISSION_MODE:
      Serial.print(CONTROL_STR);Serial.print("TXMO:");
      switch(getTransmissionMode()){
        case TRANSPARENT_TRANSMISSION_MODE:
          Serial.println("TRANSPARENT TRANSMISSION");
          break;
        case FIXED_TRANSMISSION_MODE:
          Serial.println("FIXED TRANSMISSION");
          break;
      }
      break;
    case READ_OPERATION_MODE:
      Serial.print(CONTROL_STR);Serial.print("OPMO:");
      switch(getOperationMode()){
        case NORMAL:
          Serial.println("NORMAL");
          break;
        case WAKE_UP:
          Serial.println("WAKE_UP");
          break;
        case POWER_SAVING:
          Serial.println("POWER_SAVING");
          break;
        case SLEEP:
          Serial.println("SLEEP");
          break;
      }
      break;
    case READ_ALL:
      // printConfiguration();
      break;
    case SET_ADDH:
      parsing_function = control_setAddh;
      break;
    case SET_ADDL:
      parsing_function = control_setAddl;
      break;
    case SET_CHAN:
      parsing_function = control_setChannel;
      break;
    case SET_PARITY:
      parsing_function = control_setParity;
      break;
    case SET_UART_BAUD_RATE:
      parsing_function = control_setUARTBaudRate;
      break;
    case SET_AIR_DATA_RATE:
      parsing_function = control_setAirDataRate;
      break;
    case SET_TRANSMISSION_POWER:
      parsing_function = control_setTxPower;
      break;
    case SET_TRANSMISSION_MODE:
      parsing_function = control_setTxMode;
      break;
    case SET_OPERATION_MODE:
      parsing_function = control_setOperationMode;
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
  // delay(400);
}

void control_setTxChannel(uint8_t c){
  txChan = c;
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

void control_setAddh(uint8_t c){
  setADDH((uint8_t)c);
  setConfiguration();
  parsing_function = parseSerial;
}

void control_setAddl(uint8_t c){
  setADDL((uint8_t)c);
  setConfiguration();  
  parsing_function = parseSerial;
}

void control_setChannel(uint8_t c){
  setChannel((uint8_t)c);
  setConfiguration();
  parsing_function = parseSerial;
}

void control_setParity(uint8_t c){
  switch(c){
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
  parsing_function = parseSerial;
}

void control_setUARTBaudRate(uint8_t c){
  // switch(((unsigned int)c)<<8 + (unsigned int)received_serial[2]){
  //   case 12:
  //     setBaudRate(TTL_UART_BAUD_RATE_1200);
  //     break;
  //   case 24:
  //     setBaudRate(TTL_UART_BAUD_RATE_2400);
  //     break;
  //   case 48:
  //     setBaudRate(TTL_UART_BAUD_RATE_4800);
  //     break;
  //   case 96:
  //     setBaudRate(TTL_UART_BAUD_RATE_9600);
  //     break;
  //   case 192:
  //     setBaudRate(TTL_UART_BAUD_RATE_19200);
  //     break;
  //   case 384:
  //     setBaudRate(TTL_UART_BAUD_RATE_38400);
  //     break;
  //   case 576:
  //     setBaudRate(TTL_UART_BAUD_RATE_57600);
  //     break;
  //   case 1152:
  //     setBaudRate(TTL_UART_BAUD_RATE_115200);
  //     break;
  // }
  setBaudRate((TTL_UART_BAUD_RATE)c);
  setConfiguration();
  parsing_function = parseSerial;
}

void control_setAirDataRate(uint8_t c){
  // switch((uint8_t)c){
  //   case 3:
  //     setAirDataRate(AIR_DATA_RATE_300);
  //     break;
  //   case 12:
  //     setAirDataRate(AIR_DATA_RATE_1200);
  //     break;
  //   case 24:
  //     setAirDataRate(AIR_DATA_RATE_2400);
  //     break;
  //   case 48:
  //     setAirDataRate(AIR_DATA_RATE_4800);
  //     break;
  //   case 96:
  //     setAirDataRate(AIR_DATA_RATE_9600);
  //     break;
  //   case 192:
  //     setAirDataRate(AIR_DATA_RATE_19200);
  //     break;
  // }
  setAirDataRate((AIR_DATA_RATE)c);
  setConfiguration();
  parsing_function = parseSerial;
}

void control_setTxPower(uint8_t c){
  switch((uint8_t)c){
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
  parsing_function = parseSerial;
}

void control_setTxMode(uint8_t c){
  switch((uint8_t)c){
    case 0:
      setTransmissionMode(TRANSPARENT_TRANSMISSION_MODE);
      break;
    case 1:
      setTransmissionMode(FIXED_TRANSMISSION_MODE);
      break;
  }
  setConfiguration();
  parsing_function = parseSerial;
}

void control_setOperationMode(uint8_t c){
  setOperationMode((OperationMode)c);
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