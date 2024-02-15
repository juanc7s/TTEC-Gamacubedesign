#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#include "DataFIFO.h"
#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"

const int bufsize=256;

class ImagingDataServer{
  public:
    ImagingFIFO *_fifo;
    int client, server;
    bool isExit = false;
    char buffer[bufsize];
    unsigned int _portNum;

    struct sockaddr_in server_addr;
    socklen_t size;

    ImagingDataServer(ImagingFIFO *fifo, unsigned int portNum){
      _fifo = fifo;
      _portNum = portNum;

      // init socket.

      client = socket(AF_INET, SOCK_STREAM, 0);

      if(client < 0){
        cout << "Error establishing connection." << endl;
        exit(1);
      }

      cout <<  "Server Socket connection created..." << endl;

      server_addr.sin_family = AF_INET;
      server_addr.sin_addr.s_addr = htons(INADDR_ANY);
      server_addr.sin_port = htons(portNum);

      // bindind socket

      if(bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        cout << "Error binding socket..." << endl;
        exit(1);
      }

      size = sizeof(server_addr);
      cout << "Looking for clients..." << endl;

      // listening socket

      listen(client, 2);
  }

  void run_server(){

    //accept client

    server = accept(client, (struct sockaddr*)&server_addr, &size);

    if(server < 0){
      cout << "Error on accepting..." << endl;
      exit(1);
    }

    ImagingData newPacket;

    while(server > 0){
      strcpy(buffer, "Server connected...\n");
      send(server, buffer, bufsize, 0);

      cout << "connected with client..." << endl;
      // cout << "Enter # to end the connection" << endl;


      do{
        // do{
        //   unsigned int recN = recv(server, buffer, bufsize, 0);
        //   cout << recN << " bytes received: ";
        //   cout << buffer << " ";

        //   cout << endl;
        //   for(unsigned int i = 0; i < N; i++){
        //     _fifo->write(buffer[i]);
        //   }

        //   if(*buffer == '#'){
        //       *buffer = '*';
        //       isExit = true;
        //   }
        // } while(*buffer != '*');

        int recvN = recv(server, (uint8_t*)&newPacket, sizeof(ImagingData), 0);
        if(recvN > 0){
          _fifo->write(newPacket);
          cout << "FIFO: ";
          while(_fifo->available()){
            newPacket = _fifo->read();
            for(int i = 0; i < 5; i++){
              cout << "Index: " << newPacket.lightnings[i].index << endl;
              cout << "Duration: " << newPacket.lightnings[i].duration << endl;
              cout << "Size: " << newPacket.lightnings[i].radius << endl;
              cout << "x: " << newPacket.lightnings[i].x << endl;
              cout << "y: " << newPacket.lightnings[i].y << endl << endl;
            }
          }
          cout << endl;
        }
      } while(!isExit);
      
      cout << "Connection terminated..." << endl;
      cout << "Goodbye..." << endl;
      isExit = false;
      exit(1);
    }
    
    close(client);
  }

  void close_server(){
    isExit = true;
  }
};