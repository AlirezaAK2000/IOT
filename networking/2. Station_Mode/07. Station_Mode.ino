#include <ESP8266WiFi.h>

const char *ssid     = "";
const char *password = "";
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  networkList();
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("connected to " + String(ssid));
  Serial.println(WiFi.localIP());
}

void loop() {
}

void networkList(){
   Serial.println("Network lists: ");
   int count = WiFi.scanNetworks();
   if (count != -1){ 
     for (int i = 0; i < count; i++) {
        Serial.print(String(i) + " . ");
        Serial.println(WiFi.SSID(i));
     }
   }
  Serial.println("---------------------------");
}
