#include <cstdint>

class SerialConsole{
  uint8_t buffer[32];
  unsigned int writing_pointer = 0;
  unsigned int reading_pointer = 0;
  unsigned int reading_size = 0;
  const unsigned int buffer_size = 32;
  
  public:
    void write(uint8_t b);
    uint8_t read();
    unsigned int available();
};