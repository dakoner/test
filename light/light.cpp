#include "Adafruit_NeoPixel.h"

#define LEDPIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, LEDPIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
String accum;

int red = 255;
int blue = 255;
int green = 255;
//int oldPot = 0;

void loop() {
  while (Serial.available()) {
    int ret = Serial.read();
    if (ret == -1) break;
    char c = char(ret);
    if (c == '\r') {
      if (accum[0] == 'S') {
        Serial.print("R");
        Serial.print(red);
        Serial.print(" G");
        Serial.print(green);
        Serial.print(" B");
        Serial.println(blue);
      } else if (accum[0] == 'R' || accum[0] == 'G' || accum[0] == 'B') {
	int val = accum.substring(1).toInt();
	if (val >= 0 && val <= 255) {
	  if (accum[0] == 'R') red = val;
	  else if (accum[0] == 'G') green = val;
	  else if (accum[0] == 'B') blue = val;
	} else {
	  Serial.print("Invalid value: ");
	  Serial.println(val);
	}
      } else  {
	Serial.print("Invalid color: ");
	Serial.println(accum[0]);
      }  
      accum = "";
    } else {
      accum += char(ret);
    }
    
  }
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}
