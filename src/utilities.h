#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

enum CubeState {
   LEFT = 0,
   RIGHT,
   FRONT,
   BACK,
   BASE,
   TOP,
   MULTIPLE_SIDE,
   NO_SIDE, 
};

struct SensorState {
   bool pressedL;
   bool pressedR;
   bool pressedF;
   bool pressedB;
   bool pressedO;
   bool pressedT;

   bool releasedL;
   bool releasedR;
   bool releasedF;
   bool releasedB;
   bool releasedO;
   bool releasedT;
};



#define TH_ADC_H 500
#define TH_ADC_L 200


extern String CubeStateStrings[];

extern CubeState _cubeState;
 
//** function */

SensorState getSensorValues(int sensorL, int sensorR, int sensorF, int sensorB, int sensorO, int sensorT);

int getCubeState(SensorState s);

int mapString2Enum(String side);

#endif //UTILITIES_H