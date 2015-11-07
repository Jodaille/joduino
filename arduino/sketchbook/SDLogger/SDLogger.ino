/**
* JoBee monitor
* records temperature and log it on sdCard
* time from RTclock
* temperature with DS18B20
*/

/** 
* Wire is needed for clock
*/
#include <Wire.h>

/*
* Clock pins:
* A4 SDA
* A5 SCL
* you will find RTClib at :
* https://github.com/adafruit/RTClib
*/
#include "RTClib.h"
RTC_DS1307 RTC;

/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

*/
#include <OneWire.h>

/**
* DallasTemperature
* DS18B20 
* https://github.com/milesburton/Arduino-Temperature-Control-Library
*/
#include <DallasTemperature.h>

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/**
* One measure every five minutes
*/
#define FIVEMIN (1000UL * 60 * 5);
/**
* every five seconds
*/
//#define FIVEMIN (1000UL * 5);
unsigned long rolltime = millis() + FIVEMIN;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }*/
  //Initialize the serial port, wire library and RTC module
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  // Start up the library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
}

void loop() {
  if((long)(millis() - rolltime) >= 0) {
    // make a string for assembling the data to log:
    DateTime now = RTC.now();
    String dataString = "";
    dataString += now.day();
    dataString += "/";
    dataString += now.month();
    dataString += "/";
    dataString += now.year();
    dataString += " ";
    dataString += now.hour();
    dataString += ":";
    dataString += now.minute();
    dataString += ":";
    dataString += now.second();
    dataString += ",";
    sensors.requestTemperatures();
    // read three sensors and append to the string:
    for (int tempIndex = 0; tempIndex < 3; tempIndex++) {
      //int sensor = analogRead(analogPin);
      float sensor =  sensors.getTempCByIndex(tempIndex);
      dataString += String(sensor);
      if (tempIndex < 2) {
        dataString += ",";
      }
    }
  

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  rolltime += FIVEMIN;
  }
}
