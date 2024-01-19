 #include <Adafruit_BMP085.h>
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "GPS.h"

#define I2C_SDA 21
#define I2C_SCL 22
//#define I2C_DEV_ADDR 0x98

//const int I2C_DEV_ADDR = 0x24;
const int MPU_addr=0x68; 
char dado_dic[100];
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t gps_state;
float gps_lon;
float gps_lat;
char dado_i2c[200];
byte gpsData;
int buzzer = 14;

//const int pinoSensor = 4; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
// 
//float tensaoEntrada = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DE TENSÃO DE ENTRADA DO SENSOR
//float tensaoMedida = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DA TENSÃO MEDIDA PELO SENSOR
// 
//float valorR1 = 30000.0; //VALOR DO RESISTOR 1 DO DIVISOR DE TENSÃO
//float valorR2 = 7500.0; // VALOR DO RESISTOR 2 DO DIVISOR DE TENSÃO
//int leituraSensor = 0; //VARIÁVEL PARA ARMAZENAR A LEITURA DO PINO ANALÓGICO
 
Adafruit_BMP085 bmp; 
SoftwareSerial ss(4, 3);

void setup(){

//  Serial.begin(115200);
//  Serial.println("Hello Mafe!");

  //pinMode(pinoSensor, INPUT); //DEFINE O PINO COMO ENTRADA
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); 
  Wire.write(0); 
  Wire.endTransmission(true);

  if(!bmp.begin()){ 
//  Serial.println("Sensor BMP180 não foi identificado! Verifique as conexões."); 
    while(1){} 
  }

  //inicializar o SD card

//  //Serial.begin(115200);
  if(!SD.begin()){
//    Serial.println("Card Mount Failed");
    while(1){}
  }
  uint8_t cardType = SD.cardType();
  
  if(cardType == CARD_NONE){
//    Serial.println("No SD card attached");
    return;
  }
//  //Serial.begin(9600);
  ss.begin(9600);
  
  init_gps();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
//  //Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
//    //Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
//    //Serial.println("File written");
  } else {
//    //Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
//  //Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
//    //Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
    //Serial.println("Message appended");
  } else {
    //Serial.println("Append failed");
  }
  file.close();
}

void buzina() 
{
    // Define o pino do Buzzer como Saida 
    pinMode(buzzer,OUTPUT);    
    // Aciona o buzzer na frequencia relativa ao Dó em Hz
    tone(buzzer,261);    
    // Espera um tempo para Desativar
    delay(200);
    //Desativa o buzzer
    noTone(buzzer);
    // Aciona o buzzer na frequencia relativa ao Ré em Hz   
    tone(buzzer,293);             
    delay(200);    
    noTone(buzzer); 
    // Aciona o buzzer na frequencia relativa ao Mi em Hz
    tone(buzzer,329);      
    delay(200);
    noTone(buzzer);     
    // Aciona o buzzer na frequencia relativa ao Fá em Hz
    tone(buzzer,349);    
    delay(200);    
    noTone(buzzer); 
    // Aciona o buzzer na frequencia relativa ao Sol em Hz
    tone(buzzer,392);            
    delay(200);
    noTone(buzzer); 
    
    digitalWrite(buzzer, HIGH);
}

bool gps_flag_1 = false;
bool gps_flag_2 = true;

void loop(){
//  Serial.print("En~tao, eu to levando uma bomba aqui, mas perai");
  gps_loop();
  long cur_time = millis();
  if (cur_time > 5000){
    gps_flag_1 = true;
    if(gps.charsProcessed() < 10){
//      Serial.println(F("No GPS detected: check wiring."));
      gps_flag_2 = false;
    }
  }

//  String gps_reading = "No data yet.";
  gps_lon = -1.0;
  gps_lat = -1.0;
  gps_state = 0;
  if(gps_flag_1){
    gps_lon = 0.0;
    gps_lat = 0.0;
    gps_state = 1;
//    gps_reading = 1;
//    gps_reading = "No GPS detected: check wiring.";
    if(gps_flag_2){
      gps_lon = gps.location.lng();
      gps_lat = gps.location.lat();
      gps_state = 2;
//      gps_reading = "LON " + String(gps.location.lng()) + ":LAT " + String(gps.location.lat());
//      gps_reading = "GPS working.";
//      gps_reading = 2;
    }
  }

  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true); 
  
  AcX=Wire.read()<<8|Wire.read(); 
  AcY=Wire.read()<<8|Wire.read(); 
  AcZ=Wire.read()<<8|Wire.read(); 
  Tmp=Wire.read()<<8|Wire.read(); 
  GyX=Wire.read()<<8|Wire.read(); 
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read(); 

 
  //Serial.print("AcX = "); //Serial.print(AcX);
  //Serial.print(" | AcY = "); //Serial.print(AcY);
  //Serial.print(" | AcZ = "); //Serial.print(AcZ);
  //Serial.print(" | Tmp = "); //Serial.print(Tmp/340.00+36.53); 
  //Serial.print(" | GyX = "); //Serial.print(GyX);
  //Serial.print(" | GyY = "); //Serial.print(GyY);
  //Serial.print(" | GyZ = "); //Serial.println(GyZ);
  delay(333);


      //Serial.print("Temperatura: "); 
    //Serial.print(bmp.readTemperature());
    //Serial.println(" *C (Grau Celsius)"); 
        
    //Serial.print("Pressão: "); 
    //Serial.print(bmp.readPressure());
    //Serial.println(" Pa (Pascal)"); 
     
    //Serial.print("Altitude: "); 
    //Serial.print(bmp.readAltitude());
    //Serial.println(" m (Metros)");
    
    //Serial.print("Pressão a nível do mar (calculada): "); 
    //Serial.print(bmp.readSealevelPressure()); 
    //Serial.println(" Pa (Pascal)"); 
 
    //Serial.print("Altitude real: "); 
    //Serial.print(bmp.readAltitude(101500)); 
    //Serial.println(" m (Metros)"); 
     
    //Serial.println("-----------------------------------"); 
    delay(200); 

//    while (ss.available() > 0){
//        // get the byte data from the GPS
//        gpsData = ss.read();
//        //Serial.write(gpsData);
//    }

//   leituraSensor = analogRead(pinoSensor); //FAZ A LEITURA DO PINO ANALÓGICO E ARMAZENA NA VARIÁVEL O VALOR LIDO
//   tensaoEntrada = (leituraSensor * 5.0) / 1024.0; //VARIÁVEL RECEBE O RESULTADO DO CÁLCULO
//   tensaoMedida = tensaoEntrada / (valorR2/(valorR1+valorR2)); //VARIÁVEL RECEBE O VALOR DE TENSÃO DC MEDIDA PELO SENSOR

    StaticJsonDocument<250> doc;
    doc["AcX"] = AcX;
    doc["AcY"] = AcY;
    doc["AcZ"] = AcZ;
    doc["tmp"] = (Tmp/340.00+36.53);
    doc["GyX"] = GyX;
    doc["GyY"] = GyY;
    doc["GyZ"] = GyZ;
    doc["P"]=bmp.readPressure();
    doc["Alt"]=bmp.readAltitude();
    doc["PM"]=bmp.readSealevelPressure();
    doc["AR"]=bmp.readAltitude(101500);
    doc["GPS"]= gps_state;
    doc["LON"]= gps_lon;
    doc["LAT"]= gps_lat;
    //doc["BAT"] = tensaoMedida;
    //criar um de bateria
   // Convert the JSON object to a string
    String jsonString;
    serializeJson(doc, jsonString);

   appendFile(SD, "/dados.txt", jsonString.c_str());
  //writeFile(SD, "/dados.txt",jsonString.c_str());
  //dado_i2c = jsonString.c_str();
   buzina(); 
}
