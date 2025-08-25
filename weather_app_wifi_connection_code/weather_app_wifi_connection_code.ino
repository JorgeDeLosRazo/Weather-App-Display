#include <WiFi.h>
#include<HTTPClient.h>

const char* ssid = ":)";
const char* password = ";)";

String apiKey = "YOUR_APIKEY";
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
      String payload = http.getString(); // stores JSON response
      Serial.println("Response:");
      Serial.println(payload);  // prints raw JSON
    } else {
      Serial.print("Error code: "); //If request failed print error message
      Serial.println(httpResponseCode);
    }
    http.end(); //Releases HUZZAH32's RAM reserved for http call
  }
  delay(60000); // wait 1 min before next request

}
