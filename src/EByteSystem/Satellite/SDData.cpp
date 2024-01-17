#include "Arduino.h"
#include "SDData.h"

const int sd_chipSelect = 10;
const char* stf1 = "stf1.dat";
const char* imf1 = "imf1.dat";
const char* stf2 = "stf2.dat";
const char* imf2 = "imf2.dat";
bool st_state = false;
bool im_state = false;

char* current_status_reading_file;
char* current_status_writing_file;
char* current_imaging_reading_file;
char* current_imaging_writing_file;

unsigned long int reading_status_counter = 0;
unsigned long int reading_imaging_counter = 0;
unsigned long int writing_status_counter = 0;
unsigned long int writing_imaging_counter = 0;

unsigned int reading_status_file_pointer = 0;
unsigned int reading_imaging_file_pointer = 0;

bool SD_on = false;

void init_sd_logger(){
  if (!SD.begin(sd_chipSelect)) {
    Serial.println("SD:F");
    while(1);
  } else{
    Serial.println("SD:S");
    SD_on = true;
    current_status_writing_file = stf2;
    current_imaging_writing_file = imf2;
  }
}
void switch_status_file(){
  if(st_state){
    if(current_status_reading_file){
      SD.remove(stf2);
    }
    current_status_writing_file = stf2;
    current_status_reading_file = stf1;
  } else{
    if(current_status_reading_file){
      SD.remove(stf1);
    }
    current_status_writing_file = stf1;
    current_status_reading_file = stf2;
  }
  reading_status_counter = writing_status_counter;
  writing_status_counter = 0;
  st_state = !st_state;
}
void switch_imaging_file(){
  if(st_state){
    if(current_imaging_reading_file){
      SD.remove(stf1);
    }
    current_imaging_writing_file = imf1;
    current_imaging_reading_file = imf2;
  } else{
    if(current_imaging_reading_file){
      SD.remove(imf2);
    }
    current_imaging_writing_file = imf2;
    current_imaging_reading_file = imf1;
  }
  reading_imaging_counter = writing_imaging_counter;
  writing_imaging_counter = 0;
  im_state = !im_state;
}

void sdReadSatStatusPacket(HealthData* healthData, unsigned int index){
  File f = SD.open(current_status_reading_file, FILE_READ);
  f.seek(index*sizeof(HealthData));
  for(unsigned int i = 0; i < sizeof(HealthData); i++){
    ((uint8_t*)healthData)[i] = f.read();
  }
  f.close();
}
void sdWriteSatStatusPacket(unsigned long int t,
  float bv, float bcu, float bch, float bt, float ti, float te,
  unsigned long int sd, uint8_t* rasp_data){
  File f = SD.open(current_status_writing_file, FILE_WRITE);
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
    // "0"
    f.write(((uint8_t*)(&st))[i]);
  }
  f.close();
  writing_status_counter++;
}

void sdReadSatImagingDataPacket(ImagingData* imagingData, unsigned int index){
  File f = SD.open(current_imaging_reading_file, FILE_READ);
  for(uint8_t i = 0; i < 5; i++){
    imagingData->lightnings[i].x = index*10 - i;
    imagingData->lightnings[i].y = index*7 + i;
    imagingData->lightnings[i].radius = 20;
    imagingData->lightnings[i].duration = i;
  }
  f.close();
}
void sdWriteSatImagingDataPacket(){
  writing_imaging_counter++;
}