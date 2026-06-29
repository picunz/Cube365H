#ifndef PREFS_H
#define PREFS_H


#include <Arduino.h> 

#include <Preferences.h>

extern Preferences preferences;


struct NetworkCredentials {
    String ssid;
    String password;
};


void prefResetCredentials();
void prefSetCredentials(String name, String pwd);
NetworkCredentials prefGetCredentials();


void prefSetUrl(String url);
String prefGetUrl();

String prefGetDataCube();
String prefGetDayCube();

void prefSetAllCube(String stringDataCube, String day);

#endif // PREFS_H