# Tweet-Tree

## Overview
Tweet-tree is a project to expand upon the Adafruit [Christmahanukwanzaa tree](https://learn.adafruit.com/wifi-controlled-led-christmahanukwanzaa-tree/) project by making the tree Tweet reactive.

Currently: Project mostly works, but there are some tweepy exceptions that need to be handled.

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

#### Tweet-Tree
arduino sketch
- Set the IP address to work on your network on (or around) line 9
- Arduino listens on 80 for web request.  Arguments are parsed and used to signal different ornaments to light up.