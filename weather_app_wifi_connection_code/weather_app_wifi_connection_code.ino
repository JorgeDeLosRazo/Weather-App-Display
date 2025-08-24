#include <WiFi.h>

const char* ssid = "wifi name";
const char* password = "wifi password";

void setup() {
  Serial.begin(115200);
  delay(1000); //One second wait for the USB to fully connect

  Serial.println("Waiting for WiFi connection...");
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:

}
