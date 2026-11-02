#ifndef _WIFICONTROL_H_
#define _WIFICONTROL_H_

#include <Arduino.h> 
 

    extern int serverPort;
    extern String NetworkArray[8]; // 8 is limit
    extern int numberOfNetworkFound;
    extern bool serverStarted;

   
    int WiFi_scanner();

    void WiFi_accessPointSetup(String ssid, String pwd);

    void WiFi_processAccessPoint();

    bool WiFi_connectToAccessPoint(String ssid, String pwd, String ssid2 = "", String pwd2 = "");

    String WiFi_startWebServer();

    String WiFI_getMac();

#endif