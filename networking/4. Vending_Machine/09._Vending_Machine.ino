#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "index_html.h";
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// pins
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

// servo config
#define OPEN_ANGLE 90
#define CLOSE_ANGLE 0
Servo S;

// rfid
const uint8_t RST_PIN = D3;     
const uint8_t SS_PIN = D4;     

MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;

String not_eligible = "131140132155";
String eligible = "16218010527";
int budget = 20000;
String tag;
String message = "";
// Replace with your network credentials
const char *ssid     = "";
const char *password = "";
bool transaction_succeed = true;

AsyncWebServer server(80);

String processor(const String& var){
  return String();
}

String desired_good = "";

void setup(){


  // Serial port for debugging purposes
  Serial.begin(9600);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  
  // LED
  pinMode(D1, OUTPUT);  

  // servo
  S.attach(D8);
  S.write(CLOSE_ANGLE);

  // Buzzer
  pinMode(D2, OUTPUT);  

  // RFID settings   
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522


  // Route for root /yaghob web page
  server.on("/yaghob", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/check", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<100> data;
    data["message"] = message;
    data["succeed"] = transaction_succeed;
    if (!transaction_succeed){
      transaction_succeed = true;
      message = "";
    }
    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/order",
    [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<200> data;
    if (json.is<JsonArray>())
    {
      data = json.as<JsonArray>();
    }
    else if (json.is<JsonObject>())
    {
      data = json.as<JsonObject>();
    }
    desired_good = data["Food"].as<String>();
    String response;
    StaticJsonDocument<100> resp_json;
    message = "کارت دانشجویی خود را بزنید";
    resp_json["message"] = message;
    serializeJson(resp_json, response);
    
    request->send(200, "application/json", response);
    Serial.println(response);
  });
  server.addHandler(handler);


  server.begin();
}

  
void loop() {
  if ( ! rfid.PICC_IsNewCardPresent() && desired_good != "")
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    if(tag == eligible){
      Serial.println("access granted");
      if(desired_good == "Milk")
          transaction_succeed = transaction(7000);
      else if(desired_good == "Soda")
          transaction_succeed = transaction(5000);
      else if(desired_good == "Peanut Pack")
          transaction_succeed = transaction(4000);
      else if(desired_good == "Chips")
          transaction_succeed = transaction(6000);
          
    }else{
      Serial.println("access denied");
      desired_good = "";
      message = "این دسترسی برای شما وجود ندارد";
      transaction_succeed = false;
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}

bool transaction(int price){
  if(price <= budget){
    budget -= price;
    S.write(OPEN_ANGLE);
    digitalWrite(D1, HIGH);
    delay(3000);
    digitalWrite(D1,LOW);
    S.write(CLOSE_ANGLE);
    desired_good = "";
    message = "";
    return true;
  }else{
    digitalWrite(D2, HIGH);
    delay(1000);
    digitalWrite(D2, LOW);
    desired_good = "";
    message = "بودجه ی شما ناکافی است";
    return false;
  }
}
