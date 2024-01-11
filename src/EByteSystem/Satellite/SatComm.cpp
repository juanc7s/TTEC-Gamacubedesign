#include "Arduino.h"
#include "SatComm.h"

uint8_t rxChan = 23;
uint8_t rxAddh = 0xa1;
uint8_t rxAddl = 0x06;
uint8_t txChan = 10;
uint8_t txAddh = 0x8f;
uint8_t txAddl = 0xf7;

bool talking = false;
unsigned long int communication_timeout;
const unsigned long int communication_timeout_limit = 1000;
bool telemetry_received = false;
uint8_t telemetry_state = 0;

bool switch_active_thermal_control = false;
bool switch_attitude_control = false;
bool switch_imaging = false;
bool switch_imaging_mode = false;
bool switch_stand_by_mode = false;

// TelemetryData telemetryData = {
//   .length = sizeof(TelemetryData),
//   .data = {0xa1, 0xa1}
// };
SatPacket satPacket;
GSPacket gsPacket;

uint8_t rx_pointer = 0;

void sendTelemetry(){
  Serial.print("Sending packet of length ");
  Serial.println(satPacket.length);
  // asynchronousWriteFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&telemetry, sizeof(telemetry));
  writeFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&satPacket, satPacket.length);
}

void updateRFComm(){
  uint8_t b;
  while(e32serial.available()){
    b = e32serial.read();
    Serial.println(b);
    ((uint8_t*)(&gsPacket))[rx_pointer++] = b;
    if(rx_pointer>0 && rx_pointer==gsPacket.length){
      onReceive();
      rx_pointer = 0;
    }
    if(rx_pointer >= sizeof(satPacket)){
      rx_pointer = 0;
    }
  }
}

void onReceive(){
  Serial.print("Received packet with length ");
  Serial.println(gsPacket.length);
  switch(gsPacket.operation){
    case PROTOCOL_GREETING:
      Serial.println("Greeting");
      sendNumberOfPackets();
      break;
    case PROTOCOL_START_TELEMETRY_TRANSMISSION:
      Serial.println("Start transmission");
      for(unsigned int i = 0; i < 100; i++){
        sendPacket(i);
      }
      Serial.println("Protocol done");
      sendDone();
      break;
    case PROTOCOL_RESEND_STATUS:
      Serial.println("Resend packet");
      if(!gsPacket.data.resend.isDone){
        Serial.println("Recalling packets");
        for(unsigned int i = 0; i < 32; i++){
          for(unsigned int j = 0; j < 8; j++){
            if(bitRead(gsPacket.data.resend.packets[i], j)){
              Serial.print("Resending packet ");
              Serial.println(i*8+j);
              sendPacket(i*8 + j);
            }
          }
        }
      } else{
        Serial.println("All packets transmitted");
      }
      Serial.println("Protocol done");
      sendDone();
      break;
    case PROTOCOL_DONE:
      Serial.println("Protocol done");
      // sendDone();
  }
}

void sendNumberOfPackets(){
  satPacket.length = 3;
  satPacket.operation = SATELLITE_PACKETS_AVAILABLE;
  satPacket.data.numberOfPackets = 1;
  sendTelemetry();
}

void sendPacket(unsigned int packet_index){
  satPacket.operation = SATELLITE_PACKET;
  satPacket.index = packet_index;
  satPacket.length = 10;
  sendTelemetry();
}

void sendDone(){
  satPacket.length = 2;
  satPacket.operation = SATELLITE_DONE;
  sendTelemetry();
}