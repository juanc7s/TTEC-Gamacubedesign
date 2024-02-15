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

ImagingData testingData;

int main(){
  int client, server;
  int portNum = 8080;
  bool isExit = false;
  int bufsize=256;
  char buffer[bufsize];
  const char *ip = "127.0.0.1";

  struct sockaddr_in server_addr;
  
  for(int i = 0; i < 5; i++){
    testingData.lightnings[i].duration = i*10.0;
    testingData.lightnings[i].index = i;
    testingData.lightnings[i].radius = 5.0;
    testingData.lightnings[i].x = i + 25;
    testingData.lightnings[i].y = 20 + i * 1.5;
  }

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
  }

  recv(client, buffer, bufsize, 0);
  cout << "Connection confirmed" << endl;

  // strcpy(buffer, "123\n");
  char inputN;
  cin >> inputN;
  while(inputN!='e'){
    send(client, (uint8_t*)&testingData, sizeof(ImagingData), 0);
    cin >> inputN;
  }

  // cout << "Enter # to end the connection" << endl;

  // do{
  //     cout << "Client: ";
  //     do{
          // int Number;
          // cin >> Number;
          // cout << Number;
          // send(client, (uint8_t*)&testingData, sizeof(ImagingData), 0);
          // send(client, buffer, 5, 0);
      //     if(*buffer == '#'){
      //         send(client, buffer, bufsize, 0);
      //         *buffer = '*';
      //         isExit = true;
      //     }
      // } while(*buffer != 42);

      // cout << "Server: ";

      // do{
      //     recv(client, buffer, bufsize, 0);
      //     cout << buffer << " ";
      //     if(*buffer == '#'){
      //         *buffer = '*';
      //         isExit = true;
      //     }
      // } while(*buffer != 42);

      // cout << endl;
  // } while(!isExit);

  cout << "Connection terminated..." << endl;
  cout << "Goodbye" << endl;

  close(client);

  return 0;
}