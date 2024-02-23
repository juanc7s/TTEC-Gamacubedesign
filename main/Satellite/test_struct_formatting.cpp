#include "stdio.h"
#include <iostream>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <unistd.h>

using namespace std;

#include "../CommunicationProtocol.h"

int main(){
  SatPacket satPacket;
  GSPacket gsPacket;

  sizeof(SatPacket);

  *(uint8_t*)&(satPacket.length) = 1;

  *(uint8_t*)&(satPacket.operation) = 1;

  *(uint8_t*)&(satPacket.byte_data) = 1;

  // *(uint8_t*)&(satPacket.data) = 1;

  *(uint8_t*)&(satPacket.data.healthData.index) = 1;
  *(uint8_t*)&(satPacket.data.healthData.time) = 1;
  *(uint8_t*)&(satPacket.data.healthData.battery_charge) = 1;
  *(uint8_t*)&(satPacket.data.healthData.battery_current) = 1;
  *(uint8_t*)&(satPacket.data.healthData.battery_temperature) = 1;
  *(uint8_t*)&(satPacket.data.healthData.battery_voltage) = 1;
  *(uint8_t*)&(satPacket.data.healthData.external_temperature) = 1;
  *(uint8_t*)&(satPacket.data.healthData.internal_temperature) = 1;
  *(uint8_t*)&(satPacket.data.healthData.sd_memory_usage) = 1;

  for(unsigned int i = 0; i < sizeof(SatPacket); i++){
    cout << (int)((uint8_t*)&satPacket)[i] << " ";
  }
  cout << endl;
  return 0;
}