#ifndef JSON_H
#define JSON_H


#include <Arduino.h> 

#include <ArduinoJson.h>

extern StaticJsonDocument<300> jsonCube;
 
extern DynamicJsonDocument* jsonTasks;

extern String _taskArray[8];
extern String _activityArray[8];


void initJsonTasks(size_t capacity);


void convertJsonInTasksArray(String jsonStr);
void convertJsonInActivitiesArray(String jsonStr);


void createjsonCube();

#endif // JSON_H