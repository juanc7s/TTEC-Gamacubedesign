#ifndef GS_COMM_H
#define GS_COMM_H

#include <EbyteLib.h>

extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;
extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;

extern unsigned int telemetry_index;
extern bool telemetry_received;

struct Greeting{
  uint8_t type;
  uint8_t message;
}; // sending a struct with multiple fields

struct Protocol{
  uint8_t type;
  uint8_t data;
};

struct Resend{
  uint8_t length;
  uint8_t packets[32];
};

struct Telemetry{
  uint8_t length;
  uint8_t index;
  uint8_t data[56];
};

// extern Message message;
extern Telemetry telemetry;

void sendCommand();
void listenForResponse(unsigned long int timeout);

void updateRFComm();

#endif