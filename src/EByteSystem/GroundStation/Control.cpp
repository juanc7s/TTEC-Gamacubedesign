#include "Arduino.h"
#include "Control.h"

#include <EbyteLib.h>
#include <Transmission.h>
#include <Utils.h>
#include <configurations.h>
#include <debugging.h>
#include <definitions.h>
#include <modes.h>

void checkControl(){
  char c;
  while(Serial.available() && !serial_received){
    c = Serial.read();
    if(c=='\n'){
      received_serial = receiving_serial;
      receiving_serial = "";
      serial_received = true;
    } else{
      receiving_serial += c;
    }
  }

  if(serial_received){
    serial_received = false;
    if(received_serial.length()==0){
      state_sending = !state_sending;
      Serial.print("Toggled communication ");
      Serial.println(state_sending ? "ON":"OFF");
    } else if(received_serial[0] == 'T'){
      if(received_serial.startsWith("Tset tx chan ")){
        txChan = received_serial.substring(13).toInt();
        Serial.print("Set tx channel to ");
        Serial.println(txChan);
      } else if(received_serial.startsWith("Tset tx addh ")){
        txAddh = received_serial.substring(13).toInt();
        Serial.print("Set tx addh to ");
        Serial.println(txAddh);
      } else if(received_serial.startsWith("Tset tx addl ")){
        txAddl = received_serial.substring(13).toInt();
        Serial.print("Set tx addl to ");
        Serial.println(txAddl);
      }
    } else if(received_serial[0] == 'G'){
      if(received_serial.startsWith("GAddh")){
        printADDH();
      } else if(received_serial.startsWith("GAddl")){
        printADDL();
      } else if(received_serial.startsWith("GParity")){
        printParity();
      } else if(received_serial.startsWith("GAirDataRate")){
        printAirDataRate();
      } else if(received_serial.startsWith("GBaudRate")){
        printBaudRate();
      } else if(received_serial.startsWith("GChannel")){
        printChannel();
      } else if(received_serial.startsWith("GTxPower")){
        printTransmissionPower();
      }
    } else if(received_serial[0] == 'S'){
      parseMessage(received_serial.substring(1));
    } else if(received_serial[0] == 'M'){
      Serial.println("Set telemetry index to 0");
      telemetry_index = 0;
    }
  }
}
