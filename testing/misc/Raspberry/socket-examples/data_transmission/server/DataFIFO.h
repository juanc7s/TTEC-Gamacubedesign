#ifndef DATA_FIFO_H
#define DATA_FIFO_H

#include <iostream>
#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"

class FIFO{
  public:
    unsigned int serial_buffer_writing_pointer = 0;
    unsigned int serial_buffer_reading_pointer = 0;
    unsigned int serial_buffer_size = 0;
    static const unsigned int buffer_size;

    bool available();

    bool data_available = false;
};

class StatusFIFO : public FIFO{
  public:
    static const unsigned int buffer_size = 10;
    HealthData serial_buffer[buffer_size];

    void write(HealthData b);
    HealthData read();
};

class ImagingFIFO : public FIFO{
  public:
    static const unsigned int buffer_size = 30;
    ImagingData serial_buffer[buffer_size];

    void write(ImagingData b);
    ImagingData read();
};

#endif