#include "SerialConsole.h"

void SerialConsole::write(uint8_t b){
  buffer[writing_pointer++] = b;
  if(writing_pointer == buffer_size){
    writing_pointer = 0;
  }
  reading_size++;
}

uint8_t SerialConsole::read(){
  uint8_t b = buffer[reading_pointer++];
  if(reading_pointer == buffer_size){
    reading_pointer = 0;
  }
  reading_size--;
  return b;
}

unsigned int SerialConsole::available(){
  return reading_size;
}