#include <stdio.h>
#include <thread>
#include <time.h>

#include "ImagingServer.cpp"
#include "StatusServer.cpp"
#include "DataFIFO.h"

clock_t timer = clock();

float seconds(){
  return ((float)(clock() - timer))/CLOCKS_PER_SEC;
}

long millis(){
  return seconds()*1000;
}

ImagingFIFO imaging_fifo;
StatusFIFO status_fifo;
bool running = false;
// int portNum = 8080;

ImagingDataServer server_1(&imaging_fifo, 8080);
StatusDataServer server_2(&status_fifo, 8081);

void run_server_1(){
  server_1.run_server();
}

void run_server_2(){
  server_2.run_server();
}

void do_stuff(){
  char inputN;
  cin >> inputN;
  if(inputN=='e'){
    running = false;
  } else{

  }
}

void read_fifos(){while(imaging_fifo.available()){
    cout << "Imaging FIFO: " << endl;
    ImagingData newPacket = imaging_fifo.read();
    for(int i = 0; i < 5; i++){
      cout << "Index: " << newPacket.lightnings[i].index << endl;
      cout << "Duration: " << newPacket.lightnings[i].duration << endl;
      cout << "Size: " << newPacket.lightnings[i].radius << endl;
      cout << "x: " << newPacket.lightnings[i].x << endl;
      cout << "y: " << newPacket.lightnings[i].y << endl << endl;
    }
    cout << endl;
  }
  while(status_fifo.available()){
    cout << "Status FIFO: " << endl;
    HealthData newPacket = status_fifo.read();
    cout << "Packet index: " << newPacket.index << endl;
    cout << "Reading time: " << newPacket.time << endl;
    cout << "Battery charge: " << newPacket.battery_charge << endl;
    cout << "Battery current: " << newPacket.battery_current << endl;
    cout << "Battery voltage: " << newPacket.battery_voltage << endl;
    cout << "Battery temperature: " << newPacket.battery_temperature << endl;
    cout << "External temperature: " << newPacket.external_temperature << endl;
    cout << "Internal temperature: " << newPacket.internal_temperature << endl;
    cout << "Memory usage: " << newPacket.sd_memory_usage << endl;
    cout << endl;
  }
}

int main(){
  std::thread thread_1 (run_server_1);
  std::thread thread_2 (run_server_2);

  running = true;
  while(running){
    read_fifos();
    do_stuff();
  }
  cout << "Closing servers" << endl;
  server_1.close_server();
  server_2.close_server();
  thread_1.join();
  thread_2.join();
  
  exit(1);

  return 0;
}