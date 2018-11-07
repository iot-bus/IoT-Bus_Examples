# IoT-Bus Examples

These examples are designed to run on IoT-Bus boards by oddWires. In each case, they demonstrate how use each of the IoT-Bus boards. In the examples that need networks access you will find a section like this in the code to enter your ssid and password.

    //TODO: Hard-code your WiFi credentials here (and keep it private)
    const char* ssid = "........";
    const char* password = "........";

These examples are structured for PlatformIO in the github repository *iot-bus-examples-platformio* and for Arduino in *iot-bus-examples-arduino*.  If you run in PlatformIO, PLatformIO will automatically download the library dependencies specified the examples platformio.ini file. If you run under Arduino you'll have to ensure each library is installed in the "Arduino/Libraries" folder by using the Arduino Library Manager. In most cases you can find the library by searching for it. If you can't find it in the Library Manager then download and install it as a zip file. You will find the library dependencies for each example below.

![iot-bus-example](meter.jpg)

## iot-bus-blink

This example shows how to flash the built-in LED on GPIO5. It requires no additional libraries besides Arduino.

## iot-bus-hello-world

This example creates a simple webserver and displays the response on a browser. It requires AsyncWebserver from: https://github.com/me-no-dev/ESPAsyncWebServer. 

## iot-bus-display-touch

There are three examples using the Adafruit ILI9341 library and three using Bodmer's TFT_eSPI library. The Adafruit libraries are available from https://github.com/adafruit/Adafruit-GFX-Library and
https://github.com/adafruit/Adafruit_ILI9341. Bodmer's library is available from https://github.com/iot-bus/TFT_eSPI. This fork has got the correct User_Setup.h file for IoT-Bus. Where a touch screen is used we have forked Paul Stoffgren's XPT2046_Touchscreen here: https://github.com/iot-bus/XPT2046_Touchscreen. This library has two key changes to the original. The first provides an ability to get a mapped point rather than raw touch data so you can work in pixels. The second change is to disable the PENIRQ so that we can ensure the IRQ pin can be used for other boards besides the display.

### ILI9341_graphicstest

This example uses the Adafruit ILI9341 and the Adafruit-GFX-Libraries to perform a simple graphics test on the display. 

### spitftbitmap

This example uses the Adafruit ILI9341 and the Adafruit-GFX-Libraries to continuously display and re-display a bitmap image. You will need to place the tiger.bmp image on an SD card and insert it into the SD card socket on the display board. 

### spitftbitmap-single-draw

This example uses the Adafruit_ILI9341 and the Adafruit-GFX-Library to display a bitmap image. You will need to place the tiger.bmp image on an SD card and insert it into the SD card socket on the display board. 

### drawcorners

This example uses Bodmer's TFT_eSPI to obtain the raw touch values at the corners of the display. These can be used to calibrate the display to get more accurate touch results. In practice, we find you get reasonable results without calibration.

### tft-meters

This example uses Bodmer's TFT_eSPI to show off the display capabilities.

### touch-draw-2-4

This example uses Bodmer's TFT_eSPI to use the touch capabilities and display to offer a simple paint application.

## iot-bus-sdcard

There are two examples using the SD Card on the IoT-Bus Display board. No libraries apart from the Arduino core libraries are required.

### iot-bus-sdcard

This example uses standard SPI to access an SD card. You will need to put an SD card in the socket on the display board.

### iot-bus-sdmmc

This example uses 4-bit SDIO to access an SD card. You will need to put an SD card in the socket on the display board.

## iot-bus-relay 

This example uses shows how to drive the IoT-Bus Relay using GPIO 17. The relay can be used on pins 16 or 17 by the means of a 0 OHM resistor on the board on either IO16 or IO17. Pin 17 is required if using in conjunction with the display. No additional libraries are required.

## iot-bus-motor 

This example show how to control a stepper motor using the IoT-Bus Motor board. It uses the 

## iot-bus-lora

These two example show how to send and receive using IoT-Bus LoRa.  They use Sandeep Mistry' LoRa library: https://github.com/sandeepmistry/arduino-LoRa.

### iot-bus-lora-sender

Send LoRa packet data.

### iot-bus-lora-receiver

Receive LoRa packet data.

## iot-bus-canbus

These two example show how to send and receive using IoT-BUs CAN Bus. They use Sandeep Mistry' arduino-CAN library: https://github.com/sandeepmistry/arduino-CAN. Note that Sandeep has a very nice OBD2 library, arduino-OBD2, that can be used in conjunction with this CAN BUs library to read OBD2 data. You can find that here: https://github.com/sandeepmistry/arduino-OBD2.

### iot-bus-canbus-send

Send CAN Bus frame data.

### iot-bus-canbus-receive 

Receive CAN Bus frame data.
