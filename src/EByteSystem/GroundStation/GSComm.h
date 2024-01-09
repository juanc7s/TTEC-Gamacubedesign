#ifndef GS_COMM_H
#define GS_COMM_H

#include <EbyteLib.h>

extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;
extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;

enum PROTOCOL{
  PROTOCOL_GREETING,
  PROTOCOL_START_TELEMETRY_TRANSMISSION,
  PROTOCOL_RESEND_STATUS,
  SATELLITE_PACKETS_AVAILABLE
};

struct TxCommand{
  unsigned int length;
  bool actuator_1;
  bool actuator_2;
  bool actuator_3;
  bool actuator_4;
}; // sending a struct with multiple fields

#define N 10 // Size of the added array
struct TelemetryData{
  uint8_t length;
  uint8_t data[N];
};

struct TxResend{
  bool isDone;
  uint8_t packets[32];
};

struct TelemetryPacket{
  uint8_t index;
  uint8_t data[56];
};

union RxData{
  uint8_t numberOfPackets;
  TelemetryPacket telemetryPacket;
};

struct RxPacket{
  uint8_t length;
  uint8_t operation;
  RxData data;
};

union TxData{
  TxCommand cmd;
  TxResend resed;
};

struct TxPacket{
  uint8_t length;
  uint8_t operation;
  TxData data;
};

// extern Message message;
extern TelemetryData telemetryData;
extern RxPacket rxPacket;
extern TxPacket txPacket;

void sendCommand();
bool listenForResponse(unsigned long int timeout);

void updateRFComm(uint8_t* received_buffer, unsigned int size);

#endif