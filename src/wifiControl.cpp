#include "wifiControl.h"

//#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>

#include <AsyncTCP.h>

//#define TEMPLATE_PLACEHOLDER '~'
#include <ESPAsyncWebServer.h>
 

#include "prefs.h"

#include "webpage_net.h"
#include "webpage_faces.h"

#include "webapi.h"

#include "config.h"

String NetworkArray[8]; 
int numberOfNetworkFound;

String webAPIUrl;

int serverPort;
AsyncWebServer *server = nullptr;
DNSServer dnsServer;
IPAddress accessPointIp;
bool accessPointPortalActive = false;

bool serverStarted = false;


AsyncWebSocket ws("/ws");
 

 
const char *PARAM_1 = "NET";
const char *PARAM_2 = "Password";

const char *PARAM_3 = "NET2";
const char *PARAM_4 = "Password2";
 
//const String inputParam1 = "NET";
//const String inputParam2 = "Password";

String inputNetworkName;
String inputNetworkPwd;
String inputNetworkName2;
String inputNetworkPwd2;
//String inputNetworkName2;


// configurations
String inputUrl;
const char *PARAM_URL = "Url";

static void sendCaptivePortalRedirect(AsyncWebServerRequest *request)
{
    request->redirect("http://" + accessPointIp.toString() + "/");
}

static void resetWebServer(int port)
{
    if (server != nullptr)
    {
        server->end();
        delete server;

        server = nullptr;
        serverStarted = false;
    }

    serverPort = port;
    server = new AsyncWebServer(serverPort);
}
 

int WiFi_scanner()
{
   Serial.println("scan start");

    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect(false, true);
    WiFi.scanDelete();

   // WiFi.scanNetworks will return the number of networks found
   int n = WiFi.scanNetworks();
    numberOfNetworkFound = n;
   Serial.println("scan done");
   if (n == 0)
   {
      Serial.println("no networks found");
   }
   else
   {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i)
      {
         // Print SSID and RSSI for each network found
         Serial.print(i + 1);
         Serial.print(": ");
         Serial.print(WiFi.SSID(i));
         Serial.print(" (");
         Serial.print(WiFi.RSSI(i));
         Serial.print(")");
         Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
         delay(10);

         if (i < 8) {
            NetworkArray[i] = WiFi.SSID(i);
         }

      }
   }
   return n;
}


String processor(const String& var) 
{
    if (var == "WIFI_LIST") {

        String wifiList = "";
      
        int n = numberOfNetworkFound;
        //n=2;
        if (n > 8) {
            n=8;
        }

        for (int i = 0; i < n; ++i) {
            wifiList += "<option value='" + NetworkArray[i] + "'>" + NetworkArray[i] + "</option>";
        }

        return wifiList; 
    }
    else if (var == "CUR_NET") {

        NetworkCredentials creds = prefGetCredentials();
        return creds.ssid;
    }
    else if(var == "CUR_URL") {

        ServerUrl = prefGetUrl();
        return ServerUrl;
    }
    return String(); 
}

void notFound(AsyncWebServerRequest *request)
{
    if (accessPointPortalActive) {
        sendCaptivePortalRedirect(request);
        return;
    }

    request->send(404, "text/plain", "Not found");
}

void WiFi_accessPointSetup(String ssid, String pwd)
{
    resetWebServer(80);
    dnsServer.stop();
    accessPointPortalActive = false;

    WiFi.softAPdisconnect(true);
    WiFi.disconnect(true, true);
    delay(200);

    WiFi.persistent(false);
    WiFi.mode(WIFI_AP_STA);
    WiFi.setSleep(false);
   
    IPAddress local_IP(192, 168, 4, 1);

    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);

    //WiFi.enableIpV6(false);
    //WiFi.softAPsetHostname("MyCube");

    bool configOk = WiFi.softAPConfig(local_IP, gateway, subnet);
    Serial.println("WF: softAPConfig " + String(configOk ? "OK" : "FAIL"));
 


    bool apOk = WiFi.softAP(ssid.c_str(), pwd.c_str(), 1, 0, 4);
    Serial.println("WF: softAP " + String(apOk ? "OK" : "FAIL"));

    if (!apOk) {
        Serial.println("WF: AP start failed, check SSID/password");
        return;
    }

    IPAddress IP = WiFi.softAPIP();
    accessPointIp = IP;
    Serial.print("WF: AP IP address ");
    Serial.println(IP.toString());
    Serial.println("WF: AP SSID " + ssid);
    Serial.println("WF: AP PWD len " + String(pwd.length()));

    numberOfNetworkFound = WiFi_scanner();
    Serial.println("WF: scanned networks " + String(numberOfNetworkFound));

    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(53, "*", accessPointIp);
    accessPointPortalActive = true;
 
 
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("WF: start");
        request->send_P(200, "text/html", index_selectNetwork, processor); 
    });

    server->on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("generate_204 requested by client");
        sendCaptivePortalRedirect(request);
    });

    server->on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("hotspot-detect.html requested by client");
        sendCaptivePortalRedirect(request);
    });

    server->on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("connecttest.txt requested by client");
        sendCaptivePortalRedirect(request);
    });

    server->on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("ncsi requested by client");
        sendCaptivePortalRedirect(request);
    });

    server->on("/redirect", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("redirect requested by client");
        sendCaptivePortalRedirect(request);
    });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server->on("/getWifi", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("WF: init submit");

        // network 1
        if (request->getParam(PARAM_1)->value() != "") 
        {
            inputNetworkName = request->getParam(PARAM_1)->value();
        }
        else 
        {
            inputNetworkName = "none";
        }
 
        
        if (request->getParam(PARAM_2)->value() != "") 
        {
            inputNetworkPwd = request->getParam(PARAM_2)->value();
        }
        else 
        {
            inputNetworkPwd = "none";
        }

        // network 2
        if (request->getParam(PARAM_3)->value() != "") 
        {
            inputNetworkName2 = request->getParam(PARAM_3)->value();
        }
        else 
        {
            inputNetworkName2 = "none";
        }
 
        if (request->getParam(PARAM_4)->value() != "") 
        {
            inputNetworkPwd2 = request->getParam(PARAM_4)->value();
        }
        else 
        {
            inputNetworkPwd2 = "none";
        }

        String newNetwork = inputNetworkName + "," + inputNetworkPwd + "\n";
        String newNetwork2 = inputNetworkName2 + "," + inputNetworkPwd2 + "\n";

        Serial.println("WF1: " + newNetwork);
        Serial.println("WF2: " + newNetwork2);
        
        Serial.println("WF: submit");

        prefSetCredentials(inputNetworkName, inputNetworkPwd);

        prefSetCredentials(inputNetworkName2, inputNetworkPwd2, 2);

        changeCredentialMode(false);
        dnsServer.stop();
        accessPointPortalActive = false;
      
        server->end();

        // reset
        ESP.restart();

    });
 

    server->onNotFound(notFound);

    server->begin();

    Serial.println("WF: server begin on port " + String(serverPort));
}

void WiFi_processAccessPoint()
{
    if (accessPointPortalActive) {
        dnsServer.processNextRequest();
    }
}

bool WiFi_connectToAccessPoint(String ssid, String pwd, String ssid2, String pwd2)
{
     WiFi.mode(WIFI_STA);

    for (int net = 0; net < 2; net++)
    {
        String currentSSID = (net == 0) ? ssid : ssid2;
        String currentPWD  = (net == 0) ? pwd  : pwd2;

        if (currentSSID.length() == 0) {
            continue; // Skip if SSID is empty
        }
        
        Serial.println("Trying to connect to: " + currentSSID);

        WiFi.begin(currentSSID, currentPWD);

        int attempts = 10;

        while (WiFi.status() != WL_CONNECTED && attempts > 0)
        {
            delay(800);
            Serial.print(".");
            attempts--;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\nConnected to " + currentSSID);
            return true;
        }

        Serial.println("\nFailed.");
        WiFi.disconnect();
    }


    return false;
}

String WiFi_startWebServer()
{
    if (serverStarted) {
        Serial.println("WF: server already started");
        return WiFi.localIP().toString() + ":" + String(serverPort);
    }

    resetWebServer(SERVER_PORT);
    dnsServer.stop();
    accessPointPortalActive = false;


    server->addHandler(&ws);

    String localIp = "";
    
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("WF: server on");
        
        request->send_P(200, "text/html", index_page, processor);      
    });

    server->on("/setUrl", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("WF: set url");
        
        if (request->hasParam(PARAM_URL))
        {
            inputUrl = request->getParam(PARAM_URL)->value();
            Serial.println("URL: " + inputUrl);
            
            prefSetUrl(inputUrl);
        }
        else 
        {
            Serial.println("WF: url parameter not found!");
        }

        request->send_P(200, "text/html", index_page, processor);      
    });
    
    server->begin();

    serverStarted = true;

    String ipAddress = WiFi.localIP().toString() + ":" + String(SERVER_PORT);

    Serial.println(ipAddress);

    return ipAddress;
    
}

String WiFI_getMac()
{
    uint8_t mac[6];
    WiFi.macAddress(mac);

    String macStr = "";
    for (int i = 0; i < 6; i++) 
    {
        if (mac[i] < 0x10) {
            macStr += "0";
        }
        macStr += String(mac[i], HEX);
    }

    macStr.toUpperCase();
    return macStr;
}
 