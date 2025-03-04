#include "utilities.h"
#include <string>
//CubeState _cubeState;

String CubeStateStrings[] = {
    "LEFT",
    "RIGHT",
    "FRONT",
    "BACK", 
    "BASE", 
    "TOP",  
    "MULTIPLE_SIDE",
    "NO_SIDE",
};

SensorState getSensorValues(int sensorL, int sensorR, int sensorF, int sensorB, int sensorO, int sensorT)
{
   SensorState sensorState;
   if (sensorL > TH_ADC_H) {
      sensorState.pressedL = true;
      sensorState.releasedL = false;
   }
   else if (sensorL < TH_ADC_L) {
      
      if (sensorState.pressedL) {
         sensorState.releasedL = true;
      }
      sensorState.pressedL = false;
   }


   if (sensorR > TH_ADC_H) {
      sensorState.pressedR = true;
      sensorState.releasedR = false;
   }
   else if (sensorR < TH_ADC_L) {
      
      if (sensorState.pressedR) {
         sensorState.releasedR = true;
      }
      sensorState.pressedR = false;
   }

   if (sensorF > TH_ADC_H) {
      sensorState.pressedF = true;
      sensorState.releasedF = false;
   }
   else if (sensorF < TH_ADC_L) {
      
      if (sensorState.pressedF) {
         sensorState.releasedF = true;
      }
      sensorState.pressedF = false;
   }

   if (sensorB > TH_ADC_H) {
      sensorState.pressedB = true;
      sensorState.releasedB = false;
   }
   else if (sensorB < TH_ADC_L) {
      
      if (sensorState.pressedB) {
         sensorState.releasedB = true;
      }
      sensorState.pressedB = false;
   }

   if (sensorO > TH_ADC_H) {
      sensorState.pressedO = true;
      sensorState.releasedO = false;
   }
   else if (sensorO < TH_ADC_L) {
      
      if (sensorState.pressedO) {
         sensorState.releasedO = true;
      }
      sensorState.pressedO = false;
   }

   if (sensorT > TH_ADC_H) {
      sensorState.pressedT = true;
      sensorState.releasedT = false;
   }
   else if (sensorT < TH_ADC_L) {
      
      if (sensorState.pressedT) {
         sensorState.releasedT = true;
      }
      sensorState.pressedT = false;
   }
   return sensorState;
} 

int getCubeState(SensorState s)
{
    
    int sum = s.pressedL + s.pressedR + s.pressedF + s.pressedB + s.pressedO + s.pressedT;
    if (sum == 0) {
        return (int)CubeState::NO_SIDE;
    }
    if (sum > 1)
    {
        return (int)CubeState::MULTIPLE_SIDE;
    }
    else
    {
        if (s.pressedL) {
            return (int)CubeState::LEFT;
        }
        else if (s.pressedR) {
            return (int)CubeState::RIGHT;
        }
        else if (s.pressedF) {
            return (int)CubeState::FRONT;
        }
        else if (s.pressedB) {
            return (int)CubeState::BACK;
        }
        else if (s.pressedO) {
            return (int)CubeState::BASE;
        }
        else if (s.pressedT) {
            return (int)CubeState::TOP;
        }
    }

    return sum;
}


int mapString2Enum(String side)
{
   if (side == "LEFT") {
      return (int) CubeState::LEFT;
   }
   else if (side == "RIGHT") {
      return (int) CubeState::RIGHT;
   }
   else if (side == "FRONT") {
      return (int) CubeState::FRONT;
   }
   else if (side == "BACK") {
      return (int) CubeState::BACK;
   }
   else if (side == "BASE") {
      return (int) CubeState::BASE;
   }
   else if (side == "TOP") {
      return (int) CubeState::TOP;
   }
   else {
      return -1;
   }
}