#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// pins 
#define D1 5
#define D2 4
#define D3 0
#define D4 2



WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String client_id = "agent1";
unsigned long current_time = 0;
unsigned long previous_time = 0;
unsigned int ping_log_time = 10000;

int ldr_value = 0;


const char *ssid     = "";
const char *password = "";

const char *SERVER_IP = "192.168.1.128";


const char* PING_TOPIC = "I1820/main/agent/ping";
const char* LOG_TOPIC = "I1820/main/log/send";
const char* REQUEST_TOPIC = "I1820/main/configuration/request";
const char* MESSAGE_TYPE = "lamp";

void ping(){
    StaticJsonDocument<512> resp_json;
    resp_json["id"] = client_id;
    resp_json["things"][0]["id"] = "t1";
    resp_json["things"][0]["type"] = "lamp";
    resp_json["things"][1]["id"] = "t2";
    resp_json["things"][1]["type"] = "lamp";
    resp_json["things"][2]["id"] = "t3";
    resp_json["things"][2]["type"] = "lamp";
    resp_json["things"][3]["id"] = "t4";
    resp_json["things"][3]["type"] = "lamp";
    resp_json["things"][4]["id"] = "l1";
    resp_json["things"][4]["type"] = "light";
    char msg[512];
    serializeJson(resp_json, msg);
    client.publish(PING_TOPIC , msg);
}

void logg(){
  ldr_value = analogRead(A0);
  int ldr_mapped = map(ldr_value , 0 , 1023 , 0 , 100);
  StaticJsonDocument<512> resp_json;
  resp_json["agent"] = client_id;
  resp_json["timestamp"] = timeClient.getEpochTime(); 
  resp_json["type"] = "light";
  resp_json["device"] = "l1";
  resp_json["states"][0]["name"] = "light";
  resp_json["states"][0]["value"] = ldr_mapped;
  char msg[512];
  serializeJson(resp_json, msg);
  client.publish(LOG_TOPIC , msg);
}




void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("connected to the wifi");

  //connecting to a mqtt broker
  client.setServer(SERVER_IP, 1883);
  client.setCallback(callback);
  
  while (!client.connected()) {
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str())) {
          Serial.println("mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }

  timeClient.begin();
  timeClient.setTimeOffset(12600);
  client.subscribe(REQUEST_TOPIC);
// LED pins 
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);  

  
}


void callback(char* topic, byte* payload, unsigned int length){
  
  Serial.printf("Message arrived [%s]\n", topic);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  char* jsonBody = (char*) payload;
  if(!strcmp(topic, REQUEST_TOPIC)){
    StaticJsonDocument<512> doc;

    DeserializationError error = deserializeJson(doc, jsonBody, length);
    
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
  

    if(!strcmp("lamp" , doc["type"])){
      if(!strcmp("t1" , doc["device"])){
        if(doc["settings"][0]["value"]){
          digitalWrite(D1,HIGH);
        }else{
          digitalWrite(D1,LOW);
        }
        
      }else if(!strcmp("t2" , doc["device"])){
        if(doc["settings"][0]["value"]){
          digitalWrite(D2,HIGH);
        }else{
          digitalWrite(D2,LOW);
        }
      }else if(!strcmp("t3" , doc["device"])){
        if(doc["settings"][0]["value"]){
          digitalWrite(D3,HIGH);
        }else{
          digitalWrite(D3,LOW);
        }
      }else if(!strcmp("t4" , doc["device"])){
        if(doc["settings"][0]["value"]){
          digitalWrite(D4,HIGH);
        }else{
          digitalWrite(D4,LOW);
        }
      }
    }
  }
  
}


void loop() {
  timeClient.update();
  current_time = millis();
  client.loop();
  if ((unsigned long)(current_time - previous_time) >= ping_log_time){
    ping();
    logg();
    previous_time = current_time;  
  }
  

}
