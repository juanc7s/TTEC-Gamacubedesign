#include "Logger.h"

const char* stf1 = "stf1.dat";
const char* imf1 = "imf1.dat";
const char* stf2 = "stf2.dat";
const char* imf2 = "imf2.dat";
bool st_state = false;
bool im_state = false;

// char* Logger::current_status_reading_file;
// char* Logger::current_status_writing_file;
// char* Logger::current_imaging_reading_file;
// char* Logger::current_imaging_writing_file;

unsigned long int reading_status_counter = 0;
unsigned long int reading_imaging_counter = 0;
unsigned long int writing_status_counter = 0;
unsigned long int writing_imaging_counter = 0;

// unsigned int Logger::reading_status_file_pointer;
// unsigned int Logger::reading_imaging_file_pointer;

Logger::Logger(){
  std::cout << "Initiating logger!" << std::endl;
  std::remove(stf1);
  std::remove(stf2);
  std::remove(imf1);
  std::remove(imf2);
  current_status_writing_file = (char*)stf2;
  current_imaging_writing_file = (char*)imf2;
}
void Logger::switch_status_file(){
  if(st_state){
    std::remove(stf2);
    current_status_writing_file = (char*)stf2;
    current_status_reading_file = (char*)stf1;
  } else{
    std::remove(stf1);
    current_status_writing_file = (char*)stf1;
    current_status_reading_file = (char*)stf2;
  }
  reading_status_counter = writing_status_counter;
  writing_status_counter = 0;
  st_state = !st_state;
}
void Logger::switch_imaging_file(){
  if(st_state){
    std::remove(imf2);
    current_imaging_writing_file = (char*)imf2;
    current_imaging_reading_file = (char*)imf1;
  } else{
    std::remove(imf1);
    current_imaging_writing_file = (char*)imf1;
    current_imaging_reading_file = (char*)imf2;
  }
  reading_imaging_counter = writing_imaging_counter;
  writing_imaging_counter = 0;
  im_state = !im_state;
}

void Logger::readSatStatusPacket(HealthData* healthData, unsigned int index){
  fstream f;
  f.open(current_status_reading_file, ios::in | ios::binary);
  f.seekg(index*sizeof(HealthData), ios::beg);
  for(unsigned int i = 0; i < sizeof(HealthData); i++){
    f >> ((uint8_t*)healthData)[i];
  }
  f.close();
}
void Logger::writeSatStatusPacket(HealthData he){
  fstream f;
  f.open(current_status_writing_file, ios::app | ios::binary);
  if(f){
    for(unsigned int i = 0; i < sizeof(HealthData); i++){
      // "0"
      f << ((uint8_t*)(&he))[i];
    }
    f.close();
    writing_status_counter++;
  } else{
    std::cout << "Error opening status file" << std::endl;
  }
}

void Logger::readSatImagingDataPacket(ImagingData* imagingData, unsigned int index){
  fstream f;
  f.open(current_imaging_reading_file, ios::in | ios::binary);
  if(f){
    f.seekg(index*sizeof(ImagingData), ios::beg);
    for(unsigned int i = 0; i < sizeof(ImagingData); i++){
      f >> ((uint8_t*)imagingData)[i];
    }
    f.close();
  } else{
    std::cout << "Error opening reading image file: " << current_imaging_reading_file << std::endl;
  }
}
void Logger::writeSatImagingDataPacket(ImagingData im){
  fstream f;
  f.open(current_imaging_writing_file, ios::app | ios::binary);
  if(f){
    for(unsigned int i = 0; i < sizeof(ImagingData); i++){
      // "0"
      // std::cout << ((uint8_t*)(&im))[i] << std::endl;
      f << ((uint8_t*)(&im))[i];
    }
    f.close();
    writing_imaging_counter++;
  } else{
    std::cout << "Error opening imaging file" << std::endl;
  }
}

void writeLoggers(Logger *logger){
  std::cout << "writing loggers" << std::endl;

  ImagingData im;
  for(int i = 0 ; i < 5; i++){
    im.lightnings[i].index = writing_imaging_counter;
    im.lightnings[i].duration = 5;
    im.lightnings[i].radius = 1;
    im.lightnings[i].x = i*10;
    im.lightnings[i].y = i*20;
  }
  logger->writeSatImagingDataPacket(im);

  HealthData he = {
    .index = writing_status_counter,
    .time = 1,
    .battery_voltage = 3.7,
    .battery_current = 2.0,
    .battery_charge = 100.0,
    .battery_temperature = 33.0,
    .internal_temperature = 37.9,
    .external_temperature = 51.2,
    .sd_memory_usage = (unsigned long)3,
    .rasp_data = {0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1},
  };
  logger->writeSatStatusPacket(he);
}

void printImagingPacket(Logger *logger){
  std::cout << "Reading imaging packet" << std::endl;
  if(reading_imaging_counter == 0){
    if(writing_imaging_counter > 0){
      std::cout << "Switching imaging file" << std::endl;
      logger->switch_imaging_file();
    } else{
      return;
    }
  }
  ImagingData im;
  while(reading_imaging_counter){
    im.lightnings[0].index = 0;
    logger->readSatImagingDataPacket(&im, reading_imaging_counter-1);
    std::cout << "lightning index " << im.lightnings[0].index << std::endl;
    reading_imaging_counter--;
  }
  std::cout << std::endl;
}

void printStatusPacket(Logger *logger){
  std::cout << "Reading status packet" << std::endl;
  if(reading_status_counter == 0){
    if(writing_status_counter > 0){
      std::cout << "Switching status file" << std::endl;
      logger->switch_status_file();
    } else{
      return;
    }
  }
  HealthData he;
  while(reading_status_counter){
    logger->readSatStatusPacket(&he, reading_status_counter-1);
    std::cout << "Sat packet index: " << he.index << std::endl;
    // std::cout << "Sat time: " << he.time << std::endl;
    // std::cout << "Sat bat charge: " << he.battery_charge << std::endl;
    // std::cout << "Sat bat current: " << he.battery_current << std::endl;
    // std::cout << "Sat bat temperature: " << he.battery_temperature << std::endl;
    // std::cout << "Sat bat voltage: " << he.battery_voltage << std::endl;
    // std::cout << "Sat ext temp: " << he.external_temperature << std::endl;
    // std::cout << "Sat int temp: " << he.internal_temperature << std::endl;
    // std::cout << "Sat mem usage: " << he.sd_memory_usage << std::endl;
    // for(int i = 0; i < 10; i++){
    //   std::cout << "Sat data: " << i << " " << (int)he.rasp_data[i] << std::endl;
    // }
    reading_status_counter--;
  }
  std::cout << std::endl;
}

int main(){
  Logger logger;
  for(int i = 0; i < 5; i++){
    writeLoggers(&logger);
  }
  while(reading_imaging_counter || writing_imaging_counter){
    printImagingPacket(&logger);
  }
  while(reading_status_counter || writing_status_counter){
    printStatusPacket(&logger);
  }
  for(int i = 0; i < 4; i++){
    writeLoggers(&logger);
  }
  while(reading_imaging_counter || writing_imaging_counter){
    printImagingPacket(&logger);
  }
  while(reading_status_counter || writing_status_counter){
    printStatusPacket(&logger);
  }
  return 0;
}