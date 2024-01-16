#include "Arduino.h"
#include "SDData.h"

const int chipSelect = 10;
const char* stf1 = "stf1.dat";
const char* imf1 = "imf1.dat";
const char* stf2 = "stf2.dat";
const char* imf2 = "imf2.dat";
bool st_state = false;
bool im_state = false;

File current_status_reading_file;
File current_status_writing_file;
File current_imaging_reading_file;
File current_imaging_writing_file;

unsigned long int reading_status_counter = 0;
unsigned long int reading_imaging_counter = 0;
unsigned long int writing_status_counter = 0;
unsigned long int writing_imaging_counter = 0;

unsigned int reading_status_file_pointer = 0;
unsigned int reading_imaging_file_pointer = 0;

void init_sd_logger(){
  if (!SD.begin(chipSelect)) {
    Serial.println("SD:F");
  } else{
    Serial.println("SD:S");
    current_status_writing_file = SD.open(stf2, 'w');
    current_imaging_writing_file = SD.open(imf2, 'w');
  }
}
void switch_status_file(){
  current_status_writing_file.close();
  if(st_state){
    if(current_status_reading_file){
      current_status_reading_file.close();
      SD.remove(stf2);
    }
    current_status_writing_file = SD.open(stf2, 'w');
    current_status_reading_file = SD.open(stf1, 'r');
  } else{
    if(current_status_reading_file){
      current_status_reading_file.close();
      SD.remove(stf1);
    }
    current_status_writing_file = SD.open(stf1, 'w');
    current_status_reading_file = SD.open(stf2, 'r');
  }
  st_state = !st_state;
}
void switch_imaging_file(){
  current_imaging_writing_file.close();
  if(st_state){
    if(current_imaging_reading_file){
      current_imaging_reading_file.close();
      SD.remove(stf1);
    }
    current_imaging_writing_file = SD.open(imf1, 'w');
    current_imaging_reading_file = SD.open(imf2, 'r');
  } else{
    if(current_imaging_reading_file){
      current_imaging_reading_file.close();
      SD.remove(imf2);
    }
    current_imaging_writing_file = SD.open(imf2, 'w');
    current_imaging_reading_file = SD.open(imf1, 'r');
  }
  im_state = !im_state;
}

bool sdReadSatStatusPacket(HealthData* healthData, unsigned int index){
  if(reading_status_counter==0){
    if(writing_status_counter>0){
      switch_status_file();
    } else{
      return false;
    }
  }
  healthData->battery_voltage = 3.3;
  healthData->battery_current = 50.0;
  healthData->battery_charge = -10.0;
  healthData->battery_temperature = index*1000.8;
  healthData->internal_temperature = 3.3;
  healthData->external_temperature = 3.3;
  healthData->sd_memory_usage = 50.0;
  for(uint8_t i = 0; i < 10; i++){
    healthData->rasp_data[i] = 0xa1;
  }
  return true;
}
void sdWriteSatStatusPacket(unsigned long int t,
  float bv, float bcu, float bch, float bt, float ti, float te,
  unsigned long int sd, uint8_t* rasp_data){
  HealthData st;
  st.time = t;
  st.battery_voltage = bv;
  st.battery_current = bcu;
  st.battery_charge = bch;
  st.battery_temperature = bt;
  st.internal_temperature = ti;
  st.external_temperature = te;
  st.sd_memory_usage = sd;
  for(unsigned int i = 0; i < 10; i++){
    st.rasp_data[i] = rasp_data[i];
  }
  for(unsigned int i = 0; i < sizeof(HealthData); i++){
    current_status_writing_file.write(((uint8_t*)(&st))[i]);
  }
  writing_status_counter++;
}

bool sdReadSatImagingDataPacket(ImagingData* imagingData, unsigned int index){
  if(reading_imaging_counter==0){
    if(writing_imaging_counter>0){
      switch_imaging_file();
    } else{
      return false;
    }
  }
  for(uint8_t i = 0; i < 5; i++){
    imagingData->lightnings[i].x = index*10 - i;
    imagingData->lightnings[i].y = index*7 + i;
    imagingData->lightnings[i].radius = 20;
    imagingData->lightnings[i].duration = i;
  }
  return true;
}
void sdWriteSatImagingDataPacket(){
  writing_imaging_counter++;
}