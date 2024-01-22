#ifndef BitCubeGSProtocol_h
#define BitCubeGSProtocol_h

#include "Arduino.h"
#include <LoRa.h>

class BitCubeGSProtocol{
  public:
    BitCubeGSProtocol();
    boolean begin(long int _frequency);
    String transfer();
//    void set_command(String _command);
//    String readData();
  
  private:
    long unsigned int frequency;

    String sending_data = String(30);
    String receiving_data = String(30);
    
    void sendCommand();
    void receiveData();
};

#endif
