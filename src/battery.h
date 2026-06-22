#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

#define VBAT_Read    1
#define	ADC_Ctrl    37

void VextON();

void VextOFF();

void initBattery();

float readBatteryVoltage();

void showBatteryVoltage(float voltage);

void testBattery();

#endif  // BATTERY_H