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
int pinYellow = 8;
int pinRed = 7;

// char cannot contain space
char msg[255] = "{\"temperature\":\"\",\"humidity\":\"\",\"msg\":\"start\"}";

int index = 0;

void setup() {

    Serial.begin(9600);//9600
    /**
    * change baudrate on RaspberryPi :
    * echo 9600 > /sys/module/i2c_bcm2708/parameters/baudrate
    */

    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
    Serial.println("DHT11 TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT11LIB_VERSION);
    Serial.println();
    delay(2000);
    int chk = DHT11.read(DHT11PIN);

    pinMode(pinYellow, OUTPUT);
    pinMode(pinRed, OUTPUT);

    Serial.println(msg);
}

void loop() {
    // No need delay

}

void receiveData(int byteCount){

    while(Wire.available()) {
        dataReceived = Wire.read();
    }
    delay(500);
 
      int m = int(dataReceived);

      if(m==11)
      {
        digitalWrite(pinRed, HIGH);
        //m=15;
      }
      if(m==12)
      {
        digitalWrite(pinRed, LOW);
        //m=15;
      }
      if(m==14)
      {
        digitalWrite(pinYellow, HIGH);
        //m=15;
      }
      if(m==15)
      {
        digitalWrite(pinYellow, LOW);
        //m=15;
      }
     sprintf(msg,"{\"temperature\":\"%i\",\"humidity\":\"%i\",\"msg\":\"%i\"}",DHT11.temperature,DHT11.humidity, m);

    Serial.println(msg);
}

void sendData(){

    Wire.write(msg[index]);
    ++index;
    if (index >= 255) {
         index = 0;
    }
}
void printData(){

    int chk = DHT11.read(DHT11PIN);

    sprintf(msg,"{\"temperature\":\"%i\",\"humidity\":\"%i\",\"msg\":\"%i\"}",DHT11.temperature,DHT11.humidity, dataReceived);

    Serial.println(msg);
}

/** python script below:
#
# send_212_to_arduino_via_i2c.py
#
import smbus
import time

# Replace 1 by 0 for old Raspberry (without hole)
bus = smbus.SMBus(1)

arduinomessage = ""

arduino_slave_address = 0x12

to_send = 212

# print "Sending number: ", to_send, " to arduino"

bus.write_byte(arduino_slave_address, to_send)

# Waitng a bit for response
time.sleep(1)

for i in range(0, 255):
	arduinomessage += chr(bus.read_byte(arduino_slave_address));

print arduinomessage
</code>

*/
