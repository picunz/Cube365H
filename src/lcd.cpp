#include "lcd.h"
 
#include <Wire.h>
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 18, 17, 21);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 21);



void lcdInit()
{
   Wire.begin(17, 18); // SDA, SCL

    u8g2.begin();

    //u8g2.setContrast(150); 

    u8g2.clear();
}

void lcdClear()
{
   u8g2.clearBuffer();
   u8g2.sendBuffer();
}

void lcdDrawTitleDbg(const String& str, int line, int xpos)
{
   u8g2.setFont(u8g2_font_logisoso20_tr);
   u8g2.drawStr(xpos, (line+1)*20, str.c_str());
   u8g2.sendBuffer();
}   
 
void lcdDrawDbg(const String& str, int line, int xpos)
{
   u8g2.setFont(u8g2_font_ncenB08_tr);
   u8g2.drawStr(xpos, (line+1) * 10, str.c_str());
   u8g2.sendBuffer();
}

void lcdClearStr(int line) 
{
   u8g2.setDrawColor(0);  
   u8g2.drawBox(0, line*10, 128, 12);
   u8g2.setDrawColor(1); 
   u8g2.sendBuffer();    
}
 
void lcdEmptyDot(int x, int y, int r)
{
   u8g2.setDrawColor(0);       
   u8g2.drawDisc(x, y, r);
   u8g2.setDrawColor(1);   
} 

void drawIcon(int x, int y, int code)
{
   u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
   u8g2.drawGlyph(x, y, code);
   u8g2.sendBuffer();
}

// level 0-6
void drawIconBattery(int x, int y, int level)
{
   //u8g2.setFont(u8g2_font_battery24_tr);
   u8g2.setFont(u8g2_font_battery19_tn);

   int code = ICON_BATTERY;

   u8g2.drawGlyph(x, y, code + level);
   u8g2.sendBuffer();
}


void printAt(const String& str, int x, int y)
{
   u8g2.setFont(u8g2_font_ncenB08_tr);

   int textWidth = u8g2.getStrWidth(str.c_str());
   int textHeight = u8g2.getMaxCharHeight();
   u8g2.setDrawColor(0);  
   u8g2.drawBox(x, y - textHeight, textWidth, textHeight);
   u8g2.setDrawColor(1);  


   u8g2.setCursor(x, y);
   u8g2.print(str.c_str());
   u8g2.sendBuffer();    


} 

void lcdDot(SensorState s, int y)
{
   u8g2.setFont(u8g2_font_ncenB08_tr);
   u8g2.setCursor(0, y + 5);
   u8g2.print("L");
    
   u8g2.setCursor(20, y + 5);
   u8g2.print("R");

   u8g2.setCursor(40, y + 5);
   u8g2.print("F");

   u8g2.setCursor(60, y + 5);
   u8g2.print("B");

   u8g2.setCursor(80, y + 5);
   u8g2.print("0");

   u8g2.setCursor(100, y + 5);
   u8g2.print("T");

   //lcdClearStr(1);
   u8g2.drawCircle(12, y, 4);
   if (s.pressedL) {    
      u8g2.drawDisc(12, y, 4);
   }
   else if (s.releasedL) {
      lcdEmptyDot(12, y, 3);
   }

   u8g2.drawCircle(32, y, 4);
   if (s.pressedR) {
      u8g2.drawDisc(32, y, 4);
   }
   else if (s.releasedR) {
      lcdEmptyDot(32, y, 3);
   }

   u8g2.drawCircle(52, y, 4);
   if (s.pressedF) {
      u8g2.drawDisc(52, y, 4);
   }
   else if (s.releasedF) {
      lcdEmptyDot(52, y, 3);
   }

   u8g2.drawCircle(72, y, 4);
   if (s.pressedB) {
      u8g2.drawDisc(72, y, 4);
   }
   else if (s.releasedB) {
      lcdEmptyDot(72, y, 3);
   }

   u8g2.drawCircle(92, y, 4);
   if (s.pressedO) {
      u8g2.drawDisc(92, y, 4);
   }
   else if (s.releasedO) {
      lcdEmptyDot(92, y, 3);
   }

   u8g2.drawCircle(112, y, 4);
   if (s.pressedT) {
      u8g2.drawDisc(112, y, 4);
   }
   else if (s.releasedT) {
      lcdEmptyDot(112, y, 3);
   }
   u8g2.sendBuffer();    
}