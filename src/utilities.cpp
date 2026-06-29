#include "utilities.h"

#include "lcd.h"

#include "leds.h"

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


//int OppositeState[3] = {0,1,0};
int _changeFace = -1;


int digiL, digiF, digiR, digiB, digiT, digi0;

void IRAM_ATTR handleInterrupt() {
  //statoCambiato = true;
  _changeFace = 1;
}

void initSensors()
{
   pinMode(PIN_SENSOR_L, INPUT);
   pinMode(PIN_SENSOR_F, INPUT);
   pinMode(PIN_SENSOR_B, INPUT);
   pinMode(PIN_SENSOR_T, INPUT);
   pinMode(PIN_SENSOR_R, INPUT_PULLDOWN);
   pinMode(PIN_SENSOR_0, INPUT);
/*
   attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_L), handleInterrupt, CHANGE);
   attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_F), handleInterrupt, CHANGE);
   attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_B), handleInterrupt, CHANGE);
   attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_T), handleInterrupt, CHANGE);
   attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_R), handleInterrupt, CHANGE);
   attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_0), handleInterrupt, CHANGE);
*/
   pinMode(PIN_SENSOR_RECHARGEBASE, INPUT);
}

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

bool singleFaceActive(int state)
{
   return (state != (int)CubeState::MULTIPLE_SIDE && 
           state != (int)CubeState::NO_SIDE);
}

bool topFacePressed(int state)
{
  bool pressed = (state == (int)CubeState::BASE && digiT == HIGH ||
                  state == (int)CubeState::LEFT && digiR == HIGH ||
                  state == (int)CubeState::RIGHT && digiL == HIGH ||
                  state == (int)CubeState::TOP && digi0 == HIGH ||
                  state == (int)CubeState::FRONT && digiB == HIGH ||
                  state == (int)CubeState::BACK && digiF == HIGH);

  return pressed;
}


void readSensors(bool debug = false)
{
  digiL = digitalRead(PIN_SENSOR_L);
  digiF = digitalRead(PIN_SENSOR_F);
  digiB = digitalRead(PIN_SENSOR_B);
  digiT = digitalRead(PIN_SENSOR_T);
  digiR = digitalRead(PIN_SENSOR_R);
  digi0 = digitalRead(PIN_SENSOR_0);

  if (debug) {
    Serial.println( " L:" + String(digiL) +
                    " F:" + String(digiF) +
                    " B:" + String(digiB) + 
                    " T:" + String(digiT) + 
                    " R:" + String(digiR) + 
                    " 0:" + String(digi0));
  }
  
   
}
 

   

SensorState getSensors(int sensorL, int sensorR, int sensorF, int sensorB, int sensorO, int sensorT)
{
   SensorState sensorState;
   if (sensorL == HIGH) {
      sensorState.pressedL = true;
      sensorState.releasedL = false;
   }
   else if (sensorL == LOW) {
      
      if (sensorState.pressedL) {
         sensorState.releasedL = true;
      }
      sensorState.pressedL = false;
   }


   if (sensorR == HIGH) {
      sensorState.pressedR = true;
      sensorState.releasedR = false;
   }
   else if (sensorR == LOW) {
      
      if (sensorState.pressedR) {
         sensorState.releasedR = true;
      }
      sensorState.pressedR = false;
   }

   if (sensorF == HIGH) {
      sensorState.pressedF = true;
      sensorState.releasedF = false;
   }
   else if (sensorF == LOW) {
      
      if (sensorState.pressedF) {
         sensorState.releasedF = true;
      }
      sensorState.pressedF = false;
   }

   if (sensorB == HIGH) {
      sensorState.pressedB = true;
      sensorState.releasedB = false;
   }
   else if (sensorB == LOW) {
      
      if (sensorState.pressedB) {
         sensorState.releasedB = true;
      }
      sensorState.pressedB = false;
   }

   if (sensorO == HIGH) {
      sensorState.pressedO = true;
      sensorState.releasedO = false;
   }
   else if (sensorO == LOW) {
      
      if (sensorState.pressedO) {
         sensorState.releasedO = true;
      }
      sensorState.pressedO = false;
   }

   if (sensorT == HIGH) {
      sensorState.pressedT = true;
      sensorState.releasedT = false;
   }
   else if (sensorT == LOW) {
      
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

int getNumberOfPressed(SensorState s)
{
   int sum = s.pressedL + s.pressedR + s.pressedF + s.pressedB + s.pressedO + s.pressedT;
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

 
bool isCharging()
{
   pinMode(PIN_SENSOR_RECHARGEBASE, INPUT_PULLDOWN);

   return (digitalRead(PIN_SENSOR_RECHARGEBASE) == HIGH);
}

void initWhiteLed()
{
   pinMode(LED, OUTPUT); // white led
}

void setWhiteLed(bool val)
{
   digitalWrite(LED, val);
}

// buzzer functions
void initBuzzer()
{
   pinMode(PIN_BUZZER, OUTPUT);
   digitalWrite(PIN_BUZZER, LOW);
}

void buzzerBeep(int duration)
{
   digitalWrite(PIN_BUZZER, HIGH);
   delay(duration);
   digitalWrite(PIN_BUZZER, LOW);
}

void buzzerTone(int frequency, int duration)
{
   tone(PIN_BUZZER, frequency, duration);
}

void buzzerOff()
{
   noTone(PIN_BUZZER);
   digitalWrite(PIN_BUZZER, LOW);
}


// *** Logs section ***

void log(String deviceId, String level, String message, float value)
{
   Serial.println(message);

   if (WiFi.status() == WL_CONNECTED) {

      sendLog(deviceId, level, message, value);
   }
}

// *** Tests section ***



void testSensors()
{
   Serial.println("testCNY70");

   lcdClearStr(0); 
   
   while(1)
   {
      int digiL = digitalRead(PIN_SENSOR_L);
      int digiF = digitalRead(PIN_SENSOR_F);
      int digiB = digitalRead(PIN_SENSOR_B);
      int digiT = digitalRead(PIN_SENSOR_T);
      int digiR = digitalRead(PIN_SENSOR_R);
      int digi0 = digitalRead(PIN_SENSOR_0);

      SensorState s = getSensors(digiL, digiR, digiF, digiB, digi0, digiT);


      lcdDot(s, 16);

      //ledsFace(s);

   
      delay(50);
   }
}

void testSensorsAnalog()
{
   Serial.println("testCNY70");

   lcdClearStr(0); 
   
   while(1)
   {
      int digiL = analogRead(PIN_SENSOR_L);
      int digiF = analogRead(PIN_SENSOR_F);
      int digiB = analogRead(PIN_SENSOR_B);
      int digiT = analogRead(PIN_SENSOR_T);
      int digiR = analogRead(PIN_SENSOR_R);
      //int digi0 = analogRead(PIN_SENSOR_0);

      Serial.printf("L:%4d  F:%4d  B:%4d  T:%4d  R:%4d\n", 
                    digiL, digiF, digiB, digiT, digiR);

      //ledsFace(s);

   
      delay(100);
   }
}

void testSensorsWithLeds()
{
   Serial.println("testCNY70 and neopixel leds");

   initBuzzer();

   lcdClearStr(0); 
   
   while(1)
   {
      int digiL = digitalRead(PIN_SENSOR_L);
      int digiF = digitalRead(PIN_SENSOR_F);
      int digiB = digitalRead(PIN_SENSOR_B);
      int digiT = digitalRead(PIN_SENSOR_T);
      int digiR = digitalRead(PIN_SENSOR_R);
      int digi0 = digitalRead(PIN_SENSOR_0);

      SensorState s = getSensors(digiL, digiR, digiF, digiB, digi0, digiT);


      lcdDot(s, 16);

      //ledsFace(s);

      int cubeState = getCubeState(s);

      setLevel(12, ColorFace[cubeState], false, 100);

      if (cubeState == CubeState::NO_SIDE) {
         buzzerTone(1000, 100);
      }
      else if (cubeState == CubeState::MULTIPLE_SIDE) {
         buzzerTone(2000, 100);
      }
      else {
         buzzerOff();
      }      

   
      delay(50);
   }
}

void testMagSensor()
{
  pinMode(PIN_SENSOR_RECHARGEBASE, INPUT_PULLDOWN);

  lcdClearStr(0); 
  lcdDrawDbg("Mag Sensor Test", 0);
  while(1)
  {
    int pin = digitalRead(PIN_SENSOR_RECHARGEBASE);
    if (pin == HIGH) {
      Serial.println("Charging");
      lcdDrawDbg("          ", 1);
      lcdDrawDbg("Charging", 1);
    }
    else {
      lcdDrawDbg("          ", 1);
      lcdDrawDbg("Not Charging", 1);
    }
    Serial.println(pin);
    delay(50);
  }
}