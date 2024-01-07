#include "Arduino.h"
#include "gsComm.h"

#include <EbyteLib.h>
#include <Transmission.h>
#include <Utils.h>
#include <configurations.h>
#include <debugging.h>
#include <definitions.h>
#include <modes.h>

uint8_t txChan = 23;
uint8_t txAddh = 0xa1;
uint8_t txAddl = 0x06;
uint8_t rxChan = 10;
uint8_t rxAddh = 0x8f;
uint8_t rxAddl = 0xf7;

unsigned int telemetry_index = 0;
bool telemetry_received = false;
  
Message message = {
  .length = sizeof(Message),
  .type = "Command  ",
  .message_1 = "Hello There!",
  .actuator_1 = false,
  .actuator_2 = true,
  .actuator_3 = true,
  .actuator_4 = false
}; // sending a struct with multiple fields

Telemetry telemetry;

void sendCommand(){
  telemetry_index = 0;
  Serial.print("Sending a message of length ");
  Serial.println(sizeof(message));
  asynchronousWriteFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&message, sizeof(message));
  if(!getTransmissionResult(500)){
    Serial.println("Transmission failed.\nSkipping to next iteration.");
  } else{
    // delay(3000);
    Serial.println("Waiting for response");
    listenForResponse(1000);
  }
}

void listenForResponse(unsigned long int timeout){
  unsigned long int to = millis() + timeout;
  while(millis() < to){
    updateRFComm();
    if(telemetry_received){
      telemetry_received = false;
      return;
    }
  }
  Serial.println("No response");
}

void updateRFComm(){
  char c;
  while(e32serial.available()){
    if(e32serial.overflow()){
      Serial.println("Serial OVERFLOW");
    }
    ((uint8_t*)&telemetry)[telemetry_index++] = (uint8_t)e32serial.read();
    if(telemetry_index < 2){
      
    } else if(telemetry_index == 2){
      Serial.print("\n\nReceiving telemetry of length ");
      Serial.println(telemetry.length);
    } else if(telemetry_index==telemetry.length){
      telemetry_index = 0;
      telemetry_received = true;
      Serial.print("Telemetry received!\n\n");
      Serial.print("Telemetry type: ");Serial.println(telemetry.type);
      Serial.print("Telemetry 1: ");Serial.println(telemetry.telemetry_message);
      Serial.print("Instrument 1: ");Serial.println(telemetry.instrument_1);
      Serial.print("Instrument 2: ");Serial.println(telemetry.instrument_2);
      Serial.print("Instrument 3: ");Serial.println(telemetry.instrument_3);
      Serial.print("Instrument 4: ");Serial.println(telemetry.instrument_4);
    }
  }
}