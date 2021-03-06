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
int t;
int h;
int l;
int m;
int chk;

int pinPhotoresistor = A0;
int pinYellow = 8;
int pinRed = 7;

// char cannot contain space
char msg[100] = "{\"l\":\"\",\"temperature\":\"\",\"humidity\":\"\",\"msg\":\"start\"}";
char debug[100] = "{\"l\":\"\",\"temperature\":\"\",\"humidity\":\"\",\"msg\":\"start\"}";
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

    pinMode(pinYellow, OUTPUT);
    pinMode(pinRed, OUTPUT);


    //Serial.println(msg);
}

void loop() {

  chk = DHT11.read(DHT11PIN);
  l = analogRead(pinPhotoresistor);
  t = 0;
  h = 0;
  m = 1;
  switch (chk)
  {
    case DHTLIB_OK: 
                t = int(DHT11.temperature);
                h = int(DHT11.humidity);
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		m=99;
                //chk = DHT11.read(DHT11PIN);
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		m=98; 
		break;
    default: 
		m=97;
		break;
  }
  //sprintf(debug,"{\"l\":\"%i\",\"temperature\":\"%i\",\"humidity\":\"%i\",\"msg\":\"%i\"}",l,t,h,m);
  //Serial.println(debug);
  delay(1000);
}

void receiveData(int byteCount){

    while(Wire.available()) {
        dataReceived = Wire.read();
        m = int(dataReceived);
    }
     

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

     sprintf(msg,"{\"l\":\"%i\",\"temperature\":\"%i\",\"humidity\":\"%i\",\"msg\":\"%i\"}",l,t,h, m);
     //Serial.println(msg);
}

void sendData(){

    Wire.write(msg[index]);
    ++index;
    if (index >= 100) {
         index = 0;
    }
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
