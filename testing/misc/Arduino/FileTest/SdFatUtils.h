char filename[256];

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
    entry.getName(filename, 256);
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
    entry.getName(filename, 256);
    if(entry.isDirectory()){
      Serial.println(String(filename));
      if(String(filename).startsWith(".")){
        Serial.println("Not deleting hidden files");
        entry.close();
      } else{
        clean_directory(entry);
        Serial.print("Removing ");
        Serial.println(filename);
        entry.remove();
      }
    } else{
      Serial.print("Removing ");
      Serial.println(filename);
      if(!entry.remove()){
        Serial.print("Failed to remove "); Serial.println(filename);
      }
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
    entry.getName(filename, 256);
    if(entry.isDirectory()){
      Serial.println(String(filename));
      if(String(filename).startsWith(".")){
        Serial.println("Not deleting hidden files");
        entry.close();
      } else{
        clean_directory(entry);
        Serial.print("Removing ");
        Serial.println(filename);
        entry.remove();
      }
    } else{
      Serial.print("Removing ");
      Serial.println(filename);
      if(!entry.remove()){
        Serial.print("Failed to remove "); Serial.println(filename);
      }
    }
  }
  root.close();
}
