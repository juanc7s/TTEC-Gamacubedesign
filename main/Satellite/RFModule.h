#ifndef RF_MODULE_H
#define RF_MODULE_H

#define DEBUGGING
#ifdef DEBUGGING
#define DBG_Print(x) std::cout << x
#define DBG_Println(x) std::cout << x << std::endl
#else
#define DBG_Print(x)
#define DBG_Println(x)
#endif

#include <cstdint>

#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"
#include "RFModemOffline.h"
#include "Timing.h"
#include "Logger.h"

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

// extern unsigned long int reading_status_counter;
// extern unsigned long int reading_imaging_counter;
// extern unsigned long int writing_status_counter;
// extern unsigned long int writing_imaging_counter;

// extern unsigned int reading_status_file_pointer;
// extern unsigned int reading_imaging_file_pointer;

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


void switch_status_file();
void switch_imaging_file();

// void readSatStatusPacket(HealthData *healthData, unsigned int index);
// void writeSatStatusPacket(unsigned long int t,
//   float bv, float bcu, float bch, float bt, float ti, float te,
//   unsigned long int sd, uint8_t* rasp_data);

// void readSatImagingDataPacket(ImagingData *imagingData, unsigned int index);
// void writeSatImagingDataPacket();

#endif