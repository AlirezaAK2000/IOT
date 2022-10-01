#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "";
const char *password = "";
String hn = "IOT";

void setup() {
 Serial.begin(9600);
 Serial.println("");
 WiFi.mode(WIFI_STA);
 // print default hostname
 Serial.print("default hostname:" );
 Serial.println(WiFi.hostname().c_str());
 // set new hostname
 WiFi.hostname(hn.c_str());
// print new hostname
 Serial.print("new hostname:" );
 Serial.println(WiFi.hostname().c_str());


  //Init Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
}


void loop() {
}
