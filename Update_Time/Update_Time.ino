#include <WiFi.h>
#include "time.h"

const char* ssid = "SSID";
const char* password = "PASSWORD";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 14400; //Offset time between your time zone and GMT (in seconds)
const int  daylightOffset_sec = 3600; //Daylight savings offset time

void LastUpdateTime();

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("Waiting for WiFi to connect...");
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED || millis() > 20000){
      delay(500);
      Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else{
    Serial.println("\nCouldn't connect to WiFi");
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop() {
  delay(1000);
  LastUpdateTime();

}

void LastUpdateTime(){
  struct tm timeInfo;
  if(!getLocalTime(&timeInfo)){
    Serial.println("Couldn't obtain time");
    return;
  }

  Serial.println(&timeInfo, "%H:%M:%S");
}