# Berlin-clock-NodeMCU
IoT Mengenlehreuhr aka Berlin clock made using ESP8266 NodeMCU and two 74HC595 shift registers.

The clock is based on a real clock that was installed in Berlin in 1975. My goal was to replicate the original design as best as I could, in a small "desktop form factor". 
You can read much more about the original clock and how to tell the time on [Wikipedia](https://en.wikipedia.org/wiki/Mengenlehreuhr).

This DIY version of the Berlin clock is far from perfect, since it was my first real Arduino project and my first time designing a 3d-print model. 

**Picture of the finished product:** [Front](Images/Front.jpg) and [back](Images/Back.jpg)
- As you can see, the three red 3mm LEDs I used for row 3 were too bright compared to the yellow ones. These LEDs were super bright LEDs, and I only used them because I didn't have anything else available. Normal clear 3mm LEDs would have been enough (5mm for row 1,2 and 4). 

## How it works:
The clock uses the WiFi capabilities of the NodeMCU to always have the correct time without the need of a clock module. 
When powered on, the NodeMCU connects to the internet with the network credentials you entered in the arduino code. Then the NodeMCU requests and recieves the current time from an NTP server and displays it using the 24 LEDs. The code used to get time from an NTP server comes from [randomnerdtutorials.com](https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/). The LEDs are divided into two LED matrices that are controlled by two 74HC595 shift registers. This reduces the number of NodeMCU pins in use greatly, however one could easily manage to use even less. 
The NodeMCU is powered via a micro USB cable.

### LED matrices
In order to limit the amount of I/O pins in use, two 74HC595 shift registers are used to control two LED matrices.
The first marix (matrix1) is a 4x4 matrix containing row 1, 2 and 4 of the clock (counting from the top - see [image](https://commons.wikimedia.org/wiki/File:Berlin-Uhr-1650-1705.gif#/media/File:Berlin-Uhr-1650-1705.gif)). Note that only 3 rows of matrix1 are in use. Matrix1 row 4 represents LED row 4 on the clock; this means that matrix1 row 3 is "empty". The second matrix (matrix2) is a 4x4 matrix containing all elleven elements of LED row 3. See my diagrams in [**Schematic.pdf**](Schematic.pdf).

The code used to multiplex LEDs with a 74HC595 comes from [this instructables by Amanda Ghassaei](https://www.instructables.com/id/Multiplexing-with-Arduino-and-the-74HC595/).

## Schematics
All pin connections and schematics diagrams of the LED matrices can be found in [**Schematic.pdf**](Schematic.pdf).
The seconds indicator LED uses D4 and GND on the NodeMCU. I used 5mm LEDs for row 1, 2 and 3 and 3mm LEDs for row 4. I wouldn't use diffused LEDs, since they are too dim to shine through the front panels. (But if you don't plan on using the panels, diffused LEDs are perfect)

## 3D printed case
I used tinkercad.com to create [all 3d models](3d_models). Everything was printed in white PLA, which ensured that the front panels were translucent enough for the LEDs to shine through.
You may want to change the size and or thickness of the front panels to get your desired look and fit. I included an optional cover for the box, to cover the bright blue light from the NodeMCU, however it fits a bit loose. The tube is supposed to guide the wires from the LEDs down to the box that contains the shift registers, NodeMCU and resistors. The round hole in the box is too small for the tube, so you will either have make it larger or glue the parts together.
