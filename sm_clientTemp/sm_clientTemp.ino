#include <SPI.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "192.168.4.1"
#define LED D6
#ifndef STASSID
#define STASSID "ilkomIOT"
#define STAPSK  "semangat"
#endif

#define GAS_ANALOG A0  
#define GAS_DIGITAL 35  
#define FIRE_DIGITAL D5 

#define DHTPIN D1 

#include "DHT.h"
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
     dht.begin();
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
    String serverPath="http://"+String(SERVER_IP)+"/TEMP/H";
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
  sensorApi();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.println(t);
  int ga = analogRead(GAS_ANALOG);
  
  if (t>33) {
    Serial.print("PANASSS!!! ");
    sendAlert();
  } else {
    Serial.print("suhu aman ");
  }
    delay(2000);

}

void sensorApi()
  {
    
    int firesensorDigital = digitalRead(FIRE_DIGITAL);
    Serial.println("Fire Class: ");
    Serial.print(firesensorDigital);
     Serial.print("Kebakarann!!");
    Serial.print("\t");
    
  
  if (firesensorDigital == 1) 
  {
    
    
    Serial.println("Fire");
     
    
  }
  else 
  
  {
    
  }
  }
