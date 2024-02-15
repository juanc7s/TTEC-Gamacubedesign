#include "DataFIFO.h"

void ImagingFIFO::write(ImagingData b){
  unsigned int n = this->serial_buffer_writing_pointer++;
  for(unsigned int i = 0; i < sizeof(ImagingData); i++){
    ((uint8_t*)&(serial_buffer[n]))[i] = ((uint8_t*)&b)[i];
  }
  if(this->serial_buffer_writing_pointer == this->buffer_size){
    this->serial_buffer_writing_pointer = 0;
  }
  this->serial_buffer_size++;
}

ImagingData ImagingFIFO::read(){
  unsigned int n = this->serial_buffer_reading_pointer++;
  ImagingData b;
  for(unsigned int i = 0; i < sizeof(ImagingData); i++){
    ((uint8_t*)&b)[i] = ((uint8_t*)&(serial_buffer[n]))[i];
  }
  if(this->serial_buffer_reading_pointer >= this->buffer_size){
    this->serial_buffer_reading_pointer = 0;
  }
  this->serial_buffer_size--;
  return b;
}

void StatusFIFO::write(HealthData b){
  unsigned int n = this->serial_buffer_writing_pointer++;
  for(unsigned int i = 0; i < sizeof(HealthData); i++){
    ((uint8_t*)&(serial_buffer[n]))[i] = ((uint8_t*)&b)[i];
  }
  if(this->serial_buffer_writing_pointer == this->buffer_size){
    this->serial_buffer_writing_pointer = 0;
  }
  this->serial_buffer_size++;
}

HealthData StatusFIFO::read(){
  unsigned int n = this->serial_buffer_reading_pointer++;
  HealthData b;
  for(unsigned int i = 0; i < sizeof(HealthData); i++){
    ((uint8_t*)&b)[i] = ((uint8_t*)&(serial_buffer[n]))[i];
  }
  if(this->serial_buffer_reading_pointer >= this->buffer_size){
    this->serial_buffer_reading_pointer = 0;
  }
  this->serial_buffer_size--;
  return b;
}

bool FIFO::available(){
  return this->serial_buffer_size > 0;
}