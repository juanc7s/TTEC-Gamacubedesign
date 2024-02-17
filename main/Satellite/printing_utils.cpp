#include <stdio.h>

void print_status_packet_bytes(HealthData statusPacket){
  for(int i = 0; i < sizeof(HealthData); i++){
    cout << (int)(((uint8_t*)&statusPacket)[i]) << " ";
  }
}

void print_imaging_packet(ImagingData imagingPacket){
  for(int i = 0; i < 5; i++){
    cout << "Index: " << imagingPacket.lightnings[i].index << endl;
    cout << "Duration: " << imagingPacket.lightnings[i].duration << endl;
    cout << "Size: " << imagingPacket.lightnings[i].radius << endl;
    cout << "x: " << imagingPacket.lightnings[i].x << endl;
    cout << "y: " << imagingPacket.lightnings[i].y << endl << endl;
  }
}

void print_imaging_packet_bytes(ImagingData imagingPacket){
  for(int i = 0; i < sizeof(ImagingData); i++){
    cout << (int)(((uint8_t*)&imagingPacket)[i]) << " ";
  }
}