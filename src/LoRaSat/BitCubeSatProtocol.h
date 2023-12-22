#ifndef BitCubeSatProtocol_h
#define BitCubeSatProtocol_h

#include "Arduino.h"
#include <LoRa.h>

class BitCubeSatProtocol{
  public:
    BitCubeSatProtocol(long int frequency);
    void main_loop();
    void send_data();
    void receive_command();
    void set_data(String _data);
    String read_command();
    boolean is_message_received();
  
  private:
    long int frequency;

    String command = String(30);
    String data = String(30);

    boolean message_received;
};

#endif
