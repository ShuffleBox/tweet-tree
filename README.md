# Tweet-Tree

## Overview
Tweet-tree is a project to expand upon the Adafruit [Christmahanukwanzaa tree](https://learn.adafruit.com/wifi-controlled-led-christmahanukwanzaa-tree/) project by making the tree Tweet reactive.

It is completely conceptual at this point in time, but will be commiting code and notes as I go.

### Parts
#### Server
- Raspberry Pi
- [Adafruit RGB 16x2 LCD + Keypad Kit](http://www.adafruit.com/product/1110)

#### Tree
- Arduino Uno
- Ethernet Shield
- Neo Pixel Strip

### Software
#### TweetStream
requires
 
- RPi.GPIO
- [Adafruit RGB LCD Library](https://github.com/adafruit/Adafruit_Python_CharLCD) - Don't forget the setup for i2c and [other configuration requirements](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c)