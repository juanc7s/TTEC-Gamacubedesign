//#include "FS.h"
//#include "FSUtils.h"

//#include "SdFat.h"
//SdFat SD;
//#include "SdFatUtils.h"
#include <SD.h>
#include "SdUtils.h"

#include <SPI.h>

#include <ArduinoJson.h>
#include <SoftwareSerial.h>
// #include "LoRa_E32.h"

#include "Telemetry.h"

#define E32_RX 7
#define E32_TX 8
#define E32_M0 5
#define E32_M1 6
#define E32_AUX 9

#define SD_CS 10
// #define E32_ON

SoftwareSerial mySerial(E32_RX, E32_TX);
//LoRa_E32 e32ttl(&mySerial, AUX, M0, M1);

String incommingMessage = "";
String receivedMessage = "";
boolean messageReceived = false;

void setup(){
  Serial.begin(57600);
  mySerial.begin(57600);
  delay(100);
  
  Serial.println("E32 LoRa test");
  pinMode(E32_RX, INPUT_PULLUP);

  
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(SPI_FULL_SPEED, SD_CS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  currentTelemetryFile = SD.open("log0.dat", FILE_WRITE);
  nextTelemetry = millis() + telemetryPeriod;
  
  #ifdef E32_ON
  // e32ttl.begin();
  // // After set configuration comment set M0 and M1 to low
  // // and reboot if you directly set HIGH M0 and M1 to program
  // ResponseStructContainer c;
  // c = e32ttl.getConfiguration();
  // Configuration configuration = *(Configuration*) c.data;
  // configuration.ADDL = 0x01;
  // configuration.ADDH = 0x00;
  // configuration.CHAN = 0x02;
  // configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
  // e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  // printParameters(configuration);
  // c.close();
  #endif
}

void loop(){
  telemetry_loop();
  // E32_loop();
  if(messageReceived){
    messageReceived = false;
    if(receivedMessage=="send"){
      telemetry_send();
    }
  }
}

void serialEvent(){
  while(Serial.available()){
    char inChar = Serial.read();
    incommingMessage += inChar;
    if(inChar=='\n'){
      receivedMessage = incommingMessage;
      incommingMessage = "";
      messageReceived = true;
    }
  }
}

#ifdef E32_ON
void E32_loop(){
  ResponseStatus rs = e32ttl.sendFixedMessage(0,3,4,&message, sizeof(Message));
  Serial.println(rs.getResponseDescription());
}
#endif