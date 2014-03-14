joduino
=======

Environment monitor:

- [x] Read DHT11 temperature and humidity sensor
- [x] Communication via I2C (RaspberryPI ask Arduino to send data)
- [x] Arduino send data to I2C master
- [ ] Master should be able to send info to Arduino (DOWN or HIGH)
- [ ] Send data to a web server
- [ ] Web server could store data
- [ ] Web server could display graphics
- [ ] Web server could send an alert (eMail, SMS ...)
- [ ] Receive via Web interface order to switch relay


# Why I2C communication

- Arduino can easily read sensors and output to serial

- Arduino can easily answer to a string in console monitor
  but I cannot easily ask Arduino via serial (in python script for example)
  to print values to serial outside of arduino console monitor prompt.

- Raspberry is not as understantable and easy usable as Arduino (pins, voltages)

- Arduino as limited distant access and network access capability

- Connecting Raspberry and Arduino with I2C need only three wires
  if Raspberry is master and Arduino slave (3.3V => 5V)

- We manage to send an int from Raspberry 
  with simple python script via I2C,
  Arduino is able to send us back sensors datas \o/

# What to care about when testing communication ?

## Raspberry must be master on I2C connection

Raspberry has 3.3V voltage without particular protection,
Arduino has 5V one with protections.

We could damage Raspberry if Arduino is master, because
it could send 5V to Raspberry input.

## Easy connect with three wires and without other electronic parts
- same ground (GND)
- two wires :

|Arduino | Raspberry |
|:--------:|:-----------:|
|A4      | 2 (front to 5V)|
|A5      | 3 (front to GND)|

## Same communication baudrate on both side
```bash
# specify baudrate 9600 on Raspberry
echo 9600 > /sys/module/i2c_bcm2708/parameters/baudrate
```
Note that 115200 seems to make DHT11 a bit mad

## DHT11 needs some time to initialize itself
if yours data are not relevant, for example humidity over 100 
(max is 100 percent) or temperature a bit low (11°C indoor)
The try to add delay before reading:
```C
delay(500);
Serial.println(DHT11.temperature);
```

## Send char via I2C was not trivial

I did not know what a char was because I usually use PHP.

Arduino is able to send a char with temperature and humidity.


