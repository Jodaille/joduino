
#include <Wire.h>
#include "RTClib.h"
#include <dht11.h>

dht11 DHT11;
RTC_DS1307 RTC;

#define DHT11PIN 2
#define PHOTORPIN 3
#define FIVEMIN (1000UL * 60 * 5);
unsigned long rolltime = millis() + FIVEMIN;

char msg[255] = "some_text_without_space";

void setup() {
  // put your setup code here, to run once:
  //Initialize the serial port, wire library and RTC module
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
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

