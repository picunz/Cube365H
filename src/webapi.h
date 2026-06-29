#ifndef WEBAPI_H
#define WEBAPI_H

#include <Arduino.h>
#include <HTTPClient.h>

//#include <ArduinoJson.h>

// variables 

extern bool cmd_reset;
extern String ServerUrl;

// functions
 

String getTasks(String deviceId);    // db, not used

String getActivities(String deviceId);    // db

String sendState(String deviceId, String state);    // db

//void onServerResponse(const String& response);
 

String registerDevice(String deviceId);

//String getTasks(String deviceId); 

String sendLog(String deviceId, String level, String message, float value = 0.0);


#endif // WEBAPI_H