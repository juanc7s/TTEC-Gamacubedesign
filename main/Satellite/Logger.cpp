// #include "Logger.h"

#include <iostream>
// #include <fstream>

using namespace std;

// #include "../CommunicationProtocol.h"
// #include "RFModule.h"

// const char* stf1 = "stf1.dat";
// const char* imf1 = "imf1.dat";
// const char* stf2 = "stf2.dat";
// const char* imf2 = "imf2.dat";
// bool st_state = false;
// bool im_state = false;

// char* Logger::current_status_reading_file;
// char* Logger::current_status_writing_file;
// char* Logger::current_imaging_reading_file;
// char* Logger::current_imaging_writing_file;

// unsigned long int reading_status_counter = 0;
// unsigned long int reading_imaging_counter = 0;
// unsigned long int writing_status_counter = 0;
// unsigned long int writing_imaging_counter = 0;

// unsigned int Logger::reading_status_file_pointer;
// unsigned int Logger::reading_imaging_file_pointer;

// Logger::Logger(){
  // if (!SD.begin(sd_chipSelect)) {
  //   std::cout << "SD:F" << std::endl;
  //   while(1);
  // } else{
  //   std::cout << "SD:S" << std::endl;
  //   SD_on = true;
  //   current_status_writing_file = stf2;
  //   current_imaging_writing_file = imf2;
  // }
// }
// void Logger::switch_status_file(){
//   if(st_state){
//     if(current_status_reading_file){
//       std::remove(stf2);
//     }
//     current_status_writing_file = (char*)stf2;
//     current_status_reading_file = (char*)stf1;
//   } else{
//     if(current_status_reading_file){
//       std::remove(stf1);
//     }
//     current_status_writing_file = (char*)stf1;
//     current_status_reading_file = (char*)stf2;
//   }
//   reading_status_counter = writing_status_counter;
//   writing_status_counter = 0;
//   st_state = !st_state;
// }
// void Logger::switch_imaging_file(){
//   if(st_state){
//     if(current_imaging_reading_file){
//       std.remove(stf1);
//     }
//     current_imaging_writing_file = imf1;
//     current_imaging_reading_file = imf2;
//   } else{
//     if(current_imaging_reading_file){
//       std.remove(imf2);
//     }
//     current_imaging_writing_file = imf2;
//     current_imaging_reading_file = imf1;
//   }
//   reading_imaging_counter = writing_imaging_counter;
//   writing_imaging_counter = 0;
//   im_state = !im_state;
// }

// void Logger::sdReadSatStatusPacket(HealthData* healthData, unsigned int index){
//   fstream f;
//   f.open(current_status_reading_file, ios::in);
//   f.seek(index*sizeof(HealthData), ios::beg);
//   for(unsigned int i = 0; i < sizeof(HealthData); i++){
//     ((uint8_t*)healthData)[i] = f.read();
//   }
//   f.close();
// }
// void Logger::sdWriteSatStatusPacket(unsigned long int t,
//   float bv, float bcu, float bch, float bt, float ti, float te,
//   unsigned long int sd, uint8_t* rasp_data){
//   fstream f;
//   f.open(current_status_writing_file, ios::app);
//   HealthData st;
//   st.time = t;
//   st.battery_voltage = bv;
//   st.battery_current = bcu;
//   st.battery_charge = bch;
//   st.battery_temperature = bt;
//   st.internal_temperature = ti;
//   st.external_temperature = te;
//   st.sd_memory_usage = sd;
//   for(unsigned int i = 0; i < 10; i++){
//     st.rasp_data[i] = rasp_data[i];
//   }
//   for(unsigned int i = 0; i < sizeof(HealthData); i++){
//     // "0"
//     f.write(((uint8_t*)(&st))[i]);
//   }
//   f.close();
//   writing_status_counter++;
// }

// void Logger::sdReadSatImagingDataPacket(ImagingData* imagingData, unsigned int index){
//   fstream f;
//   f.open(current_imaging_reading_file, ios::in);
//   f.seek(index*sizeof(HealthData), ios::beg);
//   for(unsigned int i = 0; i < sizeof(ImagingData); i++){
//     ((uint8_t*)imagingData)[i] = f.read();
//   }
//   f.close();
// }
// void Logger::sdWriteSatImagingDataPacket(ImagingData im){
//   fstream f;
//   f.open(current_imaging_writing_file, std::app);
//   for(unsigned int i = 0; i < sizeof(ImagingData); i++){
//     // "0"
//     f.write(((uint8_t*)(&im))[i]);
//   }
//   f.close();
//   writing_imaging_counter++;
// }

int main(){
  std::cout << "Initiating logger!" << std::endl;
  // Logger logger;
  return 0;
}