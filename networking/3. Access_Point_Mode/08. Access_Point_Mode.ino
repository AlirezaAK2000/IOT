#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

const char *ssid     = "";
const char *password = "";

ESP8266WebServer server(80);
IPAddress local_IP(192,168,4,2);
IPAddress gateway(192,168,4,4);
IPAddress subnet(255,255,255,0);

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.println("initial IP address");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(myIP);
  Serial.print("New IP address: ");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  IPAddress newIP = WiFi.softAPIP();
  Serial.println(newIP);
  server.begin();

}

void loop() {
}
