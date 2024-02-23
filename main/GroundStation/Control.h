#ifndef CONTROL_H
#define CONTROL_H

#include <SPI.h>              // include libraries
#include <LoRa.h>

#include "GSComm.h"
// #include "C:\Users\Vitinho\Desktop\TTEC-Gamacubedesign\main\CommunicationProtocol.h"
#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"
// #include "../CommunicationProtocol.h"

extern const char CONTROL_STR[];
extern const char PRINT_STR[];
enum PRINT_ENUM{
  PRINT_MESSAGE,
  PRINT_STATUS_PACKET,
  PRINT_IMAGING_PACKET,
  PRINT_STATUS_RECEIVED,
  PRINT_IMAGING_RECEIVED,
  PRINT_PACKET_INFO
};
enum CONTROL_ENUM{
  PRINT_DEVICE_READY,
  CONTROL_RX_ADDH,
  CONTROL_RX_ADDL,
  CONTROL_TX_ADDH,
  CONTROL_TX_ADDL,
  CONTROL_BANDWIDTH,
  CONTROL_FREQUENCY,
  CONTROL_SPI_FREQUENCY,
  CONTROL_SPREADING_FACTOR,
  CONTROL_TXPW,
  PRINT_LORA_INIT_FAILED
};

void printSeparator();
void printPrint();
void printPrint(PRINT_ENUM cmd);
void printPrintln(PRINT_ENUM cmd);
void printControl();
void printControl(CONTROL_ENUM cmd);
void printControlln(CONTROL_ENUM cmd);

void printBIN(uint8_t *buf, unsigned int size);

extern uint8_t txAddh;
extern uint8_t txAddl;
// extern uint8_t txChan;
extern uint8_t rxAddh;
extern uint8_t rxAddl;
// extern uint8_t rxChan;
extern uint8_t bandwidth;
extern unsigned long spi_frequency;
extern unsigned long frequency;
extern uint8_t spreading_factor;
extern uint8_t tx_power;

extern bool state_request_status;
extern uint8_t send_request_status;
extern bool state_request_imaging;
extern uint8_t send_request_imaging;
extern bool state_command;
extern uint8_t send_command;

extern unsigned int telemetry_index;
extern unsigned int message_data_index;

extern void (*parsing_function)(uint8_t);

void checkControl();
void decodeSerial();
void parseSerial(uint8_t);
void parse_number(uint8_t);

void control_setRxAddh(uint8_t);
void control_setRxAddl(uint8_t);
void control_setTxAddh(uint8_t);
void control_setTxAddl(uint8_t);
void control_setBandwidth(uint8_t);
void control_setFrequency();
void control_setSPIFrequency();
void control_setSpreadingFactor(uint8_t);
void control_setTxPower(uint8_t);

void control_toggleQueryStatus(uint8_t);
void control_toggleQueryImaging(uint8_t);
void control_toggleCommand(uint8_t);

void setActiveThermalControl(uint8_t);
void setAttitudeControl(uint8_t);
void setImaging(uint8_t);
void setImagingMode(uint8_t);
void setStandByMode(uint8_t);

void control_print_status_packet();
void control_print_imaging_packet();
void control_print_packet_info();

#endif