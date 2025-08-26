#include <WiFi.h>
#include<HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";

String apiKey = "APIKEY";
String latitude = "CITY_LATITUDE";
String longitude = "CITY_LONGITUDE";

String serverPath = "http://api.openweathermap.org/data/2.5/weather?lat=" + latitude + "&lon=" + longitude + "&appid=" + apiKey + "&units=metric";


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
 if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverPath.c_str()); // start connection to OpenWeatherMap URL
    int httpResponseCode = http.GET(); //Sends a get request. 200 = OK, negtive = error

    if (httpResponseCode > 0) {
      String payload = http.getString();
    
      StaticJsonDocument<1024> doc;

      // Parse JSON
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float temp = doc["main"]["temp"];
        int humidity = doc["main"]["humidity"];
        const char* description = doc["weather"][0]["description"];
        const char* city = doc["name"];

        Serial.println("Weather Data:");
        Serial.print("City: ");
        Serial.println(city);
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.println(" °C");
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
        Serial.print("Condition: ");
        Serial.println(description);
      } else {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end(); //Releases HUZZAH32's RAM reserved for http call
  }
  delay(60000); // wait 1 min before next request

}
