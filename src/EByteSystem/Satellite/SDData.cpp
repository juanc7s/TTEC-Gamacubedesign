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

void init_sd_logger(){
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
  } else{
    Serial.println("card initialized.");
  }
  current_status_writing_file = SD.open(stf2, 'w');
  current_imaging_writing_file = SD.open(imf2, 'w');
}
void switch_status_file(){
  current_status_writing_file.close();
  if(st_state){
    if(current_status_reading_file){
      current_status_reading_file.close();
      SD.remove(stf1);
    }
    current_status_writing_file = SD.open(stf1, 'w');
    current_status_reading_file = SD.open(stf2, 'r');
  } else{
    if(current_status_reading_file){
      current_status_reading_file.close();
      SD.remove(stf2);
    }
    current_status_writing_file = SD.open(stf2, 'w');
    current_status_reading_file = SD.open(stf1, 'r');
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

bool sdReadSatStatusPacket(){
  if(reading_status_counter==0){
    if(writing_status_counter>0){
      switch_status_file();
    } else{
      return false;
    }
  }

  return true;
}
void sdWriteSatStatusPacket(unsigned long int t,
  float bv, float bcu, float bch, float bt, float ti, float te,
  unsigned long int sd, uint8_t* rasp_data){
  JsonDocument doc;
  doc["time"] = t;
  doc["bv"] = bv;
  doc["bcu"] = bcu;
  doc["bch"] = bch;
  doc["bt"] = bt;
  doc["ti"] = ti;
  doc["te"] = te;
  doc["sd"] = sd;
  JsonArray data = doc["rd"].to<JsonArray>();
  for(unsigned int i = 0; i < 10; i++){
    data.add(rasp_data[i]);
  }
  serializeJson(doc, current_status_writing_file);
  writing_status_counter++;
}

bool sdReadSatImagingDataPacket(){
  if(reading_imaging_counter==0){
    if(writing_imaging_counter>0){
      switch_imaging_file();
    } else{
      return false;
    }
  }

  return true;
}
void sdWriteSatImagingDataPacket(){

}