/* Clock
* cf:
* https://learn.adafruit.com/ds1307-real-time-clock-breakout-board-kit/understanding-the-code
*/
#include <Wire.h>
#include "RTClib.h" 
RTC_DS1307 RTC;

// Ultrasonic sensor
#include <NewPing.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// DHT11 sensor
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN A0

/* Low level detection
cf: http://bateaux.trucs.free.fr/detecteur_eau_sans_composant.html
*/
const int lowlevel = A3;

int lightPin = A2;

// Warning led
const int warning = 13;

// read interval management
long previousMillis = 0;        // will store last time data was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {

    Serial.begin(9600);

    Wire.begin();
    RTC.begin();

    if (! RTC.isrunning()) {
        Serial.println("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        //RTC.adjust(DateTime(__DATE__, __TIME__));
    }

    // low water level
    pinMode(lowlevel, INPUT); 
    
}
 
void loop() {

    unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis > interval) {
        // save the last time we read data 
        previousMillis = currentMillis;

        int light = analogRead(lightPin);

        // Sonar distance
        int uS = sonar.ping();
        
        // Time
        DateTime now = RTC.now();
        
        // Temperature and humidity
        int chk = DHT11.read(DHT11PIN);
        
        // Display result
        Serial.print("{'date':'");
        printDigits(now.year());
        Serial.print('-');
        printDigits(now.month());
        Serial.print('-');
        printDigits(now.day());
        Serial.print('T');
        printDigits(now.hour());
        Serial.print(':');
        printDigits(now.minute());
        Serial.print(':');
        printDigits(now.second());
        Serial.print("',");
        Serial.print("'waterlevel':");
        Serial.print(uS / US_ROUNDTRIP_CM);
        Serial.print(",");

        Serial.print("'light':");
        Serial.print(light);
        Serial.print(",");

        Serial.print("'humidity':'");
        Serial.print((float)DHT11.humidity, 2);
        Serial.print("',");

        Serial.print("'temperature':");
        Serial.print((float)DHT11.temperature, 2);
        
        // Low level alert 
        Serial.print("'alarm':");
        digitalWrite(lowlevel, HIGH); // enable internal pull-up
        if (digitalRead(lowlevel))
        {
            //Serial.println("!!! MISSING WATER !!!");
            Serial.print("'low water level'");
            digitalWrite(warning, HIGH);
        }
        else
        {
            Serial.print("''");
            digitalWrite(warning, LOW);
        }
        digitalWrite(lowlevel, LOW); // disable internal pull-up

        Serial.println("}");        
        
    }
}

void printDigits(byte digits){
  // utility function for digital clock display: prints colon and leading 0
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits,DEC);   
}
