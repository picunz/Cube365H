
#include <Arduino.h>
#include "lcd.h"
 
#include "webapi.h"

#include "utilities.h"

#include "prefs.h"

#include "wifiControl.h"

#include "json.h"

#include "ntp.h"

#include "battery.h"

#include "config.h"

#include "leds.h"

//#include <Adafruit_NeoPixel.h>




#define BOOT_BUTTON_PIN 0 // GPIO collegato al pulsante BOOT
volatile bool bootPressed = false;
// Funzione ISR (Interrupt Service Routine)
void IRAM_ATTR handleButtonPress() 
{
  bootPressed = true;
}

bool _ApConnected = false;
String _curDay = "";
int _lastState = -1;

int counter = 0;
unsigned long prevMillis_periodic = 0;
unsigned long prevMillis_periodic_sec = 0;
unsigned long prevMillis_periodic_update = 0;
unsigned long prevMillis_periodic_step = 0;
unsigned long prevMillis_periodic_up = 0;

String _mac = "";

//int digiL, digiF, digiR, digiB, digiT, digi0;
 
float _batteryVolt = 0;




static int prevState = -1;
static int cubeState;

bool delayUp = false;

int indexdStatPart = 0;
 
int counter_up = 0;

int Milli_Periodic_Update = 10000;  // 10 sec
int Milli_Periodic_Check = 50;






bool splitTask(const String& task, String& part1, String& part2) 
{
  int sepIndex = task.indexOf(':');
  if (sepIndex == -1) return false;

  part1 = task.substring(0, sepIndex);
  part2 = task.substring(sepIndex + 1);
  return true;
}

void api_updateStateAndGetTasks(int cubeState)
{
  String stateString = CubeStateStrings[cubeState];
  String jresult = sendState(stateString, _mac);

  Serial.println(jresult);

  convertJsonInTasksArray(jresult);
}

void api_GetTasks(int cubeState)
{
  String jresult = getTasks(_mac);
  convertJsonInTasksArray(jresult);
}

void api_GetActivities()
{
  String jresult = getActivities(_mac);
  convertJsonInActivitiesArray(jresult);
}

void api_updateStateAndGetActivities(int cubeState)
{
  String stateString = CubeStateStrings[cubeState];
  String jresult = sendState(stateString, _mac);

 // Serial.println(jresult);
  convertJsonInActivitiesArray(jresult);
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

void hotSpotMode()
{
  Serial.println("No nets saved");

  lcdClearStr(0); 
  lcdDrawDbg("No nets saved", 0);

  lcdClearStr(1); 
  lcdDrawDbg("Hotspot mode", 1);

  lcdClearStr(2); 
  lcdDrawDbg("Scanning networks...", 2);
  numberOfNetworkFound = WiFi_scanner();

  for(int i = 0; i < 8; i++)
  {
      Serial.println(NetworkArray[i]);
  }
  
  lcdClearStr(2);
  String resultScan = "Found " + String(numberOfNetworkFound) + " networks";
  lcdDrawDbg(resultScan, 2);

  WiFi_accessPointSetup(AP_SSID, AP_PASSWORD);

  lcdClearStr(2);
  lcdDrawDbg(String(AP_SSID), 2);

  lcdClearStr(3);
  lcdDrawDbg(String(AP_PASSWORD), 3);


  String apUrl = WiFi.softAPIP().toString() + ":" + String(SERVER_PORT);
  Serial.print("AP IP Address: ");
  Serial.println(apUrl);
  lcdClearStr(5);
  lcdDrawDbg(apUrl, 5);


  while(1);
}

  
void setup() 
{

   //attachInterrupt(digitalPinToInterrupt(BOOT_BUTTON_PIN), handleButtonPress, FALLING);

  Serial.begin(115200);
  while (!Serial) {
    
    delay(10);
  }
  Serial.println("Serial initialized");


  initLeds();

  
  
 
  

  lcdInit();
  delay(100);


  lcdClearStr(0); 
  lcdDrawDbg("CUBE 365", 0, 40);


  initBattery();

  initBuzzer();
  buzzerTone(1000, 1500);

  initSensors();

  //testLeds();

 
  //testBattery();

  //testSensors();

  //testSensorsAnalog();

  //testSensorBase();
  

  initWhiteLed();

  setWhiteLed(false);

 
   

  NetworkCredentials creds = prefGetCredentials();

  Serial.println(creds.ssid);

  Serial.println("networkName:" + creds.ssid);
  Serial.println("NetworkPwd:" + creds.password);

  bool noNetworkSaved = (creds.ssid.length() == 0);
  if (noNetworkSaved) 
  {
    hotSpotMode();
  }
  else 
  {
    bool connected = WiFi_connectToAccessPoint(creds.ssid, creds.password);

    if(connected)
    {
      _ApConnected = true;

      //lcdDrawDbg("Connected", 3);

      drawIcon(10, 20, ICON_WIFI);

      //drawIconBattery(112, 20, 0);

      _batteryVolt = readBatteryVoltage();
      showBatteryVoltage(_batteryVolt);


      int level = map(_batteryVolt, 0, 4.5, 0, 6);
      drawIconBattery(112, 20, level);
      

/*
      // get ip and show it
      String ipAddress = WiFi_startWebServer();
      lcdClearStr(3); 
      lcdDrawDbg("Ip:  " + ipAddress, 3);

      // mac as device id and show it
      _mac = WiFI_getMac();
      lcdClearStr(5); 
      lcdDrawDbg("Mac: " + _mac, 4);
*/
      _mac = WiFI_getMac();

      // debug
      String url = prefGetUrl();
      ServerUrl = (url == "") ? "http://localhost:8080/" : url;

      ServerUrl = "https://www.kiand.it/cube365/";

      //ServerUrl = "https://70aa68a8cb62.ngrok-free.app/";

      //ServerUrl = "http://192.168.1.4:8080/";
      

      Serial.println("ServerUrl:" + ServerUrl);

      // register device
      Serial.println("register device");
      String responseReg = registerDevice(_mac);
      responseReg.trim();
      Serial.print("responseReg:");
      Serial.println(responseReg);

      // get task
      /*
      Serial.println("getTasks");
      String response = getTasks(_mac);
      Serial.println("response:" + response);
      */

      SensorState s = getSensors(digiL, digiR, digiF, digiB, digi0, digiT);
      int state = getCubeState(s);
      api_updateStateAndGetTasks(state);

      initNtp();

      String strDataCube = prefGetDataCube();

      if (strDataCube.isEmpty()) {
          Serial.println("dataCube not present, create it");
          createjsonCube();

          serializeJson(jsonCube, strDataCube);

          Serial.println("jsonString:");
          Serial.println(strDataCube);

          // save string in prefs
          _curDay = getDay();
          prefSetDataCube(strDataCube, _curDay);
      }

      Serial.println("string dataCube: " + strDataCube);
      Serial.println("convert string dataCube in jsonCube");
        
      deserializeJson(jsonCube, strDataCube);
    }
    else 
    {
      Serial.println("No AP found");

      lcdClearStr(3); 
      lcdDrawDbg("No AP found", 3);
      delay(2000);
    }
  }

 
}


void loop() 
{
  unsigned long mill = millis();
/*
  if ( (millis() - prevMillis_periodic) > Milli_Periodic_Update) 
  {
    prevMillis_periodic = mill;

    _batteryVolt = readBatteryVoltage();
    showBatteryVoltage(_batteryVolt);


    int level = map(_batteryVolt, 0, 4.5, 0, 6);
    drawIconBattery(112, 20, level);

    api_updateStateAndGetTasks(cubeState);
  }
*/
  if ( (millis() - prevMillis_periodic_up) > Milli_Periodic_Check)
  {
    prevMillis_periodic_up = mill;

    counter++;

    readSensors(false); // false is no debug
                    
    SensorState s = getSensors(digiL, digiR, digiF, digiB, digi0, digiT);
      
    lcdDot(s, 56);

    cubeState = getCubeState(s);
    
    //Serial.println(String(counter));

   
    if (cubeState == CubeState::NO_SIDE) {

      counter_up++;
      Serial.println(String(counter_up));
      if (counter_up > Milli_Periodic_Check)
      {
        Serial.println("Statistics");

        //setLevel(6, ColorFace[cubeState]);

        api_GetActivities();

        
        lcdDrawFullStats(indexdStatPart % 6);

        indexdStatPart++;

        delayUp = false;
        //counter_up = 0;
      }
    }

    if (cubeState == CubeState::MULTIPLE_SIDE) {

      if (getNumberOfPressed(s) > 2) {

        Serial.println("Draw Info");

        lcdClearStr(3); 
        lcdClearStr(4); 
        if (_ApConnected) {
          // get ip and show it
          String ipAddress = WiFi_startWebServer();
          lcdDrawDbg("Ip:  " + ipAddress, 3);

          // mac as device id and show it
          _mac = WiFI_getMac();
          lcdDrawDbg("Mac: " + _mac, 4);
        }
        else {
          lcdDrawDbg("No connect", 3);
        }
      }
    }

    if (prevState == CubeState::NO_SIDE && cubeState != CubeState::NO_SIDE && counter_up > Milli_Periodic_Check) {
      Serial.println("Return");

      indexdStatPart = 0;
      counter_up = 0;

      lcdClear();
      lcdDrawDbg("CUBE 365", 0, 40);
      drawIcon(10, 20, ICON_WIFI);
      drawIconBattery(112, 20, 0);
    }
/*
    if (topFacePressed(prevState)) {
      Serial.println("Top face " + String(prevState));
      
      api_GetActivities();

      String curInfo = _activityArray[prevState];
      
      lcdDrawActivityTime(curInfo);
      
    }
*/
    if (prevState != cubeState && singleFaceActive(cubeState)) {

      Serial.println("Change to " + CubeStateStrings[cubeState]);
      Serial.println("From " + CubeStateStrings[prevState]);

      counter_up = 0;

      // get perc
      int level = getLevelByActivityPerc(cubeState);
      
      setLevel(level, ColorFace[cubeState]);
      
      String curTask = _taskArray[cubeState];
      lcdDrawTask(curTask);
      
    }
/*
    if (isCharging()) {
     
      int level = map(_batteryVolt*100, 300, 480, 1, 12);
      Serial.println("_batteryVolt:" + String(_batteryVolt*100));
      Serial.println(level);
      setLevel(level);
    }
*/
    prevState = cubeState;

  }
}
