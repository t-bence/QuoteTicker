# QuoteTicker

An Arduino program to display some text on LED boards. The text can be set using a WiFi access point.

The code uses some elements of [this](https://lastminuteengineers.com/creating-esp8266-web-server-arduino-ide/] and [this](https://howtomechatronics.com/tutorials/arduino/8x8-led-matrix-max7219-tutorial-scrolling-text-android-control-via-bluetooth/) tutorial.  
[markruys' MAX7219 library](https://github.com/markruys/arduino-Max72xxPanel) library is used to drive the LED matrix together with [Adafruit's GFX library](https://github.com/adafruit/Adafruit-GFX-Library).
Parts of the [Skeleton CSS toolkit](http://getskeleton.com/) are used, as well.

# TODO
- [x] Make it read text submitted via a `textarea`.
- [ ] Make it display and scroll the text.
- [ ] Make it store the text when switched off.
- [ ] Store strings somewhere.