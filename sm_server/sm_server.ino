#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2
#define RELAY 13
const char *ssid = "ilkomIOT";
const char *password = "semangat";
WiFiServer server(80);
void setup() {
pinMode(LED_BUILTIN, OUTPUT);
pinMode(RELAY, OUTPUT);
digitalWrite(RELAY, HIGH);

Serial.begin(115200);
Serial.println();
Serial.println("Configuring access point…");
WiFi.softAP(ssid, password);
IPAddress myIP = WiFi.softAPIP();
Serial.print("AP IP address: ");
Serial.println(myIP);
server.begin();
Serial.println("Server started");
}

void loop() {
WiFiClient client = server.available();

if (client) {
Serial.println("New Client.");
String currentLine = "";
while (client.connected()) {
if (client.available()) {
char c = client.read();
Serial.write(c);
if (c == '\n') {
if (currentLine.length() == 0) {
client.println("HTTP/1.1 200 OK");
break;
} else {
currentLine = "";
}
} else if (c != '\r') {
currentLine += c;
}

if (currentLine.endsWith("GET /TEMP/H")) {
Serial.println("HOT!, ALERT!! ");
for (int i=0;i<20;i++){
digitalWrite(LED_BUILTIN, HIGH);
delay(100);
digitalWrite(LED_BUILTIN, LOW);
delay(100);
}

}
if (currentLine.endsWith("GET /FIRE/H")) {
for (int i=0;i<20;i++){
digitalWrite(LED_BUILTIN, HIGH);
delay(50);
digitalWrite(LED_BUILTIN, LOW);
delay(50);
}
Serial.println("FIRE!!!.. TURN OFF ELECTRICAL SWITCH!! ");
digitalWrite(RELAY, LOW);
delay(100);
}

}
}
client.stop();
Serial.println("Client Disconnected.");
}
}
