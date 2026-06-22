#include "json.h"

#include "utilities.h"

//const size_t capacity = 300;
StaticJsonDocument<300> jsonCube;

String _taskArray[8] = {"-", "-", "-", "-", "-", "-", "-", "-", };
String _activityArray[8] = {"-", "-", "-", "-", "-", "-", "-", "-", };

void createjsonCube()
{
   jsonCube["LEFT"] = "Left";
   jsonCube["C_LEFT"] = 0;
   jsonCube["RIGHT"] = "Right";
   jsonCube["C_RIGHT"] = 0;
   jsonCube["FRONT"] = "Front";
   jsonCube["C_FRONT"] = 0;
   jsonCube["BACK"] = "Back";
   jsonCube["C_BACK"] = 0;
   jsonCube["BASE"] = "Base";
   jsonCube["C_BASE"] = 0;
   jsonCube["TOP"] = "Top";
   jsonCube["C_TOP"] = 0;
}

 
DynamicJsonDocument* jsonTasks = nullptr;

void initJsonTasks(size_t capacity) 
{
    if (jsonTasks) {
        delete jsonTasks;
    }
    jsonTasks = new DynamicJsonDocument(capacity);
}


void convertJsonInTasksArray(String jsonStr)
{
    initJsonTasks(JSON_OBJECT_SIZE(6) + 6 * 6);

    DeserializationError error = deserializeJson(*jsonTasks, jsonStr);

    JsonArray _taskList = (*jsonTasks)["RowTasks"].as<JsonArray>();

    Serial.println("Task List:");
    for (JsonObject obj : _taskList) {
        String side = obj["Side"];           
        String name = obj["Name"]; 
        String type = obj["Type"]; 

        Serial.print(" Side: ");
        Serial.print(side);
        Serial.print(" Name: ");
        Serial.print(name);
        Serial.print(" Type: ");
        Serial.println(type);

        int idx = mapString2Enum(side);

        _taskArray[idx] = name + ":" + type;
    }
}

void convertJsonInActivitiesArray(String jsonStr)
{
    initJsonTasks(JSON_OBJECT_SIZE(6) + 6 * 6);

    DeserializationError error = deserializeJson(*jsonTasks, jsonStr);

    Serial.println("Activity List:");
    JsonArray _taskList = (*jsonTasks)["jActivities"].as<JsonArray>();
    for (JsonObject obj : _taskList) {
        String side = obj["Side"];           
        String perc = obj["Perc"]; 
        //String type = obj["Type"];
        String time = obj["ElapsedTime"]; 
 
        Serial.print(" Side: " + side);
        Serial.print(" Perc: " + perc);
        //Serial.print(" Type: " + type);
        Serial.println(" Elapsed: " + time);
        
        int idx = mapString2Enum(side);
        
        if (idx >= 0) {
            _activityArray[idx] = perc + ":" + time;
            //Serial.println(" _activityArray[idx] : " + _activityArray[idx]);
        }
    }

}



