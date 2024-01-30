#ifndef CONTROL_H
#define CONTROL_H

#include <SPI.h>              // include libraries
#include <LoRa.h>
#include "GSComm.h"
#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"

extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;
extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;

// extern String received_serial;
// extern String receiving_serial;
// extern bool serial_received;

extern bool state_request_status;
extern uint8_t send_request_status;
extern bool state_request_imaging;
extern uint8_t send_request_imaging;
extern bool state_command;
extern uint8_t send_command;

extern unsigned int telemetry_index;
extern unsigned int message_data_index;

extern void (*parsing_function)(uint8_t c);

void checkControl();
void decodeSerial();
void parseSerial(uint8_t c);

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

void control_toggleQueryStatus(uint8_t);
void control_toggleQueryImaging(uint8_t);
void control_toggleCommand(uint8_t);

void setActiveThermalControl(uint8_t);
void setAttitudeControl(uint8_t);
void setImaging(uint8_t);
void setImagingMode(uint8_t);
void setStandByMode(uint8_t);

#endif