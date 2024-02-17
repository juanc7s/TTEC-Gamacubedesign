#ifndef SD_DATA_H
#define SD_DATA_H

#include <iostream>
#include <fstream>

using namespace std;

#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"
// #include "RFModule.h"

extern unsigned long int reading_status_counter;
extern unsigned long int reading_imaging_counter;
extern unsigned long int writing_status_counter;
extern unsigned long int writing_imaging_counter;

class Logger{
  public:
    char* current_status_reading_file;
    char* current_status_writing_file;
    char* current_imaging_reading_file;
    char* current_imaging_writing_file;

    unsigned int reading_status_file_pointer = 0;
    unsigned int reading_imaging_file_pointer = 0;

    Logger();

    void switch_status_file();
    void switch_imaging_file();

    void readSatStatusPacket(HealthData *healthData, unsigned int index);
    void writeSatStatusPacket(HealthData he);

    void readSatImagingDataPacket(ImagingData *imagingData, unsigned int index);
    void writeSatImagingDataPacket(ImagingData im);
};

extern Logger logger;

#endif