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

void sendNumberOfPackets();
void sendPacket(unsigned int packet_index);
void sendDone();

#endif