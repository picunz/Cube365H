#ifndef ACTIVITIESTASKS_H
#define ACTIVITIESTASKS_H

#include <Arduino.h>

#include "utilities.h"
#include "webapi.h"
#include "json.h"
#include "lcd.h"
#include "leds.h"

void api_updateStateAndGetTasks(String deviceId, int cubeState);
void api_GetTasks(String deviceId, int cubeState);

void api_updateStateAndGetActivities(String deviceId,int cubeState);
void api_GetActivities(String deviceId);

// utilities
bool splitTask(const String& task, String& part1, String& part2);
void lcdDrawTask(String infoTask);
void lcdDrawActivityTime(String activityTime);
int getLevelByActivityPerc(int cubeState);
void lcdDrawFullStats(int i);

// debug functions
void showActivitiesArray();

#endif  // ACTIVITIESTASKS_H

