# QuoteTicker

## Description
An Arduino program to display some text on LED boards. The text can be set using a WiFi access point.

The code uses some elements of [this](https://lastminuteengineers.com/creating-esp8266-web-server-arduino-ide/] and [this](https://howtomechatronics.com/tutorials/arduino/8x8-led-matrix-max7219-tutorial-scrolling-text-android-control-via-bluetooth/) tutorial.  
[MajicDesigns' MD_MAX7219 library](https://github.com/MajicDesigns/MD_MAX72xx) library is used to drive the LED matrix together with [MajicDesigns' MD_Parola](https://github.com/MajicDesigns/MD_Parola). This enables the use of Hungaraian characters, as well.
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
- [x] Reduce flicker
- [x] Allow Hungarian characters

## Ideas, notes
* I could store the strings in the SPIFFS: descriptions [here](https://techtutorialsx.com/2018/08/05/esp32-arduino-spiffs-reading-a-file/) and [here](https://techtutorialsx.com/2019/05/28/esp8266-spiffs-writing-a-file/).
