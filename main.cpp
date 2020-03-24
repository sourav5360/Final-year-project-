#include <SPI.h>

#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

void setup() {

Serial.begin(9600);

display.begin(SSD1306_SWITCHCAPVCC, 0x3D);

display.display();

delay(2000);

display.clearDisplay();

}

void loop() {

while(Serial.available() > 0){

String Date = Serial.readStringUntil('|');

Serial.read();

String Time = Serial.readStringUntil('|');

Serial.read();

String Phone = Serial.readStringUntil('|');

Serial.read();

String Text = Serial.readStringUntil('\n');

Serial.read();

}

if(Text == "text" && Phone == "phone")

{ display.println(Date);

display.display();

display.println(Time);

display.display();

display.clearDisplay();

}

if (Text != "text" && Phone == "phone"){

display.println(Text);

display.display();

delay(5000);

display.clearDisplay()

}
  
