#include "ntp.h"

#include <string>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600; // Offset per l'ora (ad esempio, 3600 per GMT+1)
const int   daylightOffset_sec = 3600; // Offset per l'ora legale (se applicabile)

void initNtp()
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

String getDay() 
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    
    return "-";
  }
  
  //Serial.println(&timeinfo, "%B %d %Y");
  char buffer[50];
  strftime(buffer, sizeof(buffer), "%B %d %Y", &timeinfo);
  return String(buffer);
  
}