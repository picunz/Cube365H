#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "utilities.h"
 
 


 
#define PIXELS_LEN  12
#define PIN 48

extern uint32_t ColorFace[8];


#define COLOR_FACE_RED      ringPixels.Color(255, 0, 0)     
#define COLOR_FACE_BLUE     ringPixels.Color(0, 0, 255)     
#define COLOR_FACE_YELLOW   ringPixels.Color(255, 216, 0)   
#define COLOR_FACE_GREEN    ringPixels.Color(0, 255, 0)     
#define COLOR_FACE_ORANGE   ringPixels.Color(255, 10, 10)   
#define COLOR_FACE_VIOLET   ringPixels.Color(200, 0, 255)   
#define COLOR_FACE_WHITE    ringPixels.Color(255, 255, 255) 
#define COLOR_FACE_OFF      ringPixels.Color(0, 0, 0)
 

extern Adafruit_NeoPixel ringPixels;
 

void initLeds();

void setLevel(int nPixel);

void setLevel(int nPixel, uint32_t color, bool clearBefore = true, uint8_t brightness = 255);

//void ledsFace(SensorState s);

void testLeds();




#endif // LEDS_H