#include "Arduino.h"
#include "SDData.h"

const int chipSelect = 6;

File current_status_reading_file;
File current_status_writing_file;
File current_imaging_data_reading_file;
File current_imaging_data_writing_file;

void init_sd_logger(){
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
  } else{
    Serial.println("card initialized.");
  }
}

bool sdReadSatStatusPacket(){

}
void sdWriteSatStatusPacket(){
  if(!current_status_writing_file){
    current_status_writing_file = SD.open("stwr.dat");
  }
  JsonDocument doc;
  doc["time"] = millis();
  doc["bv"] = 3.3;
  doc["bcu"] = 1.0;
  doc["bch"] = 1.1;
  doc["bt"] = 30.0;
  doc["ti"] = 40.0;
  doc["te"] = 50.0;
  doc["sd"] = 1000.0;
  JsonArray data = doc["rd"].to<JsonArray>();
  data.add(0x01);
  data.add(0x02);
  data.add(0x03);
  data.add(0x04);
  data.add(0x05);
  data.add(0x06);
  data.add(0x07);
  data.add(0x08);
  data.add(0x09);
  data.add(0x0a);
  serualizeJson(doc, current_status_writing_file);
}

bool sdReadSatImagingDataPacket(){

}
void sdWriteSatImagingDataPacket(){

}