#ifndef SAT_COMM_H
#define SAT_COMM_H

#include <EbyteLib.h>
#include "C:\Users\Vitinho\Desktop\TTEC-Gamacubedesign\src\EByteSystem\CommunicationProtocol.h"

extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;
extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;

extern uint8_t rx_pointer;

void sendSatPacket();

void updateRFComm();
void onReceive();

void preStatusProtocol();
void updateStatusPacket(uint8_t index);
void postStatusProtocol();
void preImagingDataProtocol();
void updateImagingDataPacket(uint8_t index);
void postImagingDataProtocol();

void switchCaseStatusProtocol();
void switchCaseImagingDataProtocol();
void switchCaseSetOperationProtocol();

void sendNumberOfPackets();
void sendPacket(unsigned int packet_index, uint8_t protocol, uint8_t operation);
void sendDone(uint8_t protocol, uint8_t operation);

#endif