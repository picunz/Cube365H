#include "prefs.h"


Preferences preferences;


// credentials 

void prefResetCredentials()
{
   preferences.begin("cube-app", false); 
   preferences.putString("NetworkName", "");
   preferences.putString("NetworkPwd", "");
   preferences.end(); 
}

void prefSetCredentials(String name, String pwd)
{
   preferences.begin("cube-app", false); 
   preferences.putString("NetworkName", name);
   preferences.putString("NetworkPwd", pwd);
   preferences.end(); 
}

NetworkCredentials prefGetCredentials()
{
    NetworkCredentials creds;

    preferences.begin("cube-app", false); 
    creds.ssid = preferences.getString("NetworkName");
    creds.password = preferences.getString("NetworkPwd");
    preferences.end();

    return creds;
}

// configurations

void prefSetUrl(String url)
{
   preferences.begin("cube-config", false); 
   preferences.putString("Url", url);
   preferences.end(); 
}

String prefGetUrl()
{
    preferences.begin("cube-config", false); 
    String url = preferences.getString("Url");
    preferences.end();

    return url;
}

// data cube

String prefGetDataCube()
{
   preferences.begin("data-cube", false); 
   String dataCube = preferences.getString("dataCube");
   preferences.end(); 

   return dataCube;
}

String prefGetDayCube()
{
   preferences.begin("data-cube", false); 
   String day = preferences.getString("atDay");
   preferences.end(); 

   return day;
}
 

void prefSetDataCube(String stringDataCube, String day)
{
   preferences.begin("data-cube", false); 
   preferences.putString("dataCube", stringDataCube);
   preferences.putString("atDay", day);
   preferences.end(); 
}
