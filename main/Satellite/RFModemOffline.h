#ifndef RF_MODEM_OFFLINE_H
#define RF_MODEM_OFFLINE_H

#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;

#include "../CommunicationProtocol.h"

extern uint8_t rx_buffer[255];
extern unsigned int rx_buffer_writing_pointer;
extern unsigned int rx_buffer_reading_pointer;
extern unsigned int rx_buffer_size;

extern void modem_write(uint8_t b);
extern void modem_write(uint8_t *buf, unsigned int size);
extern uint8_t modem_available();
extern uint8_t modem_read();

extern void initRFModule();

extern void tx_send(uint8_t* buf, unsigned int size);

extern void print_satPacket_status(SatPacket packet);
extern void print_satPacket_imaging(SatPacket packet);
extern void print_satPacket_setOperation(SatPacket packet);
extern void print_satPacket_bytes(SatPacket packet);

#endif