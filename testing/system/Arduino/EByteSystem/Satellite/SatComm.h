#ifndef SAT_COMM_H
#define SAT_COMM_H

#ifdef DEBUGGING
#define DBG_Print(x) Serial.print(x)
#define DBG_Println(x) Serial.println(x)
#else
#define DBG_Print(x)
#define DBG_Println(x)
#endif

// #include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/libraries/EbyteLib/EbyteLib.h"
#include <EbyteLib.h>

// #include "C:\Users\Vitinho\Desktop\TTEC-Gamacubedesign\src\EByteSystem\CommunicationProtocol.h"
#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/src/EByteSystem/CommunicationProtocol.h"
// #include "SDData.h"

extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;
extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;

extern uint8_t rx_pointer;
extern SatPacket satPacket;
extern GSPacket gsPacket;

extern uint8_t number_of_packets;
extern const uint8_t N_status;
extern const uint8_t N_imaging;

extern unsigned long int reading_status_counter;
extern unsigned long int reading_imaging_counter;
extern unsigned long int writing_status_counter;
extern unsigned long int writing_imaging_counter;

extern unsigned int reading_status_file_pointer;
extern unsigned int reading_imaging_file_pointer;

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


void switch_status_file();
void switch_imaging_file();

void sdReadSatStatusPacket(HealthData *healthData, unsigned int index);
void sdWriteSatStatusPacket(unsigned long int t,
  float bv, float bcu, float bch, float bt, float ti, float te,
  unsigned long int sd, uint8_t* rasp_data);

void sdReadSatImagingDataPacket(ImagingData *imagingData, unsigned int index);
void sdWriteSatImagingDataPacket();

#endif