
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

#include "pomodorotimer.h"

//#include <Adafruit_NeoPixel.h>



TaskHandle_t loadingTask = nullptr;
volatile bool loading = true;

void loadingAnimation(void *parameter)
{
    int dots = 0;

    while (loading)
    {
        String s = "";

        for (int i = 0; i < dots; i++) {
            s += "-";
        }
        lcdClearStr(3);
        lcdDrawDbg(s, 3, 5); 
        // Aggiorna il display
        
        dots = (dots + 1) % 20; // Incrementa il numero di punti e resetta a 0 dopo 3
        //delay(400); // Attendi 400 millisecondi

        vTaskDelay(pdMS_TO_TICKS(400));
    }

    vTaskDelete(NULL);
}


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
unsigned long prevMillis_periodic_tomato = 0;

unsigned long prevMillis_waiting_longpress_top_face = 0;

unsigned long prevMillis_waiting_presseds = 0;

unsigned long prevMillis_periodic_up = 0;


int Milli_Periodic_Update = 10000;    // 10 sec
int Milli_Periodic_Check = 50;        // 50 msec
int Milli_Periodic_CheckCharging = 1000;  // 1 sec
int Milli_Periodic_Battery = 12000;       // 12 sec

int Milli_Waiting_Pressed = 300;  // 300 msec

int Milli_Periodic_Tomato = 5000;


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
bool waitingPressed = false;

bool topFaceActive = false;
bool chargingActive = false;

bool longPressTopFace = false;
 

PomodoroTimer pomodoro;
  
void setup() 
{

   //attachInterrupt(digitalPinToInterrupt(BOOT_BUTTON_PIN), handleButtonPress, FALLING);

  Serial.begin(115200);
  while (!Serial) {
    
    delay(10);
  }
  Serial.println("Serial initialized");


  initLeds();

   
  initWhiteLed();
  setWhiteLed(false);

  initSensors();
  delay(200);
  readSensors(true);
  SensorState s = getSensors(digiL, digiR, digiF, digiB, digi0, digiT);
  prevState = getCubeState(s);

  prevFace = prevState;

 
  Serial.println("Initial state: " + CubeStateStrings[prevState]);
   
  lcdInit();
  delay(100);

  Serial.println("Initial prevState: " + String(prevState));
  adjustLcdOrientation(prevState);

  
  lcdClearStr(0); 
  lcdDrawDbg("CUBE 365", 0, 40);
 

  initBattery();
 
  setLevel(12, ColorFace[prevState]);
 

  initBuzzer();
  buzzerTone(1000, 1500);

  //testLeds();
 
  //testBattery();

  //testSensors();

  //testSensorsWithLeds();

  //testSensorsAnalog();

  //testMagSensor();



  
  bool changeCredential = isCredentialModeEnabled();
  Serial.println("ChangeCredential Mode:" + String(changeCredential));

  NetworkCredentials creds = prefGetCredentials();

  Serial.println(creds.ssid);

  Serial.println("networkName:" + creds.ssid);
  Serial.println("NetworkPwd:" + creds.password);

  Serial.println("networkName2:" + creds.ssid2);
  Serial.println("NetworkPwd2:" + creds.password2);

       
  bool noNetworkSaved = (creds.ssid.length() == 0);
  if (noNetworkSaved || changeCredential) 
  {
    Serial.println("Hotspot mode");

    lcdClearStr(0); 
    lcdClearStr(1); 

    if (noNetworkSaved) {      
      lcdDrawDbg("No nets saved", 0);
      lcdDrawDbg("Hotspot mode", 1);
    }
    else if(changeCredential) {
      lcdDrawDbg("Change net", 0);
      lcdDrawDbg("Hotspot mode", 1);
    }

    
    hotSpotMode();
  }
  else 
  {
   
    xTaskCreate(loadingAnimation, "Loading", 2048, NULL, 1, &loadingTask);

    bool connected = WiFi_connectToAccessPoint(creds.ssid, creds.password, creds.ssid2, creds.password2);

    loading = false;
    lcdClearStr(3);
    vTaskDelay(pdMS_TO_TICKS(100)); // todo remove

    if(connected)
    {
      _ApConnected = true;
 

      drawIcon(10, 20, ICON_WIFI);

      


      _batteryVolt = readBatteryVoltage();
      showBatteryVoltage(_batteryVolt);


      int level = map(_batteryVolt, 0, 4.5, 0, 6);
      drawIconBattery(112, 20, level);
      
 
      _mac = WiFI_getMac();
      
      lcdClearStr(5); 
      lcdDrawDbg("Mac: " + _mac, 4);

      Serial.println("DeviceId:" + _mac);
     
      // debug
      String url = prefGetUrl();
      Serial.println("Host Url from pref:" + url);

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

     
      api_updateStateAndGetTasks(_mac, prevState);

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

      cubeState = getCubeState(s);
      String curTask = _taskArray[cubeState];
      lcdDrawTask(curTask);
      
    }
    else // not connected
    {
      Serial.println("No hotspot found");

      lcdClearStr(3); 
      lcdDrawDbg("No hotspot found", 3);
      delay(2000);

      hotSpotMode();
      while(1);
    }
  }

  Serial.println("end setup");
}


void loop() 
{
  unsigned long mill = millis();

  pomodoro.update();

  // *** periodic pomodoro timer update
  if ( (millis() - prevMillis_periodic_tomato) > Milli_Periodic_Tomato) 
  {
    prevMillis_periodic_tomato = mill;

    PomodoroTimer::State s = pomodoro.state();

    if (s != PomodoroTimer::Idle) {
       
      int dots = pomodoro.progressDots(12);
      lcdTomatoDots(dots, 56);

      lcdClearStr(4);
      //lcdDrawDbg("Pomodoro state: "/* + pomodoro.remainingSeconds()*/, 4);

      String remainingTime = String(pomodoro.remainingSeconds()/60) + ":" + String(pomodoro.remainingSeconds()%60);
      lcdDrawDbg(remainingTime, 4);

      Serial.println("Pomodoro state: " + String(s) + "  Dots:" + String(dots));
    }
  }

   

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

    Serial.print("Periodic_Battery: ");

    _batteryVolt = readBatteryVoltage();
    showBatteryVoltage(_batteryVolt);

    Serial.println(String(_batteryVolt) + " V");
  

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
  //if (false)
  {
    prevMillis_periodic_up = mill;
 
    counter++;

    readSensors(false); // false is no debug
 
    SensorState s = getSensors(digiL, digiR, digiF, digiB, digi0, digiT);
      
    if (!pomodoro.isRunning()) {
      lcdDot(s, 56);
    }
     

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

        api_GetActivities(_mac);//todo remove

        
        lcdDrawFullStats(indexdStatPart % 6);

        indexdStatPart++;

        delayUp = false;
      }
    }

    // *** multiple side
    if (cubeState == CubeState::MULTIPLE_SIDE) {
      
      
      int numPressed = getNumberOfPressed(s);
      if (numPressed > 2) {

        if (!waitingPressed) { 
        
          waitingPressed = true;
          prevMillis_waiting_presseds = millis();

        }
      }
       
    }

    // *** check multiple pressed

    if (waitingPressed && (millis() - prevMillis_waiting_presseds) > Milli_Waiting_Pressed) {
      waitingPressed = false;

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

            Serial.println("WiFi_startWebServer");
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

       
        if (!configNet) {

          configNet = true;
          prevMillis_periodic_step = millis();

          changeCredentialMode(true);

          Serial.println("Reset net");
          lcdClearStr(3); 
          lcdClearStr(4);
          lcdDrawDbg("Press Reset to add a net", 3);

        }
      }
    }

    // *** timer end for multiple pressed
    if (showInfoNet && (millis() - prevMillis_periodic_step) > 3000) 
    {
      showInfoNet = false;

      Serial.println("Timer end for info net");

      String curTask = _taskArray[cubeState];
      lcdDrawTask(curTask);
    }

    if (configNet && (millis() - prevMillis_periodic_step) > 6000) 
    {
      configNet = false;

      Serial.println("Timer end for config net");

      String curTask = _taskArray[cubeState];
      lcdDrawTask(curTask);
      
      changeCredentialMode(false);

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

      if (!longPressTopFace) { 
        
          longPressTopFace = true;
          prevMillis_waiting_longpress_top_face = millis();

          Serial.println("Waiting for long press top face");
      }

      //Serial.println("prevState:" + String(prevState) + "  cubeState:" + String(cubeState));

      api_updateStateAndGetActivities(_mac, prevState);


      showActivitiesArray();
       
      // light
      topFaceActive = 1- topFaceActive;      
      setWhiteLed(topFaceActive);

      buzzerTone(2000, 100);

      api_GetActivities(_mac); // todo remove

      String curActivity = _activityArray[prevState];
      Serial.println("curActivity:" + String(curActivity));

      lcdDrawActivityTime(curActivity);
 
      


      log(_mac, "INFO", "Current activity: " + curActivity, 0.0);
    }
    

    if (longPressTopFace && (millis() - prevMillis_waiting_longpress_top_face) > 3000) 
    {
      longPressTopFace = false;
 
      Serial.println("prevFace:" + String(prevFace) + "  cubeFace:" + String(cubeFace));
      
      readSensors(true);
      bool topPressed = topFacePressed(prevFace);
      Serial.println("Top face pressed:" + String(topPressed));
      if (topPressed) {

        Serial.println("Timer end for long press top face");
        buzzerTone(2000, 400);

        if (pomodoro.isRunning()) {
          Serial.println("Stop pomodoro timer");
          pomodoro.stop();
        }
        else {
          Serial.println("Start pomodoro timer");
          pomodoro.begin(25, 5, 15, 4);
          pomodoro.start();

          drawIconTomato(10, 20);
 
        }
        lcdClearStr(5);
         
      }

      
 
    }

    // *** change side
    if (prevFace != cubeFace && singleFaceActive(cubeFace)) {
      Serial.println("Change side");
      
      int level = getLevelByActivityPerc(cubeState);
      
      if (cubeState == CubeState::TOP || cubeState == CubeState::BASE) {
        level = 12;
      }
      setLevel(level, ColorFace[cubeState]);

      adjustLcdOrientation(cubeFace);
      lcdClearStr(0); 
      lcdDrawDbg("CUBE 365", 0, 40);

      api_updateStateAndGetActivities(_mac, cubeState);

      //buzzerTone(1000, 100);

      counter_up = 0;
 

     
        
      
      String curTask = _taskArray[cubeState];
      lcdDrawTask(curTask);
      
      Serial.println("curTask:" + String(curTask));

      log(_mac, "INFO", "Change from " + CubeStateStrings[prevFace] + " to " + CubeStateStrings[cubeFace], 0.0);


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
