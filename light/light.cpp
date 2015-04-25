#include "Adafruit_NeoPixel.h"

#define LEDPIN 6
#define NUM_PIXELS 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

int red = 255;
int blue = 255;
int green = 255;
int brightness = 255;
uint16_t pins = 65535;
String accum;

void draw() {
  bool pixel = 0;
  uint16_t pins_t = pins;
  strip.setBrightness(brightness);
  
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    pixel = pins_t >> i & 0x1;
    if (pixel) 
      strip.setPixelColor(i, strip.Color(red, green, blue));
    else
      strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}


void setup() {
  Serial.begin(9600);
  accum.reserve(200);
  strip.begin();
  draw();
}

void loop() {
  while (Serial.available()) {
    int ret = Serial.read();
    if (ret == -1) break;
    char c = char(ret);
    if (c == '\r') {
      if (accum[0] == 'V') {
	Serial.println("ArduinoNeoPixelShutter");
      } else if (accum[0] == 'S') {
        Serial.print("R");
        Serial.print(red);
        Serial.print(" G");
        Serial.print(green);
        Serial.print(" B");
        Serial.print(blue);
        Serial.print(" S");
        Serial.println(pins);
      } else if (accum[0] == 'P') {
	uint16_t val = accum.substring(1).toInt();
	if (val >= 0 && val <= 65535) {
	  pins = val;
	  Serial.println("OK");
	} else {
	  Serial.print("Invalid pin value: ");
	  Serial.println(val);
	}
      } else if (accum[0] == 'I') {
	int val = accum.substring(1).toInt();
	if (val >= 0 && val <= 255) {
	  brightness = val;
	  Serial.println("OK");
	} else {
	  Serial.print("Invalid brightness value: ");
	  Serial.println(val);
	}
      }
      else if (accum[0] == 'R' || accum[0] == 'G' || accum[0] == 'B') {
	int val = accum.substring(1).toInt();
	if (val >= 0 && val <= 255) {
	  if (accum[0] == 'R') red = val;
	  else if (accum[0] == 'G') green = val;
	  else if (accum[0] == 'B') blue = val;
	  Serial.println("OK");
	} else {
	  Serial.print("Invalid color value: ");
	  Serial.println(val);
	}
      } else  {
	Serial.print("Invalid command: ");
	Serial.println(accum);
      }  
      accum = "";
      draw();
    } else {
      accum += char(ret);
    }
  }
}
