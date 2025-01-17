#ifndef GS_COMM_H
#define GS_COMM_H

#include <EbyteLib.h>
// #include "C:\Users\Vitinho\Desktop\TTEC-Gamacubedesign\src\EByteSystem\CommunicationProtocol.h"
#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/testing/system/Arduino/EByteSystem/CommunicationProtocol.h"

extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;
extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;

// extern bool switch_active_thermal_control;
// extern bool switch_attitude_control;
// extern bool switch_imaging;
// extern bool switch_imaging_mode;
// extern bool switch_stand_by_mode;

// extern Message message;
// extern TelemetryData telemetryData;
// extern SatPacket satPacket;
// extern GSPacket gsPacket;

// extern uint8_t rx_pointer;

void sendCommand();

void sendGSPacket();
bool listenForResponse(unsigned long int timeout);

void updateRFComm();
void onReceive();

void startRequestStatusProtocol();
void startRequestImagingDataProtocol();
void startSetOperationProtocol();

void switchCaseStatusProtocol();
void switchCaseImagingDataProtocol();
void switchCaseSetOperationProtocol();

#endif