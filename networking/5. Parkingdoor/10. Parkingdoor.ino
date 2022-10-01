#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <Servo.h>
#include "index_html.h"

#define D1 5
#define D2 4
#define D4 2
#define D7 13
#define D8 15
#define D3 0
#define close_time 20 * 1000
#define barrier_distance 4
#define OPEN_ANGLE 90
#define CLOSE_ANGLE 0

const char* ssid = "";
const char* password = "";

int trigger_pin = D4;
int echo_pin = D2;
int led_pin = D7;
int buzzer_pin = D1;
Servo S;

unsigned long previous_time = 0;
unsigned long current_time = 0;
long distance;
int open_val = 0;
int close_val = 0;
int is_closed = 1;
String message = "ok";

AsyncWebServer server(80);

void setup() {
   Serial.begin(9600);
   Serial.println("setup");
   pinMode(echo_pin, INPUT);
   pinMode(trigger_pin, OUTPUT);
   pinMode(led_pin, OUTPUT);
   pinMode(buzzer_pin, OUTPUT);
   S.attach(D8);
   S.write(0);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();

  Serial.print("Local IP:");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request -> send_P(200, "text/html", index_html);
  });


  server.on("/open", HTTP_GET, [] (AsyncWebServerRequest *request) {
    open_val = 1;
    Serial.println("open button was pressed");
    request -> send(200, "text/plain", "OK");
  });

  server.on("/close", HTTP_GET, [] (AsyncWebServerRequest *request) {
    close_val = 1;
    Serial.println("close button was pressed");
    request -> send(200, "text/plain", "OK");
  });

    server.on("/msg", HTTP_GET, [] (AsyncWebServerRequest *request) {
      request -> send(200, "text/plain", message);
      message = "ok";
    });

  server.begin();
}

void loop() {
  update_distance();
  current_time  = millis();


  if (open_val == 1 && is_closed == 1){
    close_val = 0;
    open_door();
    previous_time = current_time;
  }
  if ((current_time - previous_time >= close_time) && is_closed == 0){
      Serial.println("timeout");
      Serial.print("current: ");
      Serial.println(current_time);
      Serial.print("prev: ");
      Serial.println(previous_time);
      close_val = 1;
  }

  if ((close_val == 1) && (distance > barrier_distance) && (is_closed == 0)){
    open_val = 0;
    close_door();
  }
  else if ((close_val == 1) && (is_closed == 0) && (distance <= barrier_distance)){
    close_val = 0;
    previous_time = current_time;
    digitalWrite(buzzer_pin, HIGH);
    message = "barrier is detected. door cannot close.";
    delay(1000);
    digitalWrite(buzzer_pin, LOW);
  }

}

void open_door(){
  digitalWrite(led_pin, HIGH);
  for(int i = CLOSE_ANGLE; i <= OPEN_ANGLE; i += 5){
    S.write(i);
    delay(150);
  }
  digitalWrite(led_pin, LOW);
  is_closed = 0;
}
void close_door(){
  digitalWrite(led_pin, HIGH);
  for(int i = OPEN_ANGLE; i >= CLOSE_ANGLE; i -= 5){
    if(check_barrier()){
      digitalWrite(led_pin, LOW);
      open_val = 1;
      is_closed = 0;
      close_val = 0;
      previous_time = current_time;
      return;
    }
    S.write(i);
    delay(150);
  }
  digitalWrite(led_pin, LOW);
  is_closed = 1;
}

void update_distance(){
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  distance = (pulseIn(echo_pin, HIGH) / 2) / 29.1;
}

bool check_barrier(){
  update_distance();
  if ((is_closed == 0) && (distance <= barrier_distance) && (open_val == 0)){
    S.write(OPEN_ANGLE);
    digitalWrite(buzzer_pin, HIGH);
    message = "barrier is detected. door open.";
    delay(1000);
    digitalWrite(buzzer_pin, LOW);
    return true;
  }
  return false;
}
