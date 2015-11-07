
/** 
* Wire is need for clock
* you will find RTClib at :
* https://github.com/adafruit/RTClib
*/
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

/**
* Libraries for DHT11
*/
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 2

/**
* Photoresistor pin
*/
#define PHOTORPIN 3

/**
* One measure every five minutes
*/
//#define FIVEMIN (1000UL * 60 * 5);
#define FIVEMIN (1000UL * 5);
unsigned long rolltime = millis() + FIVEMIN;

char msg[255] = "some_text_without_space";

void setup() {
  // put your setup code here, to run once:

  //Initialize the serial port, wire library and RTC module
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    
    // Uncomment to adjust time with computer
    //RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
 
 // signed comparison for proper handling of timer rollover
 
  if((long)(millis() - rolltime) >= 0) {
   DateTime now = RTC.now();
   //  Do your five minute roll stuff here
    Serial.print(now.day(), DEC);
    Serial.print('/');
    //We print the month
    Serial.print(now.month(), DEC);
    Serial.print('/');
    //We print the year
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    //We print the hour
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    //We print the minutes
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    //We print the seconds
    Serial.print(now.second(), DEC);
    Serial.println();
    //msg = getEnvironment(DHT11PIN);
    Serial.println(getEnvironment(DHT11PIN));
   rolltime += FIVEMIN;

 }
 
}

char *getEnvironment(int dht11pin)
{
  int chk = DHT11.read(dht11pin);
  sprintf(msg,"{\"temperature\":\"%i\",\"humidity\":\"%i\"}",DHT11.temperature,DHT11.humidity);

  return msg;
}

