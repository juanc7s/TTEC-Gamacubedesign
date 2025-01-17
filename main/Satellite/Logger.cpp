#include "Logger.h"

const char* stf1 = "stf1.dat";
const char* imf1 = "imf1.dat";
const char* stf2 = "stf2.dat";
const char* imf2 = "imf2.dat";
bool st_state = false;
bool im_state = false;

unsigned long int reading_status_counter = 0;
unsigned long int reading_imaging_counter = 0;
unsigned long int writing_status_counter = 0;
unsigned long int writing_imaging_counter = 0;

Logger logger;

Logger::Logger(){
  std::cout << "Initiating logger!" << std::endl;
  std::remove(stf1);
  std::remove(stf2);
  std::remove(imf1);
  std::remove(imf2);
  current_status_writing_file = (char*)stf2;
  current_imaging_writing_file = (char*)imf2;
  std::cout << "Logger initiated successfully\n";
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
  if(im_state){
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
  f.open(current_status_reading_file, ios::in);
  if(f){
    // f.seekg(index*sizeof(HealthData), ios::beg);
    int b;
    for(unsigned int i = 0; i < index*sizeof(HealthData); i++){
      f >> b;
    }
    for(unsigned int i = 0; i < sizeof(HealthData); i++){
      f >> b;
      ((uint8_t*)healthData)[i] = b;
    }
    f.close();
  }
}
void Logger::writeSatStatusPacket(HealthData he){
  fstream f;
  f.open(current_status_writing_file, ios::app);
  if(f){
    for(unsigned int i = 0; i < sizeof(HealthData); i++){
      f << (int)(((uint8_t*)&he)[i]) << " ";
    }
    f << '\n';
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
    // f.seekg(index*sizeof(ImagingData), ios::beg);
    int b;
    for(unsigned int i = 0; i < index*sizeof(ImagingData); i++){
      f >> b;
    }
    for(unsigned int i = 0; i < sizeof(ImagingData); i++){
      f >> b;
      ((uint8_t*)imagingData)[i] = b;
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
      f << (int)(((uint8_t*)&im)[i]) << " ";
    }
    f << '\n';
    f.close();
    writing_imaging_counter++;
  } else{
    std::cout << "Error opening imaging file" << std::endl;
  }
}

// #define debbuging_logger 
#ifdef debbuging_logger
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
#endif