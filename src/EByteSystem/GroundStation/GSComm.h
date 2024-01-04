#ifndef GS_COMM_H
#define GS_COMM_H

extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;
extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;

extern unsigned int telemetry_index;
extern bool telemetry_received;

#define N 255 // Size of the added array (+1 for an end of line char)  
struct Message{
  unsigned int length;
  char type[10];
  char message_1[13];
  bool actuator_1;
  bool actuator_2;
  bool actuator_3;
  bool actuator_4;
}; // sending a struct with multiple fields

struct Telemetry{
  unsigned int length;
  char type[10];
  char telemetry_message[16];
  bool instrument_1;
  bool instrument_2;
  bool instrument_3;
  bool instrument_4;
  uint8_t data[N+1];
};

extern Message message;
extern Telemetry telemetry;

void sendCommand();
void listenForResponse(unsigned long int timeout);

void updateRFComm();

#endif