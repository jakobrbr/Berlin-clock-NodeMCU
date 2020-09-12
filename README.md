# Berlin-clock-NodeMCU
WiFi enabled Mengenlehreuhr aka Berlin clock made using ESP8266 NodeMCU and two 74HC595 shift registers.

The clock is based on a real clock in Berlin from 1975. My goal was to replicate the original design as best as I could, in a small "desktop form factor". 
You can read about much more about the original clock here: https://en.wikipedia.org/wiki/Mengenlehreuhr

This was my first real Arduino project, (...)

## How it works:
The clock uses the WiFi capabilities of the NodeMCU to always have the correct time without the need of a clock module. 

### LED matrices
In order to save the amount of I/O pins in use, two 74HC595 shift registers are used to control two LED matrices.
The first marix (matrix1) is a 4x4 matrix containing row 1, 2 and 4 of the clock (counting from the top - see [image](https://commons.wikimedia.org/wiki/File:Berlin-Uhr-1650-1705.gif#/media/File:Berlin-Uhr-1650-1705.gif)). Note that we only use 3 rows in matrix1. Matrix row 4 represents LED row 4 on the clock; this means that matrix1 row 3 is "empty".
(...)

The second matrix (matrix2) is a 4x4 matrix containing all elleven elements of LED row 3. (...)

The top seconds indicator LED uses D4 on the NodeMCU. (...)

## Schematics
(...)
The code I used to multiplex LED with a 74HC595 comes from [this instructables by Amanda Ghassaei](https://www.instructables.com/id/Multiplexing-with-Arduino-and-the-74HC595/).

## 3D printed case
(...)
I used tinkercad.com to create all 3d print files.
