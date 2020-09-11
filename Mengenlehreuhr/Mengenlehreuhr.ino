/*
 * Mengenlehreuhr aka Berlin Clock - a 24 hour LED WiFi clock
 * ESP8266 NodeMCU & 74HC595
 * by Jakob Rud Bernhardt
 * July 2020
 */
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char *ssid     = "Your wifi network name";
const char *password = "Your wifi password";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

int oldSecond, oldMinute, oldHour;

// seconds LED
#define secPin D4
// matrix 1; row 1, 2 and 4
#define latchPin1 D2
#define clockPin1 D1
#define dataPin1 D3
// matrix 2; row 3
#define latchPin2 D6
#define clockPin2 D5
#define dataPin2 D7

//looping variables
byte i;
byte j;

//storage variable
byte dataToSend1;
byte dataToSend2;

//storage for led states, 4 bytes
byte ledData1[4]; // storage array for row 1, 2 and 4
byte ledData2[4]; // storage array for row 3

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  //set pins as output
  pinMode(secPin, OUTPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(7200);// set offset time in seconds to adjust timezone, GMT = 0 and GMT+2 = 7200
}

void secLED(int currentSecond) {
  // top blinking LED, on/off for one second at a time
  int odd = currentSecond%2;
  if(odd){
    digitalWrite(secPin,HIGH); // active low, if the current seconds value is odd the LED is off
  } else if(!odd) {
    digitalWrite(secPin,LOW);
  }
}

void rowOne(int currentHour){
  // four elements, each one is 5 hours
  if (currentHour < 5) {  
    ledData1[0] = 0;
  } else if (currentHour < 10) {  
    ledData1[0] = 1;
  } else if (currentHour < 15) {
    ledData1[0] = 3;
  } else if (currentHour < 20) {
    ledData1[0] = 7;
  } else {
    ledData1[0] = 15;
  } 
}

void rowTwo(int currentHour){
  // four elements, each element is one hour
  int n = currentHour % 5;// integer n is the number of lit LEDs 

  switch (n) {
    case 1:
      // one LED
      ledData1[1] = 1;
      break;
    case 2:
    // two LEDs
      ledData1[1] = 3;
      break;
    case 3:
      // three LEDs
      ledData1[1] = 7;
      break;
    case 4:
      // four LEDs
      ledData1[1] = 15;
      break;
    default:
      ledData1[1] = 0;
    break; 
  }
}

void rowThree(int currentMinute){ 
  //11 elements, each element represents 5 minutes

  if (currentMinute < 5) {
    // full blank row
    ledData2[0] = 0;
    ledData2[1] = 0;
    ledData2[2] = 0;
    ledData2[3] = 0;
  } else if (currentMinute < 10) {
      // one LED
      ledData2[0] = 3;
      ledData2[1] = 0;
      ledData2[2] = 0;
      ledData2[3] = 0;
  } else if (currentMinute < 15) {
       // two LEDs
      ledData2[0] = 7;
      ledData2[1] = 0;
      ledData2[2] = 0;
      ledData2[3] = 0;
  } else if (currentMinute < 20) {
      // three LEDs
      ledData2[0] = 15;
      ledData2[1] = 0;
      ledData2[2] = 0;
      ledData2[3] = 0;
  } else if (currentMinute < 25) {
      // four
      ledData2[0] = 15;
      ledData2[1] = 1;
      ledData2[2] = 0;
      ledData2[3] = 0;
  } else if (currentMinute < 30) {
      // five
      ledData2[0] = 15;
      ledData2[1] = 3;
      ledData2[2] = 0;
      ledData2[3] = 0;
  } else if (currentMinute < 35) {
      // six
      ledData2[0] = 15;
      ledData2[1] = 7;
      ledData2[2] = 0;
      ledData2[3] = 0;
  } else if (currentMinute < 40) {
      // seven
      ledData2[0] = 15;
      ledData2[1] = 15;
      ledData2[2] = 0;
      ledData2[3] = 0;
  } else if (currentMinute < 45) {
      // eight
      ledData2[0] = 15;
      ledData2[1] = 15;
      ledData2[2] = 0;
      ledData2[3] = 1;
  } else if (currentMinute < 50) {
      // nine
      ledData2[0] = 15;
      ledData2[1] = 15;
      ledData2[2] = 0;
      ledData2[3] = 3;
  } else if (currentMinute < 55) {
      // ten
      ledData2[0] = 15;
      ledData2[1] = 15;
      ledData2[2] = 0;
      ledData2[3] = 7;
  } else if (currentMinute < 60) {
      // elleven
      ledData2[0] = 15;
      ledData2[1] = 15;
      ledData2[2] = 0;
      ledData2[3] = 15;
  }
}

void rowFour(int currentMinute){// bottom row
  // four LEDs light up, with one LED corresponding to one minute passed
  // after five minutes the entire row will turn blank and start over
  
  int lastDigit = currentMinute % 10; // gets last digit of minutes

  if (lastDigit == 1 || lastDigit == 6) {
    // one LED
    ledData1[3] = 1;
  } else if (lastDigit == 2 || lastDigit == 7) {
    // two LEDs
    ledData1[3] = 3;
  } else if (lastDigit == 3 || lastDigit == 8) {
    // three LEDs
    ledData1[3] = 7;
  } else if (lastDigit == 4 || lastDigit == 9) {
    // four LEDs
    ledData1[3] = 15;
  } else if (lastDigit == 5 || lastDigit == 0) {
    // full blank row
    ledData1[3] = 0;
  } 
}


void loop() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  int currentHour = timeClient.getHours();

  // to prevent excessive calculations, the LED update functions are only run if the time has changed
  if (currentSecond != oldSecond){
    secLED(currentSecond);
  } 
  if (currentMinute != oldMinute) {
    rowFour(currentMinute);
    secLED(currentSecond); // the seconds-LED is updated again in case the previous update function is slow
    rowThree(currentMinute);
    if (currentHour != oldHour) {
      secLED(currentSecond); // the seconds-LED is once again updated
      rowOne(currentHour);
      rowTwo(currentHour);
    }
  }

  // "demultiplexing"-code: _________________________________________________________
   for (i=0;i<4;i++){
    //send data from ledData to each row, one at a time
    byte dataToSend1 = (1 << (i+4)) | (15 & ~ledData1[i]); // for LED matrix containing row 1, 2 and 4
    byte dataToSend2 = (1 << (i+4)) | (15 & ~ledData2[i]); // for LED matrix containing row 3
      
    // set latch pin low so the LEDs don't change while sending in bits
    digitalWrite(latchPin1, LOW);
    digitalWrite(latchPin2, LOW);
    
    // shift out the bits of dataToSend to the 74HC595
    // shift out(dataPin, clockPin, LSBFIRST, dataToSend);
    // the code below is the equivalent of the two lines above
    for (j=0;j<8;j++){
      // row 1, 2 and 4
      digitalWrite(clockPin1,LOW);
      digitalWrite(dataPin1,((dataToSend1>>j)&1));
      digitalWrite(clockPin1,HIGH);
      // row 3
      digitalWrite(clockPin2,LOW);
      digitalWrite(dataPin2,((dataToSend2>>j)&1));
      digitalWrite(clockPin2,HIGH);
    }
    //set latch pin high- this sends data to outputs so the LEDs will light up
    digitalWrite(latchPin1, HIGH);
    digitalWrite(latchPin2, HIGH);   
  }
  // end of "demultiplexing"-code __________________________________________________
  
  oldSecond = currentSecond;
  oldMinute = currentMinute;
  oldHour = currentHour;
}
