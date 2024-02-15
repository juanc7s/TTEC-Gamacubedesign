#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

void encode_and_send(int client, uint8_t *buffer, unsigned int size){
  uint8_t b = 10;
  send(client, &b, 1, 0);
  bool flag = false;
  for(unsigned int i = 0; i < size; i++){
    if(buffer[i] == 10){
      b = 13;
      send(client, &b, 1, 0);
    }
    send(client, &(buffer[i]), 1, 0);
  }
  b = 10;
  send(client, &b, 1, 0);
}