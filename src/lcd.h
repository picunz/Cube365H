#ifndef LCD_H
#define LCD_H


#include <Arduino.h> 

#include <Wire.h> 
#include <U8g2lib.h>
 
#include <utilities.h>


#define ICON_WIFI           0xF7
#define ICON_NOCONNECT      0x107

void lcdInit();

void lcdClear();

void lcdDrawDbg(const String& str, int line, int xpos = 0);

void lcdDrawTitleDbg(const String& str, int line, int xpos = 0);

void lcdClearStr(int line);
 
void lcdEmptyDot(int x, int y, int r);

void drawIcon(int x, int y, int code);

void lcdDot(SensorState s);

#endif // LCD_H