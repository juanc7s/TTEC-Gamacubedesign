#include "GPS.h"
#include "Display.h"

void setup(){
  Serial.begin(115200);
  init_gps();
  init_display();
  display_loop();
}

void loop(){
  gps_loop();
//  print_on_display("Hello world!", "");
  print_on_display("LON " + String(gps.location.lng()), "LAT " + String(gps.location.lat()) + " Olá Rocha!");
}
