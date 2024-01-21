#ifndef SD_DATA_H
#define SD_DATA_H

#include <SPI.h>
#include <SD.h>
// #include <ArduinoJson.h>

#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/src/EByteSystem/CommunicationProtocol.h"
#include "SatComm.h"

extern const int sd_chipSelect;

extern char* current_status_reading_file;
extern char* current_status_writing_file;
extern char* current_imaging_data_reading_file;
extern char* current_imaging_data_writing_file;

extern unsigned int reading_status_file_pointer;
extern unsigned int reading_imaging_file_pointer;

void init_sd_logger();

void sdReadSatStatusPacket(HealthData *healthData, unsigned int index);
void sdWriteSatStatusPacket(unsigned long int t,
  float bv, float bcu, float bch, float bt, float ti, float te,
  unsigned long int sd, uint8_t* rasp_data);

void sdReadSatImagingDataPacket(ImagingData *imagingData, unsigned int index);
void sdWriteSatImagingDataPacket(ImagingData im);

#endif