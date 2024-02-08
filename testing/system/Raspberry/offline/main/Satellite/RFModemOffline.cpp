#include "RFModemOffline.h"

uint8_t rx_buffer[255];
unsigned int rx_buffer_pointer = 0;
unsigned int rx_buffer_size = 0;

uint8_t modem_available(){
  return rx_buffer_size;
}

uint8_t modem_read(){
  rx_buffer_size--;
  return rx_buffer[rx_buffer_pointer++];
}

void * rx_f(void *p){
    printf("rx done \n");

    rx_buffer_pointer = 0;
    // rx_buffer_size = rx->size;
    // memcpy(rx_buffer, rx->buf, rx->size);

    free(p);
    return NULL;
}

void initRFModule(){
  
}

void tx_send(uint8_t* buf, unsigned int size){
  char txbuf[255];

  // std::memcpy(modem.tx.data.buf, buf, size);//copy data we'll sent to buffer
  // modem.tx.data.size = size;//Payload len

  std::cout << "end\n";
  std::cout << "tx done \n";
}