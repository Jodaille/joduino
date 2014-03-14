/**
* Raspberry as I2C MASTER (3.3V)
* send a number via I2C
* Arduino as I2C Slave (5V)
* answer values of DHT11 sensor
*
* 3 Wires:
*
* Arduino - Raspberry
* GND     - GND
* A4      - 2 (front to 5V)
* A5      - 3 (front to GND)
*
*/

#include <Wire.h>
#include <dht11.h>

dht11 DHT11;

#define DHT11PIN 2
#define SLAVE_ADDRESS 0x12

/*
* dataReceived is to_send number in python script
*/
int dataReceived = 0;

// char cannot contain space
char msg[255] = "some_text_without_space";

int index = 0;

void setup() {

    Serial.begin(9600);//115200 9600
    /**
    * change baudrate on RaspberryPi :
    * echo 115200 > /sys/module/i2c_bcm2708/parameters/baudrate
    */
    Serial.println("DHT11 TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT11LIB_VERSION);
    Serial.println();
    delay(2000);
    
    //Serial.println(msg);
}

void loop() {
    // No need delay
    printData();
    delay(2000);
}

void printData(){

    int chk = DHT11.read(DHT11PIN);

    sprintf(msg,"{\"temperature\":\"%i\",\"humidity\":\"%i\",\"msg\":\"%i\"}",DHT11.temperature,DHT11.humidity, dataReceived);

    Serial.println(msg);
}

