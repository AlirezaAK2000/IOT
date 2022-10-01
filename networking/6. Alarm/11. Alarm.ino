#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include  <ESPAsyncTCP.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "index_html.h"

#define D3 0
#define snooze_time 5000

struct Time {
  int h;
  int m;
  int s;
};

const char* ssid = "";
const char* password = "";

int sound;
int is_snoozed = 0;
int is_stoped = 0;
int snooze_checked = 0;
unsigned long current_time = 0;
unsigned long previous_time = 0;
bool is_ringing = false;

int buzzer_pin = D3;
Time inp_time = {0 , 0 ,0};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

AsyncWebServer server(80);

void setup() {
 Serial.begin(9600);
 pinMode(buzzer_pin, OUTPUT);


 WiFi.begin(ssid, password);
 Serial.print("Connecting to WiFi ..");
 while (WiFi.status() != WL_CONNECTED) {
   Serial.print('.');
   delay(1000);
 }
 Serial.println();

 Serial.print("Local IP:");
 Serial.println(WiFi.localIP());

 // Initialize a NTPClient to get time
 timeClient.begin();
 timeClient.setTimeOffset(16200);


 server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
   request -> send_P(200, "text/html", index_html);
 });

 server.on("/sound", HTTP_GET, [] (AsyncWebServerRequest *request) {
   if (request -> hasParam("value")){
     sound = request -> getParam("value") -> value().toInt();
     sound = map(sound, 0, 100, 0, 255);
   }
   Serial.print("sound: ");
   Serial.println(sound);
   request -> send(200, "text/plain", "OK");
 });

 server.on("/time", HTTP_GET, [] (AsyncWebServerRequest *request) {
   if (request -> hasParam("h") && request -> hasParam("m") && request -> hasParam("s")){
      inp_time.h = request -> getParam("h") -> value().toInt();
      inp_time.m = request -> getParam("m") -> value().toInt();
      inp_time.s = request -> getParam("s") -> value().toInt();
   }
   Serial.print("s = ");
   Serial.println(inp_time.s);
   request -> send(200, "text/plain", "OK");
 });

 server.on("/snooze", HTTP_GET, [] (AsyncWebServerRequest *request) {
   is_snoozed = 1;
   Serial.println("snooze button was pressed");
   request -> send(200, "text/plain", "OK");
 });

 server.on("/stop", HTTP_GET, [] (AsyncWebServerRequest *request) {
   is_stoped = 1;
   Serial.println("stop button was pressed");
   request -> send(200, "text/plain", "OK");
 });


 server.begin();
}


void loop() {
  timeClient.update();

  if(is_ringing)
    analogWrite(buzzer_pin, sound);
 
  if(inp_time.h == timeClient.getHours() && inp_time.m == timeClient.getMinutes() && timeClient.getSeconds()- 2 <= inp_time.s && inp_time.s <= timeClient.getSeconds()){
    Serial.println(sound);
    analogWrite(buzzer_pin, sound);
    is_ringing = true;
  }

  if (is_snoozed){
    analogWrite(buzzer_pin, 0);
    is_ringing = false;
    current_time = millis();
    if (snooze_checked == 0){
      previous_time = current_time;
      snooze_checked = 1;
    }

    if ((unsigned long)(current_time - previous_time) >= snooze_time){
      previous_time = current_time;
      analogWrite(buzzer_pin, sound);
      is_snoozed = 0;
      snooze_checked = 0;
      is_ringing = true;
    }
  }
 // stop button
 if (is_stoped == 1){
  analogWrite(buzzer_pin, 0);
  delay(1000);
  is_stoped = 0;
  is_ringing = false;
 }

}
