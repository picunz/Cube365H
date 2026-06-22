#ifndef _WIFICONTROL_H_
#define _WIFICONTROL_H_

#include <Arduino.h> 
 

    extern int serverPort;
    extern String NetworkArray[8]; // 8 is limit
    extern int numberOfNetworkFound;

   
    int WiFi_scanner();

    void WiFi_accessPointSetup(String ssid, String pwd);

    bool WiFi_connectToAccessPoint(String ssid, String pwd);

    String WiFi_startWebServer();

    String WiFI_getMac();

#endif