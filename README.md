# QuoteTicker

## Description
An Arduino program to display some text on LED boards. The text can be set using a WiFi access point.

The code uses some elements of [this](https://lastminuteengineers.com/creating-esp8266-web-server-arduino-ide/] and [this](https://howtomechatronics.com/tutorials/arduino/8x8-led-matrix-max7219-tutorial-scrolling-text-android-control-via-bluetooth/) tutorial.  
[markruys' MAX7219 library](https://github.com/markruys/arduino-Max72xxPanel) library is used to drive the LED matrix together with [Adafruit's GFX library](https://github.com/adafruit/Adafruit-GFX-Library).
Parts of the [Skeleton CSS toolkit](http://getskeleton.com/) are used, as well.

## Pinout
According to [Hackster.io](https://www.hackster.io/FilippoOnesti/esp8266-clock-using-max7219-led-matrix-display-b036c7), the pinout for a Wemos ESP8266 is the following:
```
LED Matrix Pin -> ESP8266 Pin
Vcc            -> 3v  (3V on NodeMCU 3V3 on WEMOS)
Gnd            -> Gnd (G on NodeMCU)
DIN            -> D7  (Same Pin for WEMOS)
CS             -> D4  (Same Pin for WEMOS)
CLK            -> D5  (Same Pin for WEMOS)
```

## TODO
- [x] Make it read text submitted via a `textarea`.
- [x] Make it display and scroll the text.
- [ ] Make it store the text when switched off: SPIFFS?
- [ ] Reduce flicker

## Ideas, notes
* I could store the strings in the SPIFFS: descriptions [here](https://techtutorialsx.com/2018/08/05/esp32-arduino-spiffs-reading-a-file/) and [here](https://techtutorialsx.com/2019/05/28/esp8266-spiffs-writing-a-file/).

* Rotation introduces flicker. When using Adafruit's original `setRotation`, exposed by `Max72xxPanel::setRotation`, there is no flicker but the display won't work properly (one is upside down, the other is not, and there is no was to specify rotation on a per-display basis). So I have to look and combine the approaches from the two libraries.

* Removing delay and adding a timer cold help maybe? 

* The problem is not related to the WiFi part, it flickers even without that. Try other SPI pins maybe? D5...D8?

* Use [this](https://blog.adafruit.com/2019/04/08/the-adafruit-gfx-library-now-supports-unicode-adafruit-adafruit-josecastillo/) for Unicode support?