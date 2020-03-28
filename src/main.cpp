#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <MD_Parola.h>
#include <EEPROM.h>

// configurations begin

const char* ssid = "IdezetGep";
const char* password = "12345678";

// graphics conf

#define MAX_DEVICES 4  // Number of modules connected
#define CLK_PIN   D5   // SPI SCK pin on UNO
#define DATA_PIN  D7   // SPI MOSI pin on UNO
#define CS_PIN    D4   // connected to pin 10 on UNO

MD_Parola P = MD_Parola(MD_MAX72XX::FC16_HW, CS_PIN, MAX_DEVICES);  // SPI config

int scrollSpeed {41};    // used to set text scroll speed in Parola at start

// sets scrolling direction if slider in middle at start
textEffect_t scrollEffect = PA_SCROLL_LEFT;

textPosition_t scrollAlign = PA_LEFT;  // how to aligh the text

int scrollPause = 0; // ms of pause after finished displaying message

#define	BUF_SIZE	600  // Maximum of 600 characters
char curMessage[BUF_SIZE] = { "" };  // used to hold current message
const uint16_t EEPROM_ADDR {0};

// configurations over

IPAddress local_ip(192,168,0,77);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

String messages {""};
String messagesOneline {""};

String html_start {"<html><head><style>h1 {font-size: 55px; } .u-full-width { width: 100%; box-sizing: border-box; } .button, button, input { display: block; height: 200px; padding: 0 30px; color: #FFF; text-align: center; font-size: 50px; font-weight: 600; line-height: 38px; letter-spacing: .1rem; text-transform: uppercase; text-decoration: none; white-space: nowrap; background-color: #33C3F0; border-radius: 4px; border: 1px solid #33C3F0; cursor: pointer; box-sizing: border-box; } textarea { font-size: 38px; background-color: #fff; border: 1px solid #D1D1D1; border-radius: 4px; box-shadow: none; box-sizing: border-box; } textarea { -webkit-appearance: none; -moz-appearance: none; appearance: none; } textarea { min-height: 65px; padding-top: 6px; padding-bottom: 6px; } textarea:focus { border: 1px solid #33C3F0; outline: 0; }</style><title>QuoteTicker</title><link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css\" integrity=\"sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO\" crossorigin=\"anonymous\"><script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js\" integrity=\"sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy\" crossorigin=\"anonymous\"></script></head><body><h1>Add meg az id&eacute;zetet</h1><form action=\"/\" method=\"POST\"><textarea class=\"u-full-width\" id=\"exampleMessage\" name=\"message\" rows=6>"};
String html_end {"</textarea><input class=\"button-primary\" type=\"submit\" value=\"K&uuml;ld&eacute;s!\"></form></body></html>"};

void showPage();
void handlePost();
void handle404();
void printMessage();

void setup() {

  EEPROM.get(EEPROM_ADDR, curMessage);
  messages = String(curMessage);

  messagesOneline = messages;
  messagesOneline.replace("\r\n", "   ");
  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", HTTP_GET, showPage);
  server.on("/", HTTP_POST, handlePost);
  server.onNotFound(handle404);
  
  server.begin();

  Serial.println("HTTP server started");

  // set up LED display
  P.begin(); 
  
  // configure Parola
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);

}

void loop() {

  server.handleClient();

  if (P.displayAnimate()) // If finished displaying message
  {
    P.displayReset();  // Reset and display it again
  }
}

void showPage() {
  // display current settings on webpage
  Serial.println("Showing page");
  server.send(200, "text/html", html_start + messages + html_end);

}

void handlePost() {
  // save & display
  // get text
  Serial.println("POST request: ");
  messages = server.arg(0);
  messages.replace("ő", "ö");
  messages.replace("ű", "ü");
  messages.replace("Ő", "Ö");
  messages.replace("Ű", "Ü");
  messagesOneline = messages;
  messagesOneline.replace("\r\n", "   ");
  
  messagesOneline.toCharArray(curMessage, BUF_SIZE);
  EEPROM.put(EEPROM_ADDR, curMessage);
  Serial.println(messages);
  
  // save messages
  showPage();
}

void handle404() {
  server.send(404, "text/plain", "Not found");
}