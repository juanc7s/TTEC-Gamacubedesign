#ifndef CONTROL_H
#define CONTROL_H

extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;
extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;

extern String received_serial;
extern String receiving_serial;
extern bool serial_received;

extern bool state_sending;
extern unsigned int telemetry_index;

void checkControl();

#endif