/*
 * LoRa E32-TTL-100
 * Send fixed transmission structured message to a specified point.
 * https://www.mischianti.org/2019/12/03/lora-e32-device-for-arduino-esp32-or-esp8266-power-saving-and-sending-structured-data-part-5/
 *
 * E32-TTL-100----- Arduino UNO or esp8266
 * M0         ----- 3.3v (To config) GND (To send) 7 (To dinamically manage)
 * M1         ----- 3.3v (To config) GND (To send) 6 (To dinamically manage)
 * TX         ----- RX PIN 2 (PullUP)
 * RX         ----- TX PIN 3 (PullUP & Voltage divider)
 * AUX        ----- Not connected (5 if you connect)
 * VCC        ----- 3.3v/5v
 * GND        ----- GND
 *
 */
#include "Arduino.h"
#include "LoRa_E32.h"

// #define E32_RX 9
// #define E32_TX 10
// #define E32_M0 7
// #define E32_M1 6
// #define E32_AUX 2

#define RX 9
#define TX 10
#define AUX 2
#define M0 7
#define M1 6

// #define CHANNEL_TX 0x02
#define CHANNEL_RX 0x04
#define RX_ADDH 0x00
#define RX_ADDL 0x01
#define TX_ADDH 0xff
#define TX_ADDL 0xff

int CHANNEL_TX = 0x02;

// ---------- Arduino pins --------------
LoRa_E32 e32ttl(RX, TX, AUX, M0, M1);
// LoRa_E32 e32ttl(9, 10); // Config without connect AUX and M0 M1

// ---------- Raspberry PI Pico pins --------------
// LoRa_E32 e32ttl100(&Serial2, 2, 10, 11); //  RX AUX M0 M1
// -------------------------------------

void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(19200);
	while (!Serial) {
	    ; // wait for serial port to connect. Needed for native USB
    }
	delay(100);

	e32ttl.begin();
	// After set configuration comment set M0 and M1 to low
	// and reboot if you directly set HIGH M0 and M1 to program
	ResponseStructContainer c;
	c = e32ttl.getConfiguration();
	Configuration configuration = *(Configuration*) c.data;
	configuration.ADDL = RX_ADDL;
	configuration.ADDH = RX_ADDH;
	configuration.CHAN = CHANNEL_RX;
	configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
  configuration.OPTION.transmissionPower = POWER_20;
  configuration.SPED.uartBaudRate = UART_BPS_19200;
  // configuration.SPED.airDataRate = AIR_DATA_RATE_001_12;
  configuration.SPED.airDataRate = AIR_DATA_RATE_000_03;
  configuration.SPED.uartParity = MODE_01_8O1;
	e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
	printParameters(configuration);
	c.close();
	// ---------------------------
}
struct Message {
    char type[5];
    char message[8];
    int temperature;
    byte chan;
} message;

int i = 0;
String receiving_message = "";
String received_message = "";
bool message_received = false;

// The loop function is called in an endless loop
void loop()
{
	delay(2500);
  // delay(900);

  if(message_received){
    CHANNEL_TX = received_message.toInt();
    Serial.print("Channel set to ");
    Serial.println(CHANNEL_TX, HEX);
    message_received = false;
  }

	i++;
	// struct Message {
	//     char type[5] = "TEMP";
	//     char message[8] = "Kitchen";
	//     byte temperature[4];
  //     byte chan = CHANNEL_TX;
	// } message;

	// *(float*)(message.temperature) = 19.2;
  uint8_t message[] = {0x20};

	// ResponseStatus rs = e32ttl.sendFixedMessage(TX_ADDH,TX_ADDL,CHANNEL_TX,&message, sizeof(Message));
	ResponseStatus rs = e32ttl.sendFixedMessage(TX_ADDH,TX_ADDL,CHANNEL_TX,&message, sizeof(message));
  // delay(100);
  Serial.print(sizeof(message));
  Serial.print(" ");
	Serial.println(rs.getResponseDescription());
  // Serial.print(" ");
  // Serial.println(CHANNEL_TX);
  // CHANNEL_TX++;
  // if(CHANNEL_TX==0x20){
  //   CHANNEL_TX = 0;
  // }
}

void printParameters(struct Configuration configuration) {
	Serial.println("----------------------------------------");

	Serial.print(F("HEAD : "));  Serial.print(configuration.HEAD, BIN);Serial.print(" ");Serial.print(configuration.HEAD, DEC);Serial.print(" ");Serial.println(configuration.HEAD, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, BIN);
	Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, BIN);
	Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit     : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDatte  : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRate());
	Serial.print(F("SpeedAirDataRate   : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRate());

	Serial.print(F("OptionTrans        : "));  Serial.print(configuration.OPTION.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFixedTransmissionDescription());
	Serial.print(F("OptionPullup       : "));  Serial.print(configuration.OPTION.ioDriveMode, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getIODroveModeDescription());
	Serial.print(F("OptionWakeup       : "));  Serial.print(configuration.OPTION.wirelessWakeupTime, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription());
	Serial.print(F("OptionFEC          : "));  Serial.print(configuration.OPTION.fec, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFECDescription());
	Serial.print(F("OptionPower        : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());

	Serial.println("----------------------------------------");

}
void printModuleInformation(struct ModuleInformation moduleInformation) {
	Serial.println("----------------------------------------");
	Serial.print(F("HEAD BIN: "));  Serial.print(moduleInformation.HEAD, BIN);Serial.print(" ");Serial.print(moduleInformation.HEAD, DEC);Serial.print(" ");Serial.println(moduleInformation.HEAD, HEX);

	Serial.print(F("Freq.: "));  Serial.println(moduleInformation.frequency, HEX);
	Serial.print(F("Version  : "));  Serial.println(moduleInformation.version, HEX);
	Serial.print(F("Features : "));  Serial.println(moduleInformation.features, HEX);
	Serial.println("----------------------------------------");

}

void serialEvent(){
  char c;
  while(Serial.available() && !message_received){
    c = Serial.read();
    if(c == '\n'){
      message_received = true;
      received_message = receiving_message;
      receiving_message = "";
    } else{
      receiving_message += c;
    }
  }
}
