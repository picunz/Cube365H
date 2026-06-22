#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>
 


#define PIN_SENSOR_RECHARGEBASE 40

#define PIN_SENSOR_L 19  
#define PIN_SENSOR_F 4
#define PIN_SENSOR_B 3
#define PIN_SENSOR_T 5  
#define PIN_SENSOR_R 1

#define PIN_SENSOR_0 26

#define PIN_BUZZER 47
 


enum CubeState {
   LEFT =  0,
   RIGHT = 1,
   FRONT = 2,
   BACK =  3,
   BASE =  4,
   TOP =   5,
   MULTIPLE_SIDE = 6,
   NO_SIDE       = 7, 
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

extern int _changeFace;

extern int digiL, digiF, digiR, digiB, digiT, digi0;

//extern int OppositeState[3];
 
// *** functions 

void initSensors();

void testSensors();
void testSensorsAnalog();

// analog
SensorState getSensorValues(int sensorL, int sensorR, int sensorF, int sensorB, int sensorO, int sensorT);

// digital


void readSensors(bool debug);

SensorState getSensors(int sensorL, int sensorR, int sensorF, int sensorB, int sensorO, int sensorT);

int getCubeState(SensorState s);

int getNumberOfPressed(SensorState s);

int mapString2Enum(String side);

bool singleFaceActive(int state);

bool topFacePressed(int state);

// sensor recharge base

void testSensorBase();

bool isCharging();

void initWhiteLed();

void setWhiteLed(bool val);

// buzzer functions
void initBuzzer();

void buzzerBeep(int duration = 100);

void buzzerTone(int frequency, int duration);

void buzzerOff();


#endif //UTILITIES_H