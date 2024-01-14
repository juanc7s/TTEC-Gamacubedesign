#ifndef SDDATA_H
#define SDDATA_H

#include <SPI.h
#include <SD.h>
#include <ArduinoJson.h>

extern const int chipSelect;

extern File current_status_reading_file;
extern File current_status_writing_file;
extern File current_imaging_data_reading_file;
extern File current_imaging_data_writing_file;

void init_sd_logger();

bool sdReadSatStatusPacket();
void sdWriteSatStatusPacket();

bool sdReadSatImagingDataPacket();
void sdWriteSatImagingDataPacket();

#endif