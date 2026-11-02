#ifndef PREFS_H
#define PREFS_H


#include <Arduino.h> 

#include <Preferences.h>

extern Preferences preferences;


struct NetworkCredentials {
    String ssid;
    String password;
    String ssid2;
    String password2;
};


void prefResetCredentials();
void prefSetCredentials(String name, String pwd, int indexNetwork = 1); 
NetworkCredentials prefGetCredentials();

void changeCredentialMode(bool enable);
bool isCredentialModeEnabled();

void prefSetUrl(String url);
String prefGetUrl();

String prefGetDataCube();
String prefGetDayCube();

void prefSetAllCube(String stringDataCube, String day);

#endif // PREFS_H