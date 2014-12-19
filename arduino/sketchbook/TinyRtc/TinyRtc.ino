#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

void setup() {
  // put your setup code here, to run once:
/*
SCL –> A5 Analog Pin
SDA –> A4 Analog Pin
VCC –> 5V
GND –> GND
*/
  //Initialize the serial port, wire library and RTC module
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    //If we remove the comment from the following line, we will set up the module time and date with the computer one
    //RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop() {
  // put your main code here, to run repeatedly:
    DateTime now = RTC.now();
    //We print the day
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
    //We check the time and sent through the serial port every 3s
    delay(3000);
}
