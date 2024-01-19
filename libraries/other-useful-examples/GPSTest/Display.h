#include "heltec.h"
#include "Arduino.h"

SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED, RST_OLED);

void VextON(void)
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void) //Vext default OFF
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}

void init_display(){
  VextON();
  delay(100);

  display.init();
  display.clear();
  display.display();
  
  display.setContrast(255);
  
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.clear();
  display.display();
  display.screenRotate(ANGLE_0_DEGREE);
  display.setFont(ArialMT_Plain_16);
}

void display_loop(){
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.clear();
  display.display();
  display.screenRotate(ANGLE_0_DEGREE);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 32-16/2, "ROTATE_0");
  display.display();
  delay(500);

  display.clear();
  display.display();
  display.screenRotate(ANGLE_90_DEGREE);
  display.setFont(ArialMT_Plain_10);
  display.drawString(32, 64-10/2, "ROTATE_90");
  display.display();
  delay(500);

  display.clear();
  display.display();
  display.screenRotate(ANGLE_180_DEGREE);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 32-16/2, "ROTATE_180");
  display.display();
  delay(500);

  display.clear();
  display.display();
  display.screenRotate(ANGLE_270_DEGREE);
  display.setFont(ArialMT_Plain_10);
  display.drawString(32, 64-10/2, "ROTATE_270");
  display.display();
  delay(500);
  display.screenRotate(ANGLE_180_DEGREE);
}

void print_on_display(String L1, String L2){
  display.clear();
  display.drawString(40,20,L1);
  display.drawString(40,40,L2);
  display.display();
}
