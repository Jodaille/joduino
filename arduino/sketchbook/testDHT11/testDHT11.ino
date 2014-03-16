/**
* Arduino echo values of DHT11 sensor
* allow to test baudrate and DHT11 values
* before use of I2C communication
* @author Jodaille
* @licence GPLv3
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

	Serial.begin(9600);
	Serial.println("DHT11 TEST PROGRAM ");
	Serial.print("LIBRARY VERSION: ");
	Serial.println(DHT11LIB_VERSION);
	Serial.println();
	
	// give time to init sensor
	delay(2000); // 2s may be a bit long

}

void loop() {

    printData();
    delay(2000);
}

/**
* Substitute datas values to msg variable
* and then print msg
*/
void printData(){

    int chk = DHT11.read(DHT11PIN);

    sprintf(msg,"{\"temperature\":\"%i\",\"humidity\":\"%i\",\"msg\":\"%i\"}",DHT11.temperature,DHT11.humidity, dataReceived);

    Serial.println(msg);
}

