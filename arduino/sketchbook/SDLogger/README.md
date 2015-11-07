! JoBee Beehive monitor

based on tutorial from Tom Igoe:
https://www.arduino.cc/en/Tutorial/Datalogger

The circuit:

* RTC clock
** SDA - pin A4
** SCL - pin A5

* DS18B20 digital temperature sensors on digital 2
** data - pin D2
** one resistor 10k between data and vcc

* SD card attached to SPI bus as follows:
** MOSI - pin D11
** MISO - pin D12
** CLK  - pin D13
** CS   - pin D4


You need to install this libraries in your sketchbook libraries folder:
* https://github.com/adafruit/RTClib
* https://github.com/milesburton/Arduino-Temperature-Control-Library
