#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#include <thread>

using namespace std;

#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"

HealthData testingData;

int client, server;
int portNum = 8081;
bool isExit = false;
bool closing = false;
const int bufsize=256;
char buffer[bufsize];
const char *ip = "127.0.0.1";

struct sockaddr_in server_addr;

// class NonBlockingInput{
//   public:
//     static const unsigned int buffer_size = 64;
//     uint8_t buffer[buffer_size];
//     unsigned int writing_pointer = 0;
//     unsigned int reading_pointer = 0;
//     unsigned int size = 0;
//     bool isExit = false;

//     void write(uint8_t b){
//       buffer[writing_pointer++] = b;
//       if(writing_pointer == buffer_size){
//         writing_pointer = 0;
//       }
//       size++;
//     }
//     uint8_t read(){
//       uint8_t b = buffer[reading_pointer++];
//       if(reading_pointer == buffer_size){
//         reading_pointer = 0;
//       }
//       size--;
//       return b;
//     }
//     bool available(){
//       return size > 0;
//     }
//     void start(){
//       std::thread thread_1 (run);
//     }
//     void run(){
//       while(!isExit){
//         uint8_t b;
//         cin >> b;
//         buffer[writing_pointer++] = b;
//         if(writing_pointer == buffer_size){
//           writing_pointer = 0;
//         }
//         size++;
//         if(b == '#'){
//           isExit = true;
//         }
//       }
//     }
// };

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

void do_stuff(){
  sleep(2);
}

void sendPacket(HealthData newPacket){
  strcpy(buffer, "SendPacket\n");
  send(client, buffer, bufsize, 0);
  unsigned int recvN = recv(client, buffer, bufsize, 0);
  if(strcmp(buffer, "Ok\n")==0){
    cout << "Sending packet..." << endl;
    send(client, (uint8_t*)&newPacket, sizeof(HealthData), 0);
  }
}

int main(){
  
  testingData.battery_charge = 10.0;
  testingData.battery_current = 1;
  testingData.battery_temperature = 1;
  testingData.battery_voltage = 1;
  testingData.external_temperature = 1;
  testingData.index = 1;
  testingData.internal_temperature = 1;
  testingData.sd_memory_usage = 1;
  testingData.time = 1;

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
      cout << "Closing connection" << endl;
    } else if(strcmp(buffer, "Nominal\n")==0){
      cout << "All nominal" << endl;
      sendPacket(testingData);
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