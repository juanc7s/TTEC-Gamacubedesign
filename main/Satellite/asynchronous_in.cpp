#include <iostream>
#include <thread>

bool running = true;

uint8_t serial_buffer[256];
unsigned int serial_buffer_writing_pointer = 0;
unsigned int serial_buffer_reading_pointer = 0;
unsigned int serial_buffer_size = 0;

void serial_receiving_thread(){
  char c;
  while(running){
    std::cin >> c;
    if(c=='e'){
      running = false;
    } else{
      serial_buffer[serial_buffer_writing_pointer++] = c;
      if(serial_buffer_writing_pointer==256){
        serial_buffer_writing_pointer = 0;
      }
      serial_buffer_size++;
    }
  }
}

uint8_t serial_available(){
  return serial_buffer_size;
}

uint8_t serial_read(){
  serial_buffer_size--;
  uint8_t b = serial_buffer[serial_buffer_reading_pointer++];
  if(serial_buffer_reading_pointer==256){
    serial_buffer_reading_pointer = 0;
  }
  return b;
}

std::thread start_serial_thread(){
  return std::thread(serial_receiving_thread);
}