String filename;

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    filename = entry.name();
    Serial.print(filename);
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void clean_directory(File dir){
  while(1){
    File entry = dir.openNextFile();
    if(!entry){
      break;
    }
    filename = entry.name();
    if(entry.isDirectory()){
      Serial.println(entry.name());
      if(filename.startsWith(".")||filename=="TRASH-~1"){
        Serial.print("Not deleting hidden files ");
        Serial.println(filename);
        entry.close();
      } else{
        clean_directory(entry);
        Serial.print("Removing ");
        Serial.println(entry.name());
        entry.close();
        SD.remove(entry.name());
      }
    } else{
      Serial.print("Removing ");
      Serial.println(entry.name());
      entry.close();
      SD.remove(entry.name());
    }
  }
}

void clean_card(){
  File root = SD.open("/");
  while(1){
    File entry = root.openNextFile();
    if(!entry){
      break;
    }
    filename = entry.name();
    if(entry.isDirectory()){
      if(filename.startsWith(".")||filename=="TRASH-~1"){
        Serial.print("Not deleting hidden files ");
        Serial.println(filename);
        entry.close();
      } else{
        clean_directory(entry);
        Serial.print("Removing ");
        Serial.println(entry.name());
        entry.close();
        SD.remove(entry.name());
      }
    } else{
      Serial.print("Removing ");
      Serial.println(entry.name());
      entry.close();
      SD.remove(entry.name());
    }
  }
  root.close();
}
