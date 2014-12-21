# Arduino sketchbook

Sketches are a work in progress. 
We try to cut some working pieces of code in order to be able to assemble them later.


## Time with arduino

We need to keep and know time, we use a tiny RTC: 

[TinyRtc] is a piece of code that allow us to check that we can access clock.

Then we decide to read temperature and humidity values from a DHT11 sensor at each interval of [fiveMinutes] 

But we certainly need to check that sensors can be accessed, so that [DHT11] could help us. 

## Experiments not very stable

[arduinoI2c] was a try to make Arduino and RasberryPi via I2C.

[PagesServer] use PageServer example of Arduino, we try to build a tiny webserver that could response a json string...

This two case seems to run out of memory, hard to manage char and string in/out :)

## Libraries directory for sketches

[libraries] are in sketchbook directory, you should not use this one but try the last version of it on their respective project.
These version are known to work with the examples here.

[TinyRtc]:https://github.com/Jodaille/joduino/tree/master/arduino/sketchbook/TinyRtc
[fiveMinutes]:https://github.com/Jodaille/joduino/tree/master/arduino/sketchbook/fiveMinutes
[DHT11]:https://github.com/Jodaille/joduino/tree/master/arduino/sketchbook/testDHT11
[PagesServer]:https://github.com/Jodaille/joduino/blob/master/arduino/sketchbook/PagesServer/PagesServer.ino
[arduinoI2c]:https://github.com/Jodaille/joduino/tree/master/arduino/sketchbook/arduinoI2c
[libraries]:https://github.com/Jodaille/joduino/tree/master/arduino/sketchbook/libraries