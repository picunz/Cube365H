
#include "webapi.h"


String ServerUrl; 


bool cmd_reset = false;
 
String registerDevice(String deviceId)  // todo" fare funzione interna
{
    HTTPClient http;

    String queryParams = "?Action=RegisterDevice&DeviceId=" + deviceId;

    String response = "-";

    String endpoint = ServerUrl +  "/api/statistic.php";

    Serial.println("url:" + endpoint + queryParams);
   
    http.begin(endpoint + queryParams);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.GET();

    // Controlla la risposta
    if (httpResponseCode > 0) {
        response = http.getString();          
    } else {
        Serial.println("Errore durante la richiesta: " + String(httpResponseCode));
    }

    // Chiudi la connessione
    http.end();
    return response;
}
  
String getTasks(String deviceId)
{
    HTTPClient http;

    String response = "";
   
    String endpoint = ServerUrl +  "/api/statistic.php?timestamp=" + String(millis());

    http.begin(endpoint);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Crea i dati della richiesta
    String postData = "Action=GetTasks&DeviceId=" + deviceId;

    http.addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    http.addHeader("Pragma", "no-cache");
    http.addHeader("Expires", "0");

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
        response = http.getString();
    } else {
        Serial.println("Errore durante la richiesta: " + String(httpResponseCode));
    }
 
    http.end();
    response.trim();
    return response;
}

String getActivities(String deviceId)
{
    HTTPClient http;

    String response = "";
   
    String endpoint = ServerUrl +  "/api/statistic.php?timestamp=" + String(millis());
    Serial.println("ServerUrl:" + ServerUrl);

    http.begin(endpoint);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Crea i dati della richiesta
    String postData = "Action=GetActivities&DeviceId=" + deviceId;

    http.addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    http.addHeader("Pragma", "no-cache");
    http.addHeader("Expires", "0");

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
        response = http.getString();
    } else {
        Serial.println("Errore durante la richiesta: " + String(httpResponseCode));
    }
 
    http.end();
    response.trim();
    return response;
}

String sendState(String deviceId, String state)
{
    HTTPClient http;

    String response = "";
   
    String endpoint = ServerUrl +  "/api/statistic.php?timestamp=" + String(millis());

    //Serial.println("endpoint:" + endpoint);

    http.begin(endpoint);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");


    // Crea i dati della richiesta
    String postData = "Action=UpdateState&DeviceId=" + deviceId + "&State=" + state;

    Serial.println("postData:" + postData);

    http.addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    http.addHeader("Pragma", "no-cache");
    http.addHeader("Expires", "0");


    // Invia la richiesta POST
    int httpResponseCode = http.POST(postData);

    // Controlla la risposta
    if (httpResponseCode > 0) {
        response = http.getString();
        //Serial.println("Risposta del server: " + response);

        // Puoi gestire la risposta qui
        //onServerResponse(response);
    } else {
        Serial.println("Errore durante la richiesta: " + String(httpResponseCode));
    }
 
    http.end();
    response.trim();
    return response;
}



/*
void onServerResponse(const String& response) 
{
    //Serial.println("Server response: " + response);
}
*/

String sendLog(String deviceId, String level, String message, float value)
{
    HTTPClient http;

    String response = "";
   
    String endpoint = ServerUrl +  "/api/statistic.php?timestamp=" + String(millis());
 

    http.begin(endpoint);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");


    // Crea i dati della richiesta
    String postData = "Action=SendLog&DeviceId=" + deviceId + "&Level=" + level + "&Message=" + message + "&Value=" + String(value);

     
    http.addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    http.addHeader("Pragma", "no-cache");
    http.addHeader("Expires", "0");


    // Invia la richiesta POST
    int httpResponseCode = http.POST(postData);

    // Controlla la risposta
    if (httpResponseCode > 0) {
        response = http.getString();

    } else {
        Serial.println("Errore durante la richiesta: " + String(httpResponseCode));
    }
 
    http.end();
    response.trim();
    return response;
}