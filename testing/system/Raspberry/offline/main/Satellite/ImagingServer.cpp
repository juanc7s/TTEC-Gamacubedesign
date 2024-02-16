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

class ImagingDataServer{
  public:
    static const int bufsize=256;
    ImagingFIFO *_fifo;
    int client, server;
    bool isExit = false;
    bool closing = false;
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
        cout << "(Imaging) Error establishing connection." << endl;
        exit(1);
      }

      cout <<  "(Imaging) Server Socket connection created..." << endl;

      server_addr.sin_family = AF_INET;
      server_addr.sin_addr.s_addr = htons(INADDR_ANY);
      server_addr.sin_port = htons(portNum);

      // bindind socket

      if(bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        cout << "(Imaging) Error binding socket..." << endl;
        exit(1);
      }

      size = sizeof(server_addr);
      cout << "(Imaging) Looking for clients..." << endl;

      // listening socket

      listen(client, 2);
  }

  void run_server(){

    //accept client

    server = accept(client, (struct sockaddr*)&server_addr, &size);

    if(server < 0){
      cout << "(Imaging) Error on accepting..." << endl;
      exit(1);
    }

    ImagingData newPacket;

    if(server > 0){
      strcpy(buffer, "Server connected...\n");
      send(server, buffer, bufsize, 0);

      cout << "(Imaging) connected with client..." << endl;

      do{
        unsigned int recvN = recv(server, buffer, bufsize, 0);
        if(strcmp(buffer, "SendPacket\n")==0){
          cout << "(Imaging) Receiving packet" << endl;
          strcpy(buffer, "Ok\n");
          send(server, buffer, bufsize, 0);
          int recvN = recv(server, (uint8_t*)&newPacket, sizeof(ImagingData), 0);
          cout << "(Imaging) Packet received" << endl;
          if(recvN > 0){
            _fifo->write(newPacket);
          }
        } else if(strcmp(buffer, "RequestUpdate\n")==0){
          if(!closing){
            strcpy(buffer, "Nominal\n");
            send(server, buffer, bufsize, 0);
          } else{
            strcpy(buffer, "Close\n");
            send(server, buffer, bufsize, 0);
            isExit = true;
          }
        }
      } while(!isExit);
      
      cout << "(Imaging) Connection terminated..." << endl;
      cout << "(Imaging) Goodbye..." << endl;
      // exit(1);
    }
    
    close(client);
  }

  void close_server(){
    closing = true;
  }
};