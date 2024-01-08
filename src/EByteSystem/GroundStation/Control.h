#ifndef CONTROL_H
#define CONTROL_H

#include <EbyteLib.h>

extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;
extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;

// extern String received_serial;
// extern String receiving_serial;
// extern bool serial_received;

extern bool state_sending;
extern uint8_t send_tx;
extern unsigned int telemetry_index;
extern unsigned int message_data_index;

extern void (*parsing_function)(uint8_t c);

enum CONTROL_COMMAND{
  READ_HEAD,
  READ_ADDH,
  READ_ADDL,
  READ_CHAN,
  READ_AIR_DATA_RATE,
  READ_UART_BAUD_RATE,
  READ_PARITY,
  READ_TRANSMISSION_POWER,
  READ_TRANSMISSION_MODE,
  READ_OPERATION_MODE,
  READ_ALL,

  SET_HEAD,
  SET_ADDH,
  SET_ADDL,
  SET_CHAN,
  SET_AIR_DATA_RATE,
  SET_UART_BAUD_RATE,
  SET_PARITY,
  SET_TRANSMISSION_POWER,
  SET_TRANSMISSION_MODE,
  SET_OPERATION_MODE,
  SET_ALL,

  TOGGLE_TX,
  SEND_TX,

  SET_TX_ADDH,
  SET_TX_ADDL,
  SET_TX_CHAN,

  FLUSH,
  SET_MESSAGE_DATA
};

void checkControl();
void decodeSerial();
void parseSerial(uint8_t c);

void control_toggleTx(uint8_t);
void control_setTxChannel(uint8_t);
void control_setTxAddh(uint8_t);
void control_setTxAddl(uint8_t);
void control_setAddh(uint8_t c);
void control_setAddl(uint8_t c);
void control_setChannel(uint8_t c);
void control_setParity(uint8_t c);
void control_setUARTBaudRate(uint8_t c);
void control_setAirDataRate(uint8_t c);
void control_setTxPower(uint8_t c);
void control_setTxMode(uint8_t c);
void control_setOperationMode(uint8_t c);

#endif