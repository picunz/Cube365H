#include "lcd.h"

//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,  7, 6, U8X8_PIN_NONE);

void lcdInit()
{
    u8g2.begin();

    u8g2.setContrast(150); 

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
   u8g2.drawGlyph(x, y , code);
   u8g2.sendBuffer();
} 

void lcdDot(SensorState s)
{
   //lcdClearStr(1);
   u8g2.drawCircle(4,  16, 4);
   if (s.pressedL) {    
      u8g2.drawDisc(4,  16, 4);
   }
   else if (s.releasedL) {
      lcdEmptyDot(4,  16, 3);
   }

   u8g2.drawCircle(24,  16, 4);
   if (s.pressedR) {
      u8g2.drawDisc(24,  16, 4);
   }
   else if (s.releasedR) {
      lcdEmptyDot(24,  16, 3);
   }

   u8g2.drawCircle(44,  16, 4);
   if (s.pressedF) {
      u8g2.drawDisc(44,  16, 4);
   }
   else if (s.releasedF) {
      lcdEmptyDot(44,  16, 3);
   }

   u8g2.drawCircle(64,  16, 4);
   if (s.pressedB) {
      u8g2.drawDisc(64,  16, 4);
   }
   else if (s.releasedB) {
      lcdEmptyDot(64,  16, 3);
   }

   u8g2.drawCircle(84,  16, 4);
   if (s.pressedO) {
      u8g2.drawDisc(84,  16, 4);
   }
   else if (s.releasedO) {
      lcdEmptyDot(84,  16, 3);
   }

   u8g2.drawCircle(104,  16, 4);
   if (s.pressedT) {
      u8g2.drawDisc(104,  16, 4);
   }
   else if (s.releasedT) {
      lcdEmptyDot(104,  16, 3);
   }
   u8g2.sendBuffer();    
}