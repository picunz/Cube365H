#include "battery.h"

#include "lcd.h"

void VextON()
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF()
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}

void initBattery()
{
    VextON();
	  delay(100);
    pinMode(ADC_Ctrl,OUTPUT);
    pinMode(VBAT_Read,INPUT);
    adcAttachPin(VBAT_Read);
    analogReadResolution(12);
}

float readBatteryVoltage() 
{
  // ADC resolution
  const int resolution = 12;
  const int adcMax = pow(2,resolution) - 1;
  const float adcMaxVoltage = 3.3;
  // On-board voltage divider
  const int R1 = 390;
  const int R2 = 100;
  // Calibration measurements
  const float measuredVoltage = 4.2;
  const float reportedVoltage = 4.095;
  // Calibration factor
  const float factor = (adcMaxVoltage / adcMax) * ((R1 + R2)/(float)R2) * (measuredVoltage / reportedVoltage); 
  digitalWrite(ADC_Ctrl,LOW);
  delay(100);
  int analogValue = analogRead(VBAT_Read);
  digitalWrite(ADC_Ctrl,HIGH);

  float floatVoltage = factor * analogValue;
  //uint16_t voltage = (int)(floatVoltage * 1000.0);

  return floatVoltage;
   
}

void showBatteryVoltage(float voltage)
{
  Serial.print("Battery voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  printAt(String(voltage), 100, 30);
}

void testBattery()
{
    while(1)
    {
      float voltage = readBatteryVoltage();

      Serial.print("Battery voltage: ");
      Serial.print(voltage);
      Serial.println(" V");

      printAt(String(voltage), 90, 30);
      delay(400);
    }
}
