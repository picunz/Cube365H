
#include "activitiestasks.h"

void api_updateStateAndGetTasks(String deviceId, int cubeState)
{
  String stateString = CubeStateStrings[cubeState];

  String jresult = sendState(deviceId, stateString);

  Serial.println(jresult);

  convertJsonInTasksArray(jresult);
}

void api_GetTasks(String deviceId, int cubeState)
{
  String jresult = getTasks(deviceId);
  convertJsonInTasksArray(jresult);
}


void api_updateStateAndGetActivities(String deviceId,int cubeState)
{
  String stateString = CubeStateStrings[cubeState];
  String jresult = sendState(deviceId, stateString);

 // Serial.println(jresult);
  convertJsonInActivitiesArray(jresult);
}

void api_GetActivities(String deviceId)
{
  String jresult = getActivities(deviceId);
  convertJsonInActivitiesArray(jresult);
}


// utilities

bool splitTask(const String& task, String& part1, String& part2) 
{
  int sepIndex = task.indexOf(':');
  if (sepIndex == -1) return false;

  part1 = task.substring(0, sepIndex);
  part2 = task.substring(sepIndex + 1);
  return true;
}

void lcdDrawTask(String infoTask)
{
  //Serial.println("curTask:" + infoTask);

  String part1, part2;
  splitTask(infoTask, part1, part2);

  //Serial.println("Part1: " + part1 + "  Part2: " + part2);

  lcdClearStr(3);
  lcdClearStr(4);
  lcdDrawDbg(part1, 3);
  lcdDrawDbg(part2, 4);
}

void lcdDrawActivityTime(String activityTime)
{
  //Serial.println("Activity Time:" + activityTime);

  String part1, part2;
  splitTask(activityTime, part1, part2);

  //Serial.println("Part1: " + part1 + "  Part2: " + part2);

  //lcdClearStr(3);
  lcdClearStr(4);
  //lcdDrawDbg(part1, 3);
  lcdDrawDbg(part2, 4);
}

int getLevelByActivityPerc(int cubeState)
{
  String activity = _activityArray[cubeState];
      
  String perc, time;
  splitTask(activity, perc, time);
  
  int level = map(perc.toInt(), 0, 100, 1, 12);

  return level;
}

void lcdDrawFullStats(int i)
{
  int level = getLevelByActivityPerc(i);
  setLevel(level, ColorFace[i]);

  lcdClear();
  lcdDrawDbg(ColorFaceString[i], 0);

  String name, type, perc, time;
  splitTask(_taskArray[i], name, type);

  lcdDrawDbg(name, 1);
  lcdDrawDbg(type, 2);

  splitTask(_activityArray[i], perc, time);
  lcdDrawDbg(time, 3);

  Serial.println(_activityArray[i]);
}