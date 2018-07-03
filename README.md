# ESP8266 controlled guggen hat.

The hat was constructed as described in https://learn.adafruit.com/guggenhat-bluetooth-le-wearable-neopixel-marquee-scrolling-led-hat-display/overview , with 37 light per round, 8 rounds with 4 light extra (total 300 lights). The LED strip is directional. The code was written for LED strip go from top to bottom. When coiling LED strip on the hat,  it goes counter-clock wise from start to end if looking from top of hat.
If coiled differently, software setting need to be changed to correct that.

The WS2812B has 3 pins: vcc, gnd and data pin. I used thd D3 pin of an ESP8266 wemo D1 board connect to the single data pin of the WS2818B, and use the USB portable battery to power the ESP8266 and the WS2818B. 

The password needs to be 8 digits. I used a 5v portable usb battery connect to a nodemcu, and nodemcu pin to the LED directly. The 3.3v pin signal works with the LED strip, no level shift is needed.

After battery connected, from your phone or laptop, connect to wifi with SSID hat2, and enter password 12345678 (can be changed). open web browser and connect to http://192.168.4.1. Safari does not work, I might need to modify the code to work with iphone.

## Examples


## Limitations
It does not work with safari browser, due to html incompatibility. Some simple modification is needed, I do not have an iphone or mac to test with.

## Compatible Hardware

WS2812B strip(16.4ft 300 LED Pixel) can be purchased on Amazon. Any ESP8266 board would work. I used this hat https://www.amazon.com/gp/product/B005TFNY6S/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1 . 

## License

This code is released under the MIT License.