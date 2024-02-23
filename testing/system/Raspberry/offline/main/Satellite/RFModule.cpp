#include "RFModule.h"

uint8_t rxChan = 23;
uint8_t rxAddh = 0xa1;
uint8_t rxAddl = 0x06;
uint8_t txChan = 10;
uint8_t txAddh = 0x8f;
uint8_t txAddl = 0xf7;

bool talking = false;
unsigned long int communication_timeout;
const unsigned long int communication_timeout_limit = 1000;
bool telemetry_received = false;
uint8_t telemetry_state = 0;

uint8_t number_of_packets = 0;
const uint8_t N_status = 10;
const uint8_t N_imaging = 20;

Operation operation = {
  .switch_active_thermal_control = false,
  .switch_attitude_control = false,
  .switch_imaging = false,
  .switch_imaging_mode = false,
  .switch_stand_by_mode = false,
};

SatPacket satPacket;
GSPacket gsPacket;

uint8_t rx_pointer = 0;

void preStatusProtocol(){
  if(reading_status_counter==0){
    if(writing_status_counter>0){
      logger.switch_status_file();
    } else{
      number_of_packets = 0;
      return;
    }
  }
  number_of_packets = reading_status_counter;
  if(number_of_packets > N_status){
    number_of_packets = N_status;
  }
  DBG_Print("Packets available: ");
  DBG_Println((int)number_of_packets);
}

void updateStatusPacket(uint8_t index){
  // for(unsigned int i = 0; i < sizeof(HealthData); i++){
  //   ((uint8_t*)(&satPacket.data.healthData))[i] = ((uint8_t*)(&(dataBuffer.statusData[index])))[i];
  // }
  logger.readSatStatusPacket(&satPacket.data.healthData, index);
}

void postStatusProtocol(){
  logger.reading_status_file_pointer += sizeof(HealthData)*number_of_packets;
  reading_status_counter -= number_of_packets;
}

void preImagingDataProtocol(){
  if(reading_imaging_counter==0){
    if(writing_imaging_counter>0){
      logger.switch_imaging_file();
    } else{
      number_of_packets = 0;
      return;
    }
  }
  number_of_packets = reading_imaging_counter;
  if(number_of_packets > N_imaging){
    number_of_packets = N_imaging;
  }
  // for(unsigned int i = 0; i < n; i++){
  //   sdReadSatStatusPacket(&(dataBuffer.statusData[i]));
  // }
  DBG_Print("Packets available: ");
  DBG_Println(number_of_packets);
}

void updateImagingDataPacket(uint8_t index){
  logger.readSatImagingDataPacket(&satPacket.data.imagingData, index);
}

void postImagingDataProtocol(){
  logger.reading_imaging_file_pointer += sizeof(ImagingData)*number_of_packets;
  reading_imaging_counter -= number_of_packets;
}

void sendSatPacket(){
  DBG_Print("S:");
  DBG_Print(satPacket.length);DBG_Print("    ");
  DBG_Print((int)satPacket.operation.protocol);DBG_Print("    ");
  DBG_Println((int)satPacket.operation.operation);
  tx_send((uint8_t*)&satPacket, satPacket.length);
  // tx_send((uint8_t*)&satPacket, sizeof(satPacket));
}

void updateRFComm(){
  uint8_t b;
  if(modem_available()){
    // if(millis() > communication_timeout){
    //   DBG_Println("T:1");
    //   rx_pointer = 0;
    //   talking = false;
    // }
    while(modem_available()){
      b = modem_read();
      // std::cout << (int)b << " " << (int)rx_pointer << std::endl;
      ((uint8_t*)(&gsPacket))[rx_pointer++] = b;
      if(rx_pointer>0 && rx_pointer==gsPacket.length){
        std::cout << "Received gs: ";
        for(int i = 0; i < gsPacket.length; i++){
          std::cout << (int)((uint8_t*)&gsPacket)[i] << " ";
        }
        std::cout << std::endl;
        onReceive();
        rx_pointer = 0;
      }
      if(rx_pointer > sizeof(gsPacket)){
        rx_pointer = 0;
      }
    }
    communication_timeout = millis() + communication_timeout_limit;
  }
  // if(talking){
  //   if(millis() > communication_timeout){
  //     DBG_Println("T:2");
  //     rx_pointer = 0;
  //     talking = false;
  //   }
  // }
}

void onReceive(){
  DBG_Print("P:R:");
  DBG_Println((int)gsPacket.length);
  std::cout << "Protocol:" << (int)gsPacket.operation.protocol << std::endl;
  std::cout << "Operation:" << (int)gsPacket.operation.operation << std::endl;
  switch(gsPacket.operation.protocol){
    case PROTOCOL_STATUS:
      std::cout << "P:S" << std::endl;
      satPacket.operation.protocol = PROTOCOL_STATUS;
      switchCaseStatusProtocol();
      break;
    case PROTOCOL_IMAGING_DATA:
      std::cout << "P:I" << std::endl;
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      switchCaseImagingDataProtocol();
      break;
    case PROTOCOL_SET_OPERATION:
      std::cout << "P:O" << std::endl;;
      satPacket.operation.protocol = PROTOCOL_SET_OPERATION;
      switchCaseSetOperationProtocol();
      break;
  }
}

void switchCaseStatusProtocol(){
  switch(gsPacket.operation.operation){
    case GS_STATUS_REQUEST:
      DBG_Println("S:1");
      preStatusProtocol();
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_PACKETS_AVAILABLE;
      satPacket.byte_data.number_of_packets = number_of_packets;
      satPacket.length = 3;
      std::cout << "AAA " << (int)satPacket.byte_data.number_of_packets << std::endl;
      print_satPacket_bytes(satPacket);
      sendSatPacket();
      break;
    case GS_STATUS_START_TRANSMISSION:
      DBG_Println("S:2");
      for(unsigned int i = 0; i < number_of_packets; i++){
        updateStatusPacket(i);
        satPacket.operation.protocol = PROTOCOL_STATUS;
        satPacket.operation.operation = SATELLITE_STATUS_PACKET;
        satPacket.byte_data.index = i;
        satPacket.length = sizeof(SatPacket);//sizeof(int64_t)+sizeof(HealthData);
        sendSatPacket();
      }
      DBG_Println("S:3");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_PACKETS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      break;
    case GS_STATUS_RESEND_PACKET:
      DBG_Println("S:4");
      if(!gsPacket.data.resend.isDone){
        DBG_Println("Recalling packets");
        for(unsigned int i = 0; i < 32; i++){
          for(unsigned int j = 0; j < 8; j++){
            if((gsPacket.data.resend.packets[i]>>j)&0x01){
              DBG_Print("Resending packet ");
              DBG_Println(i*8+j);
              updateStatusPacket(i*8+j);
              satPacket.operation.protocol = PROTOCOL_STATUS;
              satPacket.operation.operation = SATELLITE_STATUS_PACKET;
              satPacket.byte_data.index = i*8 + j;
              satPacket.length = sizeof(SatPacket);//sizeof(int64_t)+sizeof(HealthData);
              sendSatPacket();
            }
          }
        }
      } else{
        DBG_Println("S:A");
      }
      DBG_Println("PD");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_PACKETS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      break;
    case GS_STATUS_DONE:
      DBG_Println("S:D");
      satPacket.operation.protocol = PROTOCOL_STATUS;
      satPacket.operation.operation = SATELLITE_STATUS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      postStatusProtocol();
      rx_pointer = 0;
      talking = false;
      break;
  }
}

void switchCaseImagingDataProtocol(){
  switch(gsPacket.operation.operation){
    case GS_IMAGING_REQUEST:
      DBG_Println("I:1");
      preImagingDataProtocol();
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_PACKETS_AVAILABLE;
      satPacket.byte_data.number_of_packets = number_of_packets;
      satPacket.length = 4;
      sendSatPacket();
      break;
    case GS_IMAGING_START_TRANSMISSION:
      DBG_Println("I:2");
      for(unsigned int i = 0; i < number_of_packets; i++){
        updateImagingDataPacket(i);
        satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
        satPacket.operation.operation = SATELLITE_IMAGING_PACKET;
        satPacket.byte_data.index = i;
        satPacket.length = sizeof(SatPacket);//3+sizeof(ImagingData);
        sendSatPacket();
      }
      DBG_Println("I:D");
      satPacket.length = 2;
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_PACKETS_DONE;
      sendSatPacket();
      break;
    case GS_IMAGING_RESEND_STATUS:
      DBG_Println("I:3");
      if(!gsPacket.data.resend.isDone){
        DBG_Println("I:R");
        for(unsigned int i = 0; i < 32; i++){
          for(unsigned int j = 0; j < 8; j++){
            if((gsPacket.data.resend.packets[i]>>j)&0x01){
              DBG_Print("I:R:");
              DBG_Println(i*8+j);
              updateImagingDataPacket(i*8+j);
              satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
              satPacket.operation.operation = SATELLITE_IMAGING_PACKET;
              satPacket.byte_data.index = i*8 + j;
              satPacket.length = sizeof(SatPacket);//3+sizeof(ImagingData);
              sendSatPacket();
            }
          }
        }
      } else{
        DBG_Println("I:A");
      }
      DBG_Println("I:D");
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_PACKETS_DONE;
      satPacket.length = 2;
      sendSatPacket();
      break;
    case GS_IMAGING_DONE:
      DBG_Println("I:4");
      satPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
      satPacket.operation.operation = SATELLITE_IMAGING_DONE;
      satPacket.length = 2;
      sendSatPacket();
      postImagingDataProtocol();
      rx_pointer = 0;
      talking = false;
      break;
  }
}

void switchCaseSetOperationProtocol(){
  switch(gsPacket.operation.operation){
    case GS_SET_OPERATION:
      DBG_Println("O:1");
      // DBG_Print("Active thermal control: ");DBG_Println(gsPacket.data.operation.switch_active_thermal_control ? "ON" : "OFF");
      // DBG_Print("Attitude control: ");DBG_Println(gsPacket.data.operation.switch_attitude_control ? "ON" : "OFF");
      // DBG_Print("Imaging: ");DBG_Println(gsPacket.data.operation.switch_imaging ? "ON" : "OFF");
      // DBG_Print("Imaging mode: ");DBG_Println(gsPacket.data.operation.switch_imaging_mode ? "Mode 2" : "Mode 1");
      // DBG_Print("Stand by: ");DBG_Println(gsPacket.data.operation.switch_stand_by_mode ? "ON" : "OFF");
      satPacket.operation.protocol = PROTOCOL_SET_OPERATION;
      satPacket.operation.operation = SATELLITE_SET_OPERATION_ECHO;
      satPacket.data.operation_echo.switch_active_thermal_control = gsPacket.data.operation.switch_active_thermal_control;
      satPacket.data.operation_echo.switch_attitude_control = gsPacket.data.operation.switch_attitude_control;
      satPacket.data.operation_echo.switch_imaging = gsPacket.data.operation.switch_imaging;
      satPacket.data.operation_echo.switch_imaging_mode = gsPacket.data.operation.switch_imaging_mode;
      satPacket.data.operation_echo.switch_stand_by_mode = gsPacket.data.operation.switch_stand_by_mode;
      satPacket.length = 3+sizeof(GSOperation);
      sendSatPacket();
      break;
    case GS_SET_OPERATION_DONE:
      DBG_Println("O:D");
      *((uint8_t*)&operation) = *((uint8_t*)&satPacket.data.operation_echo);
      satPacket.operation.protocol = PROTOCOL_SET_OPERATION;
      satPacket.operation.operation = SATELLITE_SET_OPERATION_DONE;
      satPacket.length = 2;
      sendSatPacket();
      rx_pointer = 0;
      talking = false;
      break;
  }
}

//void switchCasePingProtocol(){
//  switch(gsPacket.operation.operation){
//    case GS_PING_REQUEST:
//      satPacket.operation.protocol = PROTOCOL_PING;
//      satPacket.operation.operation = GS_PING_REQUEST;
//  }
//}