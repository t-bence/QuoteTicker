#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,0,77);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);


void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();

  // do application logic
  // read new text
  // save to EEPROM
  // write out
}

void handle_OnConnect() {

  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", "HTML text comes here"); 
}

void handle_led1on() {
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", "HTML comes here too"); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}