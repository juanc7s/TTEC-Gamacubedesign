/*
 * LoRa E32-TTL-100
 * Receive fixed transmission message on channel.
 * https://www.mischianti.org/2019/12/03/lora-e32-device-for-arduino-esp32-or-esp8266-power-saving-and-sending-structured-data-part-5/
 *
 * E32-TTL-100----- Arduino UNO or esp8266
 * M0         ----- 3.3v (To config) GND (To send) 7 (To dinamically manage)
 * M1         ----- 3.3v (To config) GND (To send) 6 (To dinamically manage)
 * TX         ----- MTX_E32RX PIN 2 (PullUP)
 * MTX_E32RX         ----- TX PIN 3 (PullUP & Voltage divider)
 * AUX        ----- Not connected (5 if you connect)
 * VCC        ----- 3.3v/5v
 * GND        ----- GND
 *
 */
#include "Arduino.h"
#include "LoRa_E32.h"

// ---------- esp8266 pins --------------
//LoRa_E32 e32ttl(D2, D3, D5, D7, D6);
//LoRa_E32 e32ttl(D2, D3); // Config without connect AUX and M0 M1

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(D2, D3); // Arduino MTX_E32RX <-- e32 TX, Arduino TX --> e32 MTX_E32RX
//LoRa_E32 e32ttl(&mySerial, D5, D7, D6);
// -------------------------------------

#define MTX_E32RX 2
#define TX 3
#define AUX 5
#define M0 7
#define M1 6

#define CHANNEL_TX 0x02
#define CHANNEL_RX 0x02
#define RX_ADDH 0xff
#define RX_ADDL 0xff
#define TX_ADDH 0xff
#define TX_ADDL 0xff

// ---------- Arduino pins --------------
LoRa_E32 e32ttl(MTX_E32RX, TX, AUX, M0, M1);
// LoRa_E32 e32ttl(MTX_E32RX, TX, 4); // Config without connect AUX and M0 M1

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(2, 3); // Arduino MTX_E32RX <-- e32 TX, Arduino TX --> e32 MTX_E32RX
//LoRa_E32 e32ttl(&mySerial, 5, 7, 6);
// -------------------------------------

// ---------------- STM32 --------------------
// HardwareSerial Serial2(USART2);   // PA3  (MTX_E32RX)  PA2  (TX)
// LoRa_E32 e32ttl(&Serial2, PA0, PB0, PB10); //  MTX_E32RX AUX M0 M1
// -------------------------------------------------

// ---------- Raspberry PI Pico pins --------------
// LoRa_E32 e32ttl100(&Serial2, 2, 10, 11); //  MTX_E32RX AUX M0 M1
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
  // pinMode(MTX_E32RX, INPUT_PULLUP);

//	e32ttl.resetModule();
	// After set configuration comment set M0 and M1 to low
	// and reboot if you directly set HIGH M0 and M1 to program
	ResponseStructContainer c;
	c = e32ttl.getConfiguration();
	Configuration configuration = *(Configuration*) c.data;
	configuration.ADDH = RX_ADDH;
	configuration.ADDL = RX_ADDL;
	configuration.CHAN = CHANNEL_RX;
	configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
  configuration.SPED.uartBaudRate = UART_BPS_19200;
  configuration.SPED.airDataRate = AIR_DATA_RATE_011_48;
  configuration.SPED.uartParity = MODE_01_8O1;
	e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
	printParameters(configuration);
	c.close();
	// ---------------------------
	Serial.println();
	Serial.println("Start listening!");
    e32ttl.setMode(MODE_2_POWER_SAVING);

}
struct Message {
    char type[5];
    char message[8];
    byte temperature[4];
    byte chan;
};

// The loop function is called in an endless loop
void loop()
{
	if (e32ttl.available()  > 1){

		ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
		struct Message message = *(Message*) rsc.data;
		Serial.println(message.type);

		Serial.println(*(float*)(message.temperature));
		Serial.println(message.message);
    Serial.println(message.chan);
//		free(rsc.data);
		rsc.close();
	}
}

void printParameters(struct Configuration configuration) {
	Serial.println("----------------------------------------");

	Serial.print(F("HEAD : "));  Serial.print(configuration.HEAD, BIN);Serial.print(" ");Serial.print(configuration.HEAD, DEC);Serial.print(" ");Serial.println(configuration.HEAD, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, DEC);
	Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, DEC);
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
