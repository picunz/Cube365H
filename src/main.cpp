#include <Arduino.h>
#include <Wire.h> 
#include <heltec.h>
 


 
unsigned long prevMillis_sendCloud = 0;
unsigned long prevMillis_gpsTest = 0;
unsigned long preMillis_periodic = 0;
 
double gpsLat;
double gpsLng;
 
double gpsLat_prec = 0;
double gpsLng_prec = 0;
bool gpsIsValid = false; 

int indexUploading = 0;
long interval = 0;
int counterTest = 0;
int counterUserButton = 0;
 

void setup()
{
  // setup display
  //Heltec.begin(true, false, false); // Display Enable, LoRa Disable, Serial Disable
  //Heltec.display->flipScreenVertically();
  //Heltec.display->setFont(ArialMT_Plain_10);

  Serial.begin(115200);
  
  
  delay(1500);

  Serial.println("\n\n\n"); 
 
    
 
  Serial.println("Helec test");
/*
  Heltec.display->drawString(0, 20, "File config not found");
  Heltec.display->drawString(0, 30, "Hot Spot mode");
  Heltec.display->display();
*/
       
}

void loop() 
{
  
}
