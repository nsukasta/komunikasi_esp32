#include <SPI.h>
#include <Wire.h>

#include <WiFi.h>
#include <HTTPClient.h>


#define DHTPIN 5 

#include "DHT.h"
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

#define SERVER_IP "192.168.4.1"
#ifndef STASSID
#define STASSID "ilkomIOT"
#define STAPSK  "semangat"
#endif


#define FIRE_DIGITAL 15 

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    
     WiFi.begin(STASSID, STAPSK);
    Serial.println("");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(300);
   }
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());

}

void sendAlert() {
   if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    String serverPath="http://"+String(SERVER_IP)+"/FIRE/H";
    Serial.print("\nserverPath="+serverPath+"\n");
    http.begin(client, serverPath);
    http.addHeader("Content-Type", "application/text");
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.printf("[HTTP] ... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
   }

}

void loop() {

  
  int fire = digitalRead(FIRE_DIGITAL);

  Serial.println(fire);
  if (fire==1) {
    Serial.print("KEBAKARAN ");
    sendAlert();
  } else {
    Serial.print("aman ");
  }
    delay(2000);

}
