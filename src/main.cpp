#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

// configurations begin

const char* ssid = "IdezetGep";
const char* password = "12345678";

int pinCS = D4; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 55; //45; // In milliseconds
uint16_t spacer = 1;
uint16_t width = 5 + spacer; // The font width is 5 pixels

// configurations over

IPAddress local_ip(192,168,0,77);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

String messages {"Hello vilag"};
String messagesOneline {""};

String html_start {"<html><head><style>h1 {font-size: 55px; } .u-full-width { width: 100%; box-sizing: border-box; } .button, button, input { display: block; height: 200px; padding: 0 30px; color: #FFF; text-align: center; font-size: 50px; font-weight: 600; line-height: 38px; letter-spacing: .1rem; text-transform: uppercase; text-decoration: none; white-space: nowrap; background-color: #33C3F0; border-radius: 4px; border: 1px solid #33C3F0; cursor: pointer; box-sizing: border-box; } textarea { font-size: 38px; background-color: #fff; border: 1px solid #D1D1D1; border-radius: 4px; box-shadow: none; box-sizing: border-box; } textarea { -webkit-appearance: none; -moz-appearance: none; appearance: none; } textarea { min-height: 65px; padding-top: 6px; padding-bottom: 6px; } textarea:focus { border: 1px solid #33C3F0; outline: 0; }</style><title>QuoteTicker</title><link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css\" integrity=\"sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO\" crossorigin=\"anonymous\"><script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js\" integrity=\"sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy\" crossorigin=\"anonymous\"></script></head><body><h1>Add meg az id&eacute;zetet</h1><form action=\"/\" method=\"POST\"><textarea class=\"u-full-width\" id=\"exampleMessage\" name=\"message\" rows=6>"};
String html_end {"</textarea><input class=\"button-primary\" type=\"submit\" value=\"K&uuml;ld&eacute;s!\"></form></body></html>"};

void showPage();
void handlePost();
void handle404();
void printMessage();

void setup() {
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
  matrix.setIntensity(2);
  
  matrix.setRotation(0, 1);
  matrix.setRotation(1, 1);
  matrix.setRotation(2, 1);
  matrix.setRotation(3, 1);

}

void loop() {
  // display message on the LEDs
    for ( uint16_t i = 0 ;
          i < width * messagesOneline.length() + matrix.width() - 1 - spacer;
          i++
        ) {
      //server.handleClient();

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
    matrix.fillScreen(LOW);
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
  messagesOneline.replace("\r\n", "   ");
  Serial.println(messages);
  
  // save messages
  showPage();
}

void handle404() {
  server.send(404, "text/plain", "Not found");
}