#include <iostream>
#include <sys/statvfs.h>
#include <time.h>

#include "StatusDataClient.cpp"

HealthData testingData;
StatusDataClient statusSocket(8081);

clock_t timer = clock();

float seconds(){
  return ((float)(clock() - timer))/CLOCKS_PER_SEC*1000;
}

unsigned long millis(){
  return seconds()*1000;
}

void do_stuff(){
  sleep(2);
}

int main(){
  testingData.battery_charge = 10.0;
  testingData.battery_current = 1;
  testingData.battery_temperature = 1;
  testingData.battery_voltage = 1;
  testingData.external_temperature = 1;
  testingData.index = 1;
  testingData.internal_temperature = 1;
  testingData.sd_memory_usage = 1;

  if(statusSocket.connect_to_socket() < 0){
    cout << "Error connecting to socket" << endl;
    cout << "Goodbye" << endl;
    return 0;
  }

  while(statusSocket.is_connected){
    do_stuff();
    statusSocket.update();
    cout << "All nominal" << endl;
    struct statvfs fiData;
    if((statvfs("/",&fiData)) < 0 ) {
      cout << "\nFailed to stat:"  << "/";
    } else{
      testingData.sd_memory_usage = ((fiData.f_blocks-fiData.f_bfree)*fiData.f_bsize/1000000);
      testingData.time = millis();
      cout << "Packet bytes: " << endl;
      for(int i = 0; i < sizeof(HealthData); i++){
        cout << (int)(((uint8_t*)&testingData)[i]) << " ";
      }
      cout << endl;
      statusSocket.send_packet(testingData);
    }
  }

  cout << "Finished" << endl;

  return 0;
}
