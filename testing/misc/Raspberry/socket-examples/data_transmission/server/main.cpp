#include <stdio.h>
#include <thread>
#include <time.h>

#include "server.cpp"
#include "DataFIFO.h"

clock_t timer = clock();

float seconds(){
  return ((float)(clock() - timer))/CLOCKS_PER_SEC;
}

long millis(){
  return seconds()*1000;
}

ImagingFIFO fifo1;
StatusFIFO fifo2;
bool running = false;
// int portNum = 8080;

ImagingDataServer server_1(&fifo1, 8080);
// DataServer server_2(&fifo2, 8081);

void run_server_1(){
  server_1.run_server();
}

void run_server_2(){
  // server_2.run_server();
}

void do_stuff(){

}

int main(){
  std::thread thread_1 (run_server_1);
  // std::thread thread_2 (run_server_2);

  // running = true;
  // while(running){
  //   do_stuff();
  //   // if(fifo1.available()){
  //   //   cout << "Fifo 1: ";
  //   //   while(fifo1.available()){
  //   //     cout << (char)fifo1.read();
  //   //   }
  //   //   cout << endl;
  //   //   // cout << fifo1.serial_buffer << endl;
  //   // }
  //   // if(fifo2.available()){
  //   //   cout << "Fifo 1: ";
  //   //   while(fifo2.available()){
  //   //     cout << (char)fifo2.read();
  //   //   }
  //   // }
  //   if(server_1.isExit){ //  && server_2.isExit
  //     running = false;
  //   }
  // }
  running = true;
  while(running){
    char inputN;
    cin >> inputN;
    if(inputN=='e'){
      running = false;
      cout << "Closing servers" << endl;
      server_1.close_server();
      // server_2.close_server();
    }
  }
  thread_1.join();
  // thread_2.join();

  return 0;
}