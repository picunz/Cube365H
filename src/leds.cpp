#include "leds.h"



Adafruit_NeoPixel ringPixels;
 

uint32_t colors[PIXELS_LEN] = {
  ringPixels.Color(10, 0, 0),
  ringPixels.Color(50, 0, 0),
  ringPixels.Color(100, 0, 0),
  ringPixels.Color(255, 0, 0),
  ringPixels.Color(0, 0, 10),
  ringPixels.Color(0, 0, 50),
  ringPixels.Color(0, 0, 150),
  ringPixels.Color(0, 0, 255),
  ringPixels.Color(0, 10, 0),
  ringPixels.Color(0,  50, 0),
  ringPixels.Color(0, 100, 0),
  ringPixels.Color(0, 255, 0)
};

uint32_t ColorFace[8] = {

   COLOR_FACE_RED,    //LEFT
   COLOR_FACE_BLUE,   //RIGHT = 1,
   COLOR_FACE_YELLOW, //FRONT = 2,
   COLOR_FACE_GREEN,  //BACK =  3,
   COLOR_FACE_ORANGE, //BASE =  4,
   COLOR_FACE_VIOLET, //TOP =   5,
   COLOR_FACE_WHITE,  //MULTIPLE_SIDE = 6,
   COLOR_FACE_WHITE  //NO_SIDE       = 7, 
};

String ColorFaceString[8] {
   "Red",
   "Blue",
   "Yellow",
   "Green",
   "Orange",
   "Violet",
   "White",
   "White"
};

void initLeds()
{
  ringPixels.updateType(NEO_GRB + NEO_KHZ800);
  ringPixels.updateLength(12);
  ringPixels.setPin(48);
  ringPixels.begin();
  ringPixels.show();   
}

void setLevel(int nPixel)
{
   ringPixels.begin();
   ringPixels.show();
   ringPixels.setBrightness(255);

   ringPixels.clear();

   for(int i =0; i < nPixel; i++)
   {
      ringPixels.setPixelColor(i, colors[i]);

      ringPixels.show();

      delay(50);
   }
}

void setLevel(int nPixel, uint32_t color)
{
   ringPixels.begin();
   ringPixels.show();
   ringPixels.setBrightness(255);

   ringPixels.clear();

   for(int i =0; i < nPixel; i++)
   {
      ringPixels.setPixelColor(i, color);

      ringPixels.show();

      //delay(50);
   }
}

/*
void ledsFace(SensorState s)
{
   if (s.pressedL) {
      setLevel(4, ringPixels.Color(255, 0, 0));
   }
   else if (s.pressedR) {
      setLevel(4, ringPixels.Color(0, 255, 0));
   }
   else if (s.pressedF) {
      setLevel(4, ringPixels.Color(0, 0, 255));
   }
   else if (s.pressedB) {
      setLevel(4, ringPixels.Color(255, 255, 0));
   }
   else if (s.pressedT) {
      setLevel(4, ringPixels.Color(0, 255, 255));
   }
   else if (s.pressedO) {
      setLevel(4, ringPixels.Color(255, 0, 255));
   }
   else {
      setLevel(4, ringPixels.Color(255, 255, 255));
   }
}
*/

void testLeds()
{
   ringPixels.begin();
   ringPixels.show();
   ringPixels.setBrightness(255);
   while(1)
   {
      ringPixels.clear();
      for(int i=0; i < PIXELS_LEN; i++) {

         ringPixels.setPixelColor(i, colors[i]);

         ringPixels.show();

         delay(500);
      }
   }
}

