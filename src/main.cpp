#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

// configurations begin

const char* ssid = "QuoteTicker";
const char* password = "12345678";

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 20; // In milliseconds
uint8_t spacer = 1;
uint8_t width = 5 + spacer; // The font width is 5 pixels

// configurations over

/* Put IP Address details */
IPAddress local_ip(192,168,0,77);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

String messages {"Hello vilag"};
String messagesOneline {""};

String html_start {"<html><head><style>.u-full-width { width: 100%; box-sizing: border-box; } .button, button, input { display: block; height: 38px; padding: 0 30px; color: #FFF; text-align: center; font-size: 11px; font-weight: 600; line-height: 38px; letter-spacing: .1rem; text-transform: uppercase; text-decoration: none; white-space: nowrap; background-color: #33C3F0; border-radius: 4px; border: 1px solid #33C3F0; cursor: pointer; box-sizing: border-box; } textarea { background-color: #fff; border: 1px solid #D1D1D1; border-radius: 4px; box-shadow: none; box-sizing: border-box; } textarea { -webkit-appearance: none; -moz-appearance: none; appearance: none; } textarea { min-height: 65px; padding-top: 6px; padding-bottom: 6px; } textarea:focus { border: 1px solid #33C3F0; outline: 0; }</style><title>QuoteTicker</title><link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css\" integrity=\"sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO\" crossorigin=\"anonymous\"><script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js\" integrity=\"sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy\" crossorigin=\"anonymous\"></script></head><body><h1>Add meg az id&eacute;zetet</h1><form action=\"/\" method=\"POST\"><textarea class=\"u-full-width\" id=\"exampleMessage\" name=\"message\" rows=6>"};
String html_end {"</textarea><input class=\"button-primary\" type=\"submit\" value=\"K&uuml;ld&eacute;s!\"></form></body></html>"};

void showPage();
void handlePost();
void handle404();
void printMessage();

void setup() {
  messagesOneline = messages;
  messagesOneline.replace("\n", "   ");
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
  matrix.setIntensity(7);
}

void loop() {
  server.handleClient();
  // display message on the LEDs
  for ( uint16_t i = 0 ; i < width * messagesOneline.length() + matrix.width() - 1 - spacer; i++ ) {

    matrix.fillScreen(LOW);

    uint16_t letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < messagesOneline.length() ) {
        matrix.drawChar(x, y, messagesOneline[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

    delay(wait);
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
  messagesOneline = messages;
  messagesOneline.replace("\n", "   ");
  Serial.println(messages);
  
  // save to EEPROM

  showPage();
}

void handle404() {
  server.send(404, "text/plain", "Not found");
}
