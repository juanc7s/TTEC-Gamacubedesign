#include "RFModemOffline.h"

uint8_t rx_buffer[255];
unsigned int rx_buffer_writing_pointer = 0;
unsigned int rx_buffer_reading_pointer = 0;
unsigned int rx_buffer_size = 0;

void modem_write(uint8_t b){
  rx_buffer[rx_buffer_writing_pointer++] = b;
  rx_buffer_size++;
  if(rx_buffer_writing_pointer==256){
    rx_buffer_writing_pointer = 0;
  }
}

void modem_write(uint8_t *buf, unsigned int size){
  if(rx_buffer_writing_pointer + size < 256){
    std::memcpy(&rx_buffer[rx_buffer_writing_pointer], buf, size);
    rx_buffer_writing_pointer += size;
    rx_buffer_size += size;
  } else{
    unsigned int r = 256 - rx_buffer_writing_pointer;
    std::memcpy(&rx_buffer[rx_buffer_writing_pointer], buf, r);
    rx_buffer_writing_pointer += 0;
    std::memcpy(&rx_buffer[rx_buffer_writing_pointer], buf, size-r);
    rx_buffer_writing_pointer += size-r;
    rx_buffer_size += size;
  }
}

uint8_t modem_available(){
  return rx_buffer_size;
}

uint8_t modem_read(){
  rx_buffer_size--;
  uint8_t b = rx_buffer[rx_buffer_reading_pointer++];
  if(rx_buffer_reading_pointer==256){
    rx_buffer_reading_pointer = 0;
  }
  return b;
}

void * rx_f(void *p){
    printf("rx done \n");

    rx_buffer_reading_pointer = 0;
    // rx_buffer_size = rx->size;
    // memcpy(rx_buffer, rx->buf, rx->size);

    free(p);
    return NULL;
}

void initRFModule(){
  
}

void tx_send(uint8_t* buf, unsigned int size){
  char txbuf[255];

  SatPacket packet;
  memcpy((uint8_t*)&packet, buf, size);
  std::cout << "Satellite packet with size " << size << std::endl;
  std::cout << "Data packet with length " << (int)packet.length << std::endl;
  switch(packet.operation.protocol){
    case 0:
      print_satPacket_status(packet);
      break;
    case 1:
      print_satPacket_imaging(packet);
      break;
    case 2:
      print_satPacket_setOperation(packet);
      break;
  }

  std::cout << "end\n";
  std::cout << "tx done \n";
}

void print_satPacket_status(SatPacket packet){
  std::cout << "Protocol: PROTOCOL_STATUS" << std::endl;
  std::cout << "Operation: ";
  switch(packet.operation.operation){
    case SATELLITE_STATUS_PACKETS_AVAILABLE:
      std::cout << "Available packets: " << (int)packet.byte_data.number_of_packets;
      std::cout << std::endl;
      std::cout << "pos: ";
      std::cout << ((long long)&packet.byte_data.number_of_packets - (long long)&packet) << std::endl;

      std::cout << "Sent: ";
      print_satPacket_bytes(packet);
      break;
    case SATELLITE_STATUS_PACKET:
      std::cout << "Packet: " << (int)packet.byte_data.index;
      break;
    case SATELLITE_STATUS_PACKETS_DONE:
      std::cout << "Packets done";
      break;
    case SATELLITE_STATUS_DONE:
      std::cout << "Satellite done";
      break;
  }
  std::cout << std::endl;
  std::cout << "packet length: " << (int)packet.length << std::endl;
}

void print_satPacket_imaging(SatPacket packet){
  std::cout << "Protocol: PROTOCOL_IMAGING_DATA";
  std::cout << "Operation: ";
  switch(packet.operation.operation){
    case SATELLITE_IMAGING_PACKETS_AVAILABLE:
      std::cout << "Available packets: " << (int)packet.byte_data.number_of_packets;
      break;
    case SATELLITE_IMAGING_PACKET:
      std::cout << "Packet: " << (int)packet.byte_data.index;
      break;
    case SATELLITE_IMAGING_PACKETS_DONE:
      std::cout << "Imaging packet done";
      break;
    case SATELLITE_IMAGING_DONE:
      std::cout << "Satellite done";
      break;
  }
  std::cout << std::endl;
  std::cout << "packet length: " << (int)packet.length << std::endl;
}

void print_satPacket_setOperation(SatPacket packet){
  std::cout << "Protocol: PROTOCOL_SET_OPERATION";
  std::cout << "Operation: ";
  switch(packet.operation.operation){
    case SATELLITE_SET_OPERATION_ECHO:
      std::cout << "Set operation echo";
      break;
    case SATELLITE_SET_OPERATION_DONE:
      std::cout << "Satellite done";
      break;
  }
  std::cout << std::endl;
  std::cout << "packet length: " << (int)packet.length << std::endl;
}

void print_satPacket_bytes(SatPacket packet){
  for(int i = 0; i < sizeof(SatPacket); i++){
    std::cout << (int)((uint8_t*)&packet)[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "pos: ";
  std::cout << ((long long)&packet.byte_data.number_of_packets - (long long)&packet) << std::endl;
}