#include "Arduino.h"
#include "SatComm.h"

#include <EbyteLib.h>

uint8_t rxChan = 23;
uint8_t rxAddh = 0xa1;
uint8_t rxAddl = 0x06;
uint8_t txChan = 10;
uint8_t txAddh = 0x8f;
uint8_t txAddl = 0xf7;

bool rf_comm_received = false;
unsigned int comm_pointer = 0;

Message message;

// #define N 127
Telemetry telemetry = {
  .length = sizeof(Telemetry),
  .type = "Telemetry",
  .telemetry_message = "General Kenobi!",
  .instrument_1 = false,
  .instrument_2 = true,
  .instrument_3 = true,
  .instrument_4 = false
};

void sendTelemetry(){
  asynchronousWriteFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&telemetry, sizeof(telemetry));
  // writeFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&telemetry, sizeof(telemetry));
  // printTransmissionResult(5000);
}

void updateRFComm(){
  char c;
  while(e32serial.available()){
    if(e32serial.overflow()){
      Serial.println("Serial OVERFLOW");
    }
    ((uint8_t*)&message)[comm_pointer++] = (uint8_t)e32serial.read();
    if(comm_pointer < 2){
      
    } else if(comm_pointer == 2){
      Serial.print("\n\nReceiving message of length ");
      Serial.println(message.length);
    } else if(comm_pointer==message.length){
      delay(200);
      Serial.print("Responding with a telemetry of length ");
      Serial.println(sizeof(telemetry));
      sendTelemetry();
      comm_pointer = 0;
      Serial.print("Message received!\n\n");
      Serial.print("Message type: ");Serial.println(message.type);
      Serial.print("Message: ");Serial.println(message.message);
      Serial.print("Actuator 1: ");Serial.println(message.actuator_1);
      Serial.print("Actuator 2: ");Serial.println(message.actuator_2);
      Serial.print("Actuator 3: ");Serial.println(message.actuator_3);
      Serial.print("Actuator 4: ");Serial.println(message.actuator_4);
    }
  }
}

void satReceiveCallback(){

}