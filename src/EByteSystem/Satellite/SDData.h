#ifndef SDDATA_H
#define SDDATA_H

#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>

extern const int chipSelect;

extern File current_status_reading_file;
extern File current_status_writing_file;
extern File current_imaging_data_reading_file;
extern File current_imaging_data_writing_file;

void init_sd_logger();

bool sdReadSatStatusPacket();
void sdWriteSatStatusPacket(unsigned long int t,
  float bv, float bcu, float bch, float bt, float ti, float te,
  unsigned long int sd, uint8_t* rasp_data);

bool sdReadSatImagingDataPacket();
void sdWriteSatImagingDataPacket();

#endif