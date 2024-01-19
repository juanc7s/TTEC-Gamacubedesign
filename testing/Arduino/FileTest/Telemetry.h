

#define telemetryFileSize 200

#define telemetryPeriod 200

long unsigned int currentTelemetryIndex = 0;
struct Telemetry{
  char msg1[5];
  char msg2[8];
  byte temp1[4];
  byte temp2[4];
  byte volt[4];
  byte curr[4];
  long int time;
} telemetry;

StaticJsonDocument<50> doc;

File currentWritingTelemetryFile;
long unsigned int currentWritingTelemetryFileIndex = 0;
long unsigned int nextTelemetry;

File currentReadingTelemetryFile;
long unsigned int currentReadingTelemetryFileIndex = 0;
long unsigned int nextTelemetry;

void telemetry_send(){
  
}

void write_telemetry_to_json_doc(){
  doc["msg1"] = telemetry.msg1;
  doc["msg2"] = telemetry.msg2;
  doc["time"] = telemetry.time;
  doc["Temp1"] = *(float*)telemetry.temp1;
  doc["Temp2"] = *(float*)telemetry.temp2;
  doc["Volt"] = *(float*)telemetry.volt;
  doc["Curr"] = *(float*)telemetry.curr;
  doc["index"] = currentWritingTelemetryFileIndex;
  Serial.print("Writing telemetry ");
  Serial.print(currentTelemetryIndex);
  Serial.print(" to file ");
  Serial.println(currentWritingTelemetryFile.name());
  serializeJson(doc, currentWritingTelemetryFile);
}

void telemetry_loop(){
  // write_data_to_SD();
  read_data_from_SD();
}

void write_data_to_SD(){
  if(millis() < nextTelemetry){
    return;
  }
  nextTelemetry += telemetryPeriod;
  if(millis()>=nextTelemetry){
    Serial.println("PACKAGE LOST");
    Serial.println(millis());
    Serial.println(nextTelemetry);
  }
  currentTelemetryIndex++;
  if(currentTelemetryIndex%telemetryFileSize==0){
    currentWritingTelemetryFileIndex++;
    currentTelemetryIndex = 0;
    String filename;
    filename = "log" + String(currentWritingTelemetryFileIndex) + ".dat";
    Serial.print("Saving file: ");
    Serial.println(currentWritingTelemetryFile.name());
    Serial.print("Creating new log file: ");
    Serial.println(filename);
    currentWritingTelemetryFile.close();
    if(SD.exists(filename)){
      SD.remove(filename);
    }
    currentWritingTelemetryFile = SD.open(filename, FILE_WRITE);
  }
  
  String("TEMP").toCharArray(telemetry.msg1, 5);
  String("Kitchen").toCharArray(telemetry.msg2, 8);
  telemetry.time = millis();

  *(float*)(telemetry.temp1) = 19.2;
  *(float*)(telemetry.temp2) = 19.7;
  *(float*)(telemetry.volt) = 3.7;
  *(float*)(telemetry.curr) = 0.2;

  if(currentWritingTelemetryFile){
    write_telemetry_to_json_doc();
  } else{
    Serial.print("Error opening ");
    Serial.println(filename);
    while(1);
  }
}

void read_data_from_SD(){
  read_telemetry_from_json_doc();
}

void read_telemetry_from_json_doc(){
  deserializeJson(doc, currentWritingTelemetryFile);
  telemetry.msg1 = doc["msg1"];
  telemetry.msg2 = doc["msg2"];
  telemetry.time = doc["time"];
  *(float*)telemetry.temp1 = doc["Temp1"];
  *(float*)telemetry.temp2 = doc["Temp2"];
  *(float*)telemetry.volt = doc["Volt"];
  *(float*)telemetry.curr = doc["Curr"];
  doc["index"] = currentWritingTelemetryFileIndex;
  Serial.print("Reading telemetry ");
  Serial.print(currentTelemetryIndex);
  Serial.print(" from file ");
  Serial.println(currentWritingTelemetryFile.name());
}