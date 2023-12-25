#ifndef BitCubeSatProtocol_h
#define BitCubeSatProtocol_h

#include "Arduino.h"
#include <LoRa.h>
#include <ArduinoJson.h>

class BitCubeSatProtocol{
  public:
    // BitCubeSatProtocol();
    void begin(long int frequency);
    void main_loop();
    void sendTelemetry();
    void getCommand();
    bool isCommandReceived();

    struct Telemetry {
        char type[15] = "Telemetry Data";
        const char message_1[13] = "Hello There!";
        float main_temperature = 150.5;
        const char message_2[16] = "General Kenobi!";
        float batt_voltage = 72.0;
        float batt_current = 1000.1;
        float batt_charge = -20.3;
        float batt_temperature = -50.0;
        int index = 0;
        long unsigned int time = 0;
    } telemetry;

    struct Command {
      bool cmd_1 = false;
      bool cmd_2 = false;
      bool cmd_3 = false;
    } command;

    int counter = 0;

    // // See https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
    // struct Configuration {
    //   long int frequency; // Frequency in Hz (433e6, 868E6, 915E6)
    //   long int BW; // Signal Bandwidth (7.8e3, 10.4e3, 15.6e3, 20.8e3, 31.25e3, 41.7e3, 62.5e3, [125e3], 250e3, 500e3)
    //   uint8_t txPower; // Transmission power from 2 to 20 dBm
    //   uint8_t SF; // Spreading Factor
    //   uint8_t CR; // Coding rate
    //   uint PL; // Preamble Length
    //   uint8_t SW; // Sync Word
    //   bool CRC; // Enable or disable CRC usage
    //   bool IIQS; // Invert IQ Signals
    //   uint8_t LNAGain; // LNA Gain
    // } config;
  
  private:
    StaticJsonDocument<200> telemetry_doc;
    long unsigned int frequency;
    bool message_received;
    void prepareTelemetryToLoRa();
    String receiving_command;
    String received_command;
    String sending_telemetry;
};

#endif
