#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"
SatPacket satPacket;
GSPacket gsPacket;

void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:

  *(uint8_t*)&(satPacket.length) = 1;
  sizeof(SatPacket);

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
    Serial.print((int)((uint8_t*)&satPacket)[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:

}
