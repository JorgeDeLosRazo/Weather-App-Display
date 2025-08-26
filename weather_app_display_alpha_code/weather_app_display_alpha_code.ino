#include <WiFi.h>
#include "Adafruit_ThinkInk.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define EPD_DC 33
#define EPD_CS 15
#define EPD_BUSY 32 // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS 21
#define EPD_RESET 27  // can set to -1 and share with microcontroller Reset!
#define EPD_SPI &SPI // primary SPI

const char* ssid = "SSID";
const char* password = "PASSWORD";

String apiKey = "APIKEY";
String latitude = "CITY_LATITUDE";
String longitude = "CITY_LONGITUDE";

String serverPath = "https://api.openweathermap.org/data/2.5/weather?lat=" + latitude + "&lon=" + longitude + "&appid=" + apiKey + "&units=metric";

ThinkInk_370_Mono_BAAMFGN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

void DisplayValuesOnEInk(float temp, int humidity, String description, String city); //Custom functions that displays apropriate values on display

void setup() {
  Serial.begin(115200);
  //------Waits for connection to serial is made-----------------------------
  while (!Serial) { 
    delay(10);
  }
  Serial.println("Hello dumbass");
  display.begin(THINKINK_MONO); //Initializes the display as a monohrome ThinkInk display

  //------Connects to WiFi-----------------------------
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

//----------Obtains Weather Info from openweathermap-----------------------------
 if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverPath.c_str()); // start connection to OpenWeatherMap URL
    int httpResponseCode = http.GET(); //Sends a get request. 200 = OK, negtive = error

    if (httpResponseCode > 0) {
        String payload = http.getString();
      
        StaticJsonDocument<1024> doc;

        //-----Parses JSON file-----------------------------
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
          float temp = doc["main"]["temp"];
          int humidity = doc["main"]["humidity"];
          String description = doc["weather"][0]["description"];
          String city = doc["name"];
          DisplayValuesOnEInk(temp, humidity, description, city);

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
  delay(60000); //The display refreshes every 1min
}



void DisplayValuesOnEInk(float temp, int humidity, String description, String city){

  String  fullCityName = "";
  display.clearBuffer();
  display.setTextColor(EPD_BLACK);

  display.setTextSize(3);
  display.setCursor(20, 20);
  fullCityName = city + ", CITYS_STATE";
  display.println(fullCityName); //Displays City 

  display.setTextSize(4);
  display.setCursor(20, 80);
  display.print(temp); //Displays temperature
  display.drawCircle(display.getCursorX() , display.getCursorY() - 3, 3, EPD_BLACK); // This line is to display degree symbol
  display.println(" C");

  display.setTextSize(2);
  display.setCursor(20, 140);
  display.print("Humidity: "); //Displays humidity
  display.print(humidity);
  display.println("%");

  display.setCursor(20, 170);
  display.println(description);

  display.setTextSize(1);
  display.setCursor(20, 210);
  display.println("Updated: 10:15 AM"); //Displays when the above values were last updated

  display.setTextSize(1);
  display.setCursor(display.getCursorX() + 300, 210);
  display.println("WiFi: CONNECTED");

  display.display();
}

