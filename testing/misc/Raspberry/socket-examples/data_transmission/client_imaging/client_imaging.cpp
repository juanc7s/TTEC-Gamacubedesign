#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"

int client, server;
int portNum = 8080;
bool isExit = false;
const int bufsize=256;
char buffer[bufsize];
const char *ip = "127.0.0.1";

struct sockaddr_in server_addr;

int connect_to_socket(){

  // init socket

  client = socket(AF_INET, SOCK_STREAM, 0);

  if(client < 0){
    cout << "Error creating socket..." << endl;
    exit(1);
  }

  cout << "Client socket created" << endl;

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portNum);

  // connecting socket server

  if(connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0){
    cout << "Connecting to server..." << endl;
  } else{
    cout << "Could not connect" << endl;
    cout << "Goodbye" << endl;

    close(client);

    return -1;
  }

  unsigned int recvN = recv(client, buffer, bufsize, 0);
  if(recvN > 0){
    cout << "Connection confirmed" << endl;
  } else{
    cout << "Could not connect" << endl;
    cout << "Goodbye" << endl;

    close(client);

    return -1;
  }
  return 0;
}

void sendPacket(ImagingData newPacket){
  strcpy(buffer, "SendPacket\n");
  send(client, buffer, bufsize, 0);
  unsigned int recvN = recv(client, buffer, bufsize, 0);
  if(strcmp(buffer, "Ok\n")==0){
    cout << "Sending packet..." << endl;
    send(client, (uint8_t*)&newPacket, sizeof(ImagingData), 0);
  }
}

void do_stuff(){
  sleep(2);
}

int main(){

  ImagingData testingData;
  
  for(int i = 0; i < 5; i++){
    testingData.lightnings[i].duration = i*10.0;
    testingData.lightnings[i].index = i;
    testingData.lightnings[i].radius = 5.0;
    testingData.lightnings[i].x = i + 25;
    testingData.lightnings[i].y = 20 + i * 1.5;
  }

  bool imaging_on = true;

  if(connect_to_socket() < 0){
    cout << "Error connecting to socket" << endl;
    cout << "Goodbye" << endl;
    return 0;
  }

  do{
    strcpy(buffer, "RequestUpdate\n");
    send(client, buffer, bufsize, 0);
    cout << "Requesting update" << endl;
    recv(client, buffer, bufsize, 0);
    cout << "Update received" << endl;
    if(strcmp(buffer, "Close\n")==0){
      isExit = true;
      imaging_on = false;
      cout << "Closing connection" << endl;
    } else if(strcmp(buffer, "Nominal\n")==0){
      cout << "All nominal" << endl;
      if(imaging_on){
        sendPacket(testingData);
      }
      do_stuff();
    } else{
      cout << "Unknown response: " << buffer << endl;
    }
  } while(!isExit);

  cout << "Connection terminated..." << endl;
  cout << "Goodbye" << endl;

  close(client);

  cout << "Finished" << endl;

  return 0;
}