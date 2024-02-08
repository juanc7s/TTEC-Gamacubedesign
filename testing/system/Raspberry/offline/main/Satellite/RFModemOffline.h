#ifndef RF_MODEM_OFFLINE_H
#define RF_MODEM_OFFLINE_H

#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;

#include <cstdint>

extern uint8_t rx_buffer[255];
extern unsigned int rx_buffer_pointer;
extern unsigned int rx_buffer_size;

extern uint8_t modem_available();
extern uint8_t modem_read();

extern void initRFModule();

extern void tx_send(uint8_t* buf, unsigned int size);

#endif