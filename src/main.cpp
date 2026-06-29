
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

#include "activitiestasks.h"

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
unsigned long prevMillis_periodic_chr = 0;
unsigned long prevMillis_periodic_battery = 0;
unsigned long prevMillis_periodic_update = 0;
unsigned long prevMillis_periodic_step = 0;

unsigned long prevMillis_waiting_presseds = 0;

unsigned long prevMillis_periodic_up = 0;


int Milli_Periodic_Update = 10000;    // 10 sec
int Milli_Periodic_Check = 50;        // 50 msec
int Milli_Periodic_CheckCharging = 1000;  // 1 sec
int Milli_Periodic_Battery = 12000;       // 12 sec

int Milli_Waiting_Pressed = 300;  // 300 msec


String _mac = "";

//int digiL, digiF, digiR, digiB, digiT, digi0;
 
float _batteryVolt = 0;




static int prevState = -1;
static int cubeState;

static int prevFace = -1;
static int cubeFace;


bool delayUp = false;

int indexdStatPart = 0;
 
int counter_up = 0;

int counter_recharge = 0;

bool showInfoNet = false;
bool configNet = false;

bool topFaceActive = false;
bool chargingActive = false;
 

void hotSpotMode()
{
 

  WiFi_accessPointSetup(AP_SSID, AP_PASSWORD);




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

  //testSensorsWithLeds();

  //testSensorsAnalog();

  //testMagSensor();
  

  initWhiteLed();

  setWhiteLed(false);

 
   

  NetworkCredentials creds = prefGetCredentials();

  Serial.println(creds.ssid);

  Serial.println("networkName:" + creds.ssid);
  Serial.println("NetworkPwd:" + creds.password);

       
  bool noNetworkSaved = (creds.ssid.length() == 0);
  if (noNetworkSaved) 
  {
    Serial.println("No nets saved");

    lcdClearStr(0); 
    lcdDrawDbg("No nets saved", 0);

    lcdClearStr(1); 
    lcdDrawDbg("Hotspot mode", 1);

    lcdClearStr(2);
    lcdDrawDbg(String(AP_SSID), 2);

    lcdClearStr(3);
    lcdDrawDbg(String(AP_PASSWORD), 3);

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

      
      
*/
      _mac = WiFI_getMac();
      
      lcdClearStr(5); 
      lcdDrawDbg("Mac: " + _mac, 4);
     
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
      api_updateStateAndGetTasks(_mac, state);

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
          prefSetAllCube(strDataCube, _curDay);
      }

      Serial.println("string dataCube: " + strDataCube);
      Serial.println("convert string dataCube in jsonCube");
        
      deserializeJson(jsonCube, strDataCube);
    }
    else // not connected
    {
      Serial.println("No hotspot found");

      lcdClearStr(3); 
      lcdDrawDbg("No hotspot found", 3);
      delay(2000);
    }
  }

   
}


void loop() 
{
  unsigned long mill = millis();

  // *** periodic update for state and tasks
  if ( (millis() - prevMillis_periodic_update) > Milli_Periodic_Update) 
  {
    prevMillis_periodic_update = mill;

    Serial.println("Periodic_Update:" + String(CubeStateStrings[prevState]));

    api_updateStateAndGetTasks(_mac, cubeState);
  
  }

  // *** periodic update for battery
  if ( (millis() - prevMillis_periodic_battery) > Milli_Periodic_Battery) 
  {
    prevMillis_periodic_battery = mill;

    _batteryVolt = readBatteryVoltage();
    showBatteryVoltage(_batteryVolt);

    Serial.print("Periodic_Battery " + String(_batteryVolt) + " V");
  

    int level = map(_batteryVolt, 0, 4.5, 0, 6);
    drawIconBattery(112, 20, level);
  }

  // *** enter in charging mode
  if ( (millis() - prevMillis_periodic_chr) > Milli_Periodic_CheckCharging) 
  {
    prevMillis_periodic_chr = mill;

    if (isCharging()) {
    
      counter_recharge++;
      
      Serial.println("Charging mode");

      int level = map(_batteryVolt * 100, 300, 480, 1, 12);
      Serial.println("Bat Level:" + String(level));
      Serial.println("Bat Voltage:" + String(_batteryVolt, 2));
      
      setLevel(level);

      chargingActive = true;

      if (counter_recharge == 1) {

        buzzerTone(500, 1000);
      }
    }
    else {
      chargingActive = false;
    }
  }

  // *** periodic check
  if ( (millis() - prevMillis_periodic_up) > Milli_Periodic_Check)
  {
    prevMillis_periodic_up = mill;
 
    counter++;

    readSensors(false); // false is no debug
 
    SensorState s = getSensors(digiL, digiR, digiF, digiB, digi0, digiT);
      
    lcdDot(s, 56);

    cubeState = getCubeState(s);
    
    if (cubeState != CubeState::NO_SIDE && cubeState != CubeState::MULTIPLE_SIDE) {
      cubeFace = cubeState;
    }
    
    

    // *** air side
    if (cubeState == CubeState::NO_SIDE) {

      counter_up++;
      //Serial.println(String(counter_up));
      if (counter_up > Milli_Periodic_Check)
      {
        Serial.println("Statistics");

        //setLevel(6, ColorFace[cubeState]);

        api_GetActivities(_mac);

        
        lcdDrawFullStats(indexdStatPart % 6);

        indexdStatPart++;

        delayUp = false;
      }
    }

    // *** multiple side
    if (cubeState == CubeState::MULTIPLE_SIDE) {
      
      int numPressed = getNumberOfPressed(s);
      if (numPressed == 3) {

        if (!showInfoNet) {
        
          showInfoNet = true;
          prevMillis_periodic_step = millis();

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
      else if(numPressed == 4) {

        lcdClearStr(3); 
        lcdClearStr(4);
        if (!configNet) {

          configNet = true;
          prevMillis_periodic_step = millis();

          Serial.println("Reset net");

          lcdDrawDbg("Press Reset to add a net", 3);

        }
      }
    }

    // *** timer end for multiple pressed

    if (showInfoNet && (millis() - prevMillis_periodic_step) > 3000) 
    {
      
      Serial.println("Timer end for info net");

      String curTask = _taskArray[cubeState];
      lcdDrawTask(curTask);
      
      showInfoNet = false;

    }

    if (configNet && (millis() - prevMillis_periodic_step) > 3000) 
    {
      
      Serial.println("Timer end for config net");

      String curTask = _taskArray[cubeState];
      lcdDrawTask(curTask);
      
      configNet = false;

    }

    // *** return
    if (prevState == CubeState::NO_SIDE && cubeState != CubeState::NO_SIDE && counter_up > Milli_Periodic_Check) {
      Serial.println("Return");

      indexdStatPart = 0;
      counter_up = 0;
      counter_recharge = 0;

      showInfoNet = false;

      lcdClear();
      lcdDrawDbg("CUBE 365", 0, 40);
      drawIcon(10, 20, ICON_WIFI);
      drawIconBattery(112, 20, 0);
    }

    // *** press top face
    if (topFacePressed(prevState)) {

      Serial.println("Top face " + CubeStateStrings[prevState]);

      //Serial.println("prevState:" + String(prevState) + "  cubeState:" + String(cubeState));

      api_updateStateAndGetActivities(_mac, prevState);

      
      showActivitiesArray();
       
      // light
      topFaceActive = 1- topFaceActive;      
      setWhiteLed(topFaceActive);

      buzzerTone(2000, 100);

      api_GetActivities(_mac);

      String curActivity = _activityArray[prevState];
      Serial.println("curActivity:" + String(curActivity));

      lcdDrawActivityTime(curActivity);

      
      log(_mac, "INFO", "Current activity: " + curActivity, 0.0);
    }

    // *** change side
    if (prevFace != cubeFace && singleFaceActive(cubeFace)) {

      //Serial.println("Change to " + CubeStateStrings[cubeState] + "From " + CubeStateStrings[prevState]);
      
      log(_mac, "INFO", "Change from " + CubeStateStrings[prevFace] + " to " + CubeStateStrings[cubeFace], 0.0);

      api_updateStateAndGetActivities(_mac, cubeState);

      //buzzerTone(1000, 100);

      counter_up = 0;

      // get perc
      int level = getLevelByActivityPerc(cubeState);
      
      setLevel(level, ColorFace[cubeState]);
      
      String curTask = _taskArray[cubeState];
      lcdDrawTask(curTask);
      
      Serial.println("curTask:" + String(curTask));

      if (curTask == "-") {

        lcdClearStr(3);
        lcdDrawDbg("No Task", 3);

        buzzerTone(500, 1000);
        buzzerTone(0, 500);
        buzzerTone(500, 1000);
        buzzerTone(0, 500);
        buzzerTone(500, 1000);
      }

      


    }

    

    prevState = cubeState;
    prevFace = cubeFace;

  }

  
}
