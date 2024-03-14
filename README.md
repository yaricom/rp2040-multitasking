# rp2040-multitasking
The research on simple multitasking with RP2040 to emulate RTOS

# Arduino IDE setup

It was made for [LiLyGO T-Display RP2040](https://github.com/Xinyuan-LilyGO/LILYGO-T-display-RP2040/tree/main) 
using [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI/tree/master) library as a display driver.

The following dependecies should be installed:
1. [Earle Philhower's board package](https://github.com/earlephilhower/arduino-pico) according to instructions below
2. [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI/tree/master) library according to instructions below


## Earle Philhower's board package

You can install Earle Philhower's board package using additional boards manager of Arduino IDE as following (MacOS):
1. Start the Arduino IDE and go to `Arduino IDE->Settings`.
2. At the bottom in the "Additional boards manager URLs" field type link to the board package repository:
```shell
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```
3. Go `Tools->Boards manager` in the IDE menu
4. Type 'pico' in the search box and select "Add"
5. Make sure to install version `3.2.2` **not higher**, because the latest versions doesn't work properly with TFT_eSPI library

## TFT_eSPI library

1. Clone [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI/tree/master) repository
2. Copy files from repository into `libraries` directory inside of the Arduino sketches directory
3. Edit file `User_Setup_Select.h` to use correct driver for the Lilygo T-Display RP2040 (ST7789 on SPI bus with 135x240 TFT) 
as following:
  - uncomment the line `#include <User_Setups/Setup137_LilyGo_TDisplay_RP2040.h>` 
  - comment out the line `#include <User_Setup.h>`.
