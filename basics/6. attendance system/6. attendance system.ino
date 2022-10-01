#include<Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// pins
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D8 15

// servo config
#define OPEN_ANGLE 90
#define CLOSE_ANGLE 0
Servo S;


// wifi settings 
const char *ssid     = "";
const char *password = "";

// student tags 
#define CLASS_CAPACITY 2
String student1 = "131140132155";
String student2 = "16218010527";
String names[2] = { "Alireza", "Mohammad"};
bool presence[2] = {false ,false};


time_t current_epoch = 0;

// the state of the class
bool prof_in = false;
time_t  prof_entrance_time = 0;
int allow_entrance_time = 10 * 60; // seconds
int end_of_the_class = 15 * 60; //seconds

// push button
int ldr_value = 0;
int shadow_theshold = 200;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const uint8_t RST_PIN = D3;     
const uint8_t SS_PIN = D4;     

MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;

String tag;

void setup() {
  Serial.begin(9600);

  // RFID settings   
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  pinMode(D8, OUTPUT);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(16200);

  // LED
  pinMode(D1, OUTPUT);  

  // servo
  S.attach(D8);
  S.write(CLOSE_ANGLE);

  // Buzzer
  pinMode(D2, OUTPUT);  

}

void loop() {
  //  update current time 
  timeClient.update();

  current_epoch = timeClient.getEpochTime();
  
  ldr_value = analogRead(A0);
  if(ldr_value < shadow_theshold){
    accessGranted("welcome professor");
    prof_in = true;
    prof_entrance_time = current_epoch;
  }
  
  if( prof_in && (current_epoch > prof_entrance_time + end_of_the_class)){
    prof_in = false;
    Serial.println("CLASS STAT");
    for(int i; i < CLASS_CAPACITY; i++){
      Serial.print("name: ");
      Serial.print(names[i]);
      Serial.print(" present: ");
      Serial.println(presence[i]);
      presence[i] = false;
    }
  }
  
     
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    log();
    bool is_in_time =current_epoch <= prof_entrance_time + allow_entrance_time;
    
    if(prof_in){
      if(tag == student1 && is_in_time){
        presence[0] = true;
        accessGranted("access granted");
      }else if(tag == student2 && is_in_time){
        presence[1] = true;
        accessGranted("access granted");
      }else{
        accessDenied("access denied (you are late!!!)");
      }
    }else{
      accessDenied("No Class");
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}

void accessDenied(String msg){
  Serial.println(msg);
  digitalWrite(D2, HIGH);
  delay(1000);
  digitalWrite(D2, LOW);
}

void accessGranted(String msg){
  Serial.println(msg);
  S.write(OPEN_ANGLE);
  digitalWrite(D1, HIGH);
  delay(3000);
  digitalWrite(D1,LOW);
  S.write(CLOSE_ANGLE);
}

void log(){
  
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  Serial.print("==>");
  Serial.println(tag);
}
