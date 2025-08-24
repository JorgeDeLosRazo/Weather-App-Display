#include <WiFi.h>
#include "Adafruit_ThinkInk.h"

const char* ssid = "WiFi name";
const char* password = "WiFi password";

#define EPD_DC 33
#define EPD_CS 15
#define EPD_BUSY 32 // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS 21
#define EPD_RESET 27  // can set to -1 and share with microcontroller Reset!
#define EPD_SPI &SPI // primary SPI

ThinkInk_370_Mono_BAAMFGN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

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
  display.clearBuffer();
  display.setTextSize(3);
  display.setCursor(20, 20);
  display.println("Insert City"); //Displays City 

  display.setTextSize(4);
  display.setCursor(20, 80);
  display.print("22"); //Displays temperature
  display.drawCircle(display.getCursorX() , display.getCursorY() - 3, 3, EPD_BLACK); // This line is to display degree symbol
  display.println(" C");

  display.setTextSize(2);
  display.setCursor(20, 140);
  display.println("Humidity: 60%"); //Displays humidity
  display.setCursor(20, 170);
  display.println("Clear");

  display.setTextSize(1);
  display.setCursor(20, 210);
  display.println("Updated: 10:15 AM"); //Displays when the above values were last updated

//---------Displays if connected to WiFi-------------------------
  display.setTextSize(1);
  display.setCursor(display.getCursorX() + 300, 210);
  if(WiFi.status() == WL_CONNECTED){
    display.println("WiFi: CONNECTED");
  } else {
    display.println("WiFi: NOT CONNECTED");
  }

  display.display();
  delay(10000);


}

