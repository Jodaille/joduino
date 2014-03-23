/*
 * WebServerMultiPageHTMLProgmem sketch
 * 
 *
 * Respond to requests in the URL to change digital and analog output ports
 * show the number of ports changed and the value of the analog input pins.
 *
 * http://192.168.1.177/analog/   displays analog pin data
 * http://192.168.1.177/digital/  displays digital pin data
 * http://192.168.1.177/change/  allows changing digital pin data
 *
 */

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>

#include <Wire.h>
#include <dht11.h>

dht11 DHT11;
int pinPhotoresistor = A0;
const int pinFAN = 8;
int pinFOAM = 7;

char msg[100] = "{\"temperature\":\"\",\"humidity\":\"\",\"msg\":\"start\"}";

#define DHT11PIN 2
#define SLAVE_ADDRESS 0x12
int dataReceived = 0;
int index = 0;
int t;
int h;
int l;
int m;
int chk;
    
#include <avr/pgmspace.h> // for progmem
#define P(name)   static const prog_uchar name[] PROGMEM  // declare a static string

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] =  { 192,168,0,177 };

const int MAX_PAGENAME_LEN = 8;  // max characters in page name 
char buffer[MAX_PAGENAME_LEN+1]; // additional character for terminating null

EthernetServer server(80);
EthernetClient client;

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  delay(1000);
  Serial.println(F("Ready"));

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop()
{

  client = server.available();
  if (client) {
    int type = 0;
    while (client.connected()) {

      if (client.available()) {
        // GET, POST, or HEAD
        memset(buffer,0, sizeof(buffer)); // clear the buffer
        if(client.readBytesUntil('/', buffer,MAX_PAGENAME_LEN)){ 
          if(strcmp(buffer, "GET ") == 0 )
            type = 1;
          else if(strcmp(buffer,"POST ") == 0)
            type = 2;
          // look for the page name
          memset(buffer,0, sizeof(buffer)); // clear the buffer
          if(client.readBytesUntil( '/', buffer,MAX_PAGENAME_LEN )) 
          {
            if(strcasecmp(buffer, "analog") == 0)
              showAnalog();
            else if(strcasecmp(buffer, "digital") == 0)
              showDigital();
            else if(strcmp(buffer, "change")== 0)
              showChange(type == 2);
            else if(strcmp(buffer, "json")== 0)
              showJson("json");
            else if(strcmp(buffer, "fanon")== 0)
              showJson("fanon");
            else if(strcmp(buffer, "fanoff")== 0)
              showJson("fanoff");
            else if(strcmp(buffer, "foamon")== 0)
              showJson("foamon");
            else if(strcmp(buffer, "foamoff")== 0)
              showJson("foamoff");
            else if(strcmp(buffer, "alloff")== 0)
              showJson("alloff");
            else if(strcmp(buffer, "allon")== 0)
              showJson("alloff");
            else
              unknownPage(buffer);
          }
        }
        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  
  }else {
      chk = DHT11.read(DHT11PIN);
  delay(1000);
  l = analogRead(pinPhotoresistor);
  t = int(DHT11.temperature);
  h = int(DHT11.humidity);
  sprintf(msg,"{\"l\":\"%i\",\"temperature\":\"%i\",\"humidity\":\"%i\",\"msg\":\"\",\"action\":\"\"}",l,t,h);
Serial.println(msg);
  }
}


void showJson(char * action)
{
  Serial.println(action);

    chk = DHT11.read(DHT11PIN);
    l = analogRead(pinPhotoresistor);

if(strcasecmp(action, "fanon") == 0)
{
          pinMode(pinFAN, OUTPUT);
          digitalWrite(pinFAN, HIGH);
          action = "fanhigh";
}
else if(strcmp(buffer, "fanoff")== 0)
{
          pinMode(pinFAN, OUTPUT);
          digitalWrite(pinFAN, LOW);
          action = "fanlow";
}
else if(strcasecmp(action, "foamon") == 0)
{
          pinMode(pinFOAM, OUTPUT);
          digitalWrite(pinFOAM, HIGH);
          action = "foamhigh";
}
else if(strcmp(buffer, "foamoff")== 0)
{
          pinMode(pinFOAM, OUTPUT);
          digitalWrite(pinFOAM, LOW);
          action = "foamlow";
}
else if(strcmp(buffer, "alloff")== 0)
{
          pinMode(pinFOAM, OUTPUT);
          digitalWrite(pinFOAM, LOW);
          pinMode(pinFAN, OUTPUT);
          digitalWrite(pinFAN, LOW);
          action = "alloff";
}
else if(strcmp(buffer, "allon")== 0)
{
          pinMode(pinFOAM, OUTPUT);
          digitalWrite(pinFOAM, HIGH);
          pinMode(pinFAN, OUTPUT);
          digitalWrite(pinFAN, HIGH);
          action = "allon";
}
else
{

  action = "default";
}
          


  switch (chk)
  {
    case DHTLIB_OK: 
                t = int(DHT11.temperature);
                h = int(DHT11.humidity);
                m = 2;
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		m=99;
                chk = DHT11.read(DHT11PIN);
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		m=98; 
		break;
    default: 
		m=97;
		break;
  }    
  sprintf(msg,"{\"l\":\"%i\",\"temperature\":\"%i\",\"humidity\":\"%i\",\"msg\":\"%i\",\"action\":\"%s\"}",l,t,h, m,action);
  Serial.println(msg);
  sendJson(msg);
  //client.print(msg);
}
void showAnalog()
{
    int t;
    int h;
    int l;
    int m = 1;
    int chk;
  
  chk = DHT11.read(DHT11PIN);
  l = analogRead(pinPhotoresistor);
  switch (chk)
  {
    case DHTLIB_OK: 
                t = int(DHT11.temperature);
                h = int(DHT11.humidity);
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		m=99;
                chk = DHT11.read(DHT11PIN);
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		m=98; 
		break;
    default: 
		m=97;
		break;
  }
  
  Serial.println(F("analog"));
  sendHeader("Joduino ethernet show analog");
  client.println("<h1>Analog Pins</h1>");

  // output the value of each analog input pin

    client.println(F("<table border='1' >"));
  for (int i = 0; i < 6; i++) {
    client.print(F("<tr><td>analog pin "));
    client.print(i);
    client.print(F(" </td><td>"));
    client.print(analogRead(i));
    client.println(F("</td></tr>"));
  }
  client.println(F("</table>"));
  client.println(F("</body></html>"));
}

// mime encoded data for the led on and off images:
// see: http://www.motobit.com/util/base64-decoder-encoder.asp
P(led_on) =  "<img src=\"data:image/jpg;base64,"
"/9j/4AAQSkZJRgABAgAAZABkAAD/7AARRHVja3kAAQAEAAAAHgAA/+4ADkFkb2JlAGTAAAAAAf/b"
"AIQAEAsLCwwLEAwMEBcPDQ8XGxQQEBQbHxcXFxcXHx4XGhoaGhceHiMlJyUjHi8vMzMvL0BAQEBA"
"QEBAQEBAQEBAQAERDw8RExEVEhIVFBEUERQaFBYWFBomGhocGhomMCMeHh4eIzArLicnJy4rNTUw"
"MDU1QEA/QEBAQEBAQEBAQEBA/8AAEQgAGwAZAwEiAAIRAQMRAf/EAIIAAAICAwAAAAAAAAAAAAAA"
"AAUGAAcCAwQBAAMBAAAAAAAAAAAAAAAAAAACBAUQAAECBAQBCgcAAAAAAAAAAAECAwARMRIhQQQF"
"UWFxkaHRMoITUwYiQnKSIxQ1EQAAAwYEBwAAAAAAAAAAAAAAARECEgMTBBQhQWEiMVGBMkJiJP/a"
"AAwDAQACEQMRAD8AcNz3BGibKie0nhC0v3A+teKJt8JmZEdHuZalOitgUoHnEpQEWtSyLqgACWFI"
"nixWiaQhsUFFBiQSbiMvvrmeCBp27eLnG7lFTDxs+Kra8oOyium3ltJUAcDIy4EUMN/7Dnq9cPMO"
"W90E9kxeyF2d3HFOQ175olKudUm7TqlfKqDQEDOFR1sNqtC7k5ERYjndNPFSArtvnI/nV+ed9coI"
"ktd2BgozrSZO3J5jVEXRcwD2bbXNdq0zT+BohTyjgPp5SYdPJZ9NP2jsiIz7vhjLohtjnqJ/ouPK"
"co//2Q=="
"\"/>";

P(led_off) = "<img src=\"data:image/jpg;base64,"
"/9j/4AAQSkZJRgABAgAAZABkAAD/7AARRHVja3kAAQAEAAAAHgAA/+4ADkFkb2JlAGTAAAAAAf/b"
"AIQAEAsLCwwLEAwMEBcPDQ8XGxQQEBQbHxcXFxcXHx4XGhoaGhceHiMlJyUjHi8vMzMvL0BAQEBA"
"QEBAQEBAQEBAQAERDw8RExEVEhIVFBEUERQaFBYWFBomGhocGhomMCMeHh4eIzArLicnJy4rNTUw"
"MDU1QEA/QEBAQEBAQEBAQEBA/8AAEQgAHAAZAwEiAAIRAQMRAf/EAHgAAQEAAwAAAAAAAAAAAAAA"
"AAYFAgQHAQEBAQAAAAAAAAAAAAAAAAACAQQQAAECBQAHBQkAAAAAAAAAAAECAwAREhMEITFhoSIF"
"FUFR0UIGgZHBMlIjM1MWEQABAwQDAQEAAAAAAAAAAAABABECIWESA1ETIyIE/9oADAMBAAIRAxEA"
"PwBvl5SWEkkylpJMGsj1XjXSE1kCQuJ8Iy9W5DoxradFa6VDf8IJZAQ6loNtBooTJaqp3DP5oBlV"
"nWrTpEouQS/Cf4PO0uKbqWHGXTSlztSvuVFiZjmfLH3GUuMkzSoTMu8aiNsXet5/17hFyo6PR64V"
"ZnuqfqDDDySFpNpYH3E6aFjzGBr2DkMuFBSFDsWkilUdLftW13pWpcdWqnbBzI/l6hVXKZlROUSe"
"L1KX5zvAPXESjdHsTFWpxLKOJ54hIA1DZCj+Vx/3r96fCNrkvRaT0+V3zV/llplr9sVeHZui/ONk"
"H3dzt6cL/9k="
"\"/>";
;
void receiveData(int byteCount){

    while(Wire.available()) {
        dataReceived = Wire.read();
    }
       m = int(dataReceived);

      if(m==11)
      {
        pinMode(pinFAN, OUTPUT);
        digitalWrite(pinFAN, HIGH);

        //m=15;
      }
      if(m==12)
      {
        pinMode(pinFAN, OUTPUT);
        digitalWrite(pinFAN, LOW);
        //m=15;
      }
      if(m==14)
      {
        pinMode(pinFOAM, OUTPUT);
        digitalWrite(pinFOAM, HIGH);
        //m=15;
      }
      if(m==15)
      {
        pinMode(pinFOAM, OUTPUT);
        digitalWrite(pinFOAM, LOW);
        //m=15;
      }
     //sprintf(msg,"{\"l\":\"%i\",\"temperature\":\"%i\",\"humidity\":\"%i\",\"msg\":\"%i\"}",l,t,h, m);
     Serial.println("receiveData ");
}

void sendData(){

    Wire.write(msg[index]);
    ++index;
    if (index >= 100) {
         index = 0;
    }
}

void showDigital()
{
  Serial.println(F("digital"));
  sendHeader("Joduino ethernet show digital");
  client.println(F("<h2>Digital Pins</h2>"));
  // show the value of digital pins
  client.println(F("<table border='1'>"));
  for (int i = 2; i < 8; i++) {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH); // turn on pull-ups
    client.print(F("<tr><td>digital pin "));
    client.print(i);
    client.print(F(" </td><td>"));
    if(digitalRead(i) == LOW)
      printP(led_off);
    else
      printP(led_on);
    client.println(F("</td></tr>"));
  }
  client.println(F("</table>"));

  client.println(F("</body></html>"));
}


void showChange(boolean isPost)
{
  Serial.println(F("change"));
  if(isPost)
  {
    Serial.println("isPost");
    client.find("\n\r"); // skip to the body
    // find parameters starting with "pin" and stop on the first blank line
    Serial.println(F("searching for parms"));
    while(client.findUntil("pinD", "\n\r")){
      int pin = client.parseInt();       // the pin number
      int val = client.parseInt();       // 0 or 1
      Serial.print(pin);
      Serial.print("=");
      Serial.println(val);
      pinMode(pin, OUTPUT);
      digitalWrite(pin, val);
    }
  }
  sendHeader("Joduino ethernet");
  // table with buttons from 2 through 9
  // 2 to 5 are inputs, the other buttons are outputs

  client.println(F("<table border='1'>"));

  // show the input pins
  for (int i = 2; i < 6; i++) {  // pins 2-5 are inputs
    pinMode(i, INPUT);
    digitalWrite(i, HIGH); // turn on pull-ups
    client.print(F("<tr><td>digital input "));
    client.print(i);
    client.print(F(" </td><td>"));

    client.print(F("&nbsp </td><td>"));
    client.print(F(" </td><td>"));
    client.print(F("&nbsp </td><td>"));


    if(digitalRead(i) == LOW)
      //client.print("Low");
      printP(led_off);
    else
      //client.print("high");
      printP(led_on);
    client.println("</td></tr>");
  }

  // show output pins 6-9
  // note pins 10-13 are used by the ethernet shield 
  for (int i = 6; i < 10; i++) {
    client.print(F("<tr><td>digital output "));
    client.print(i);
    client.print(F(" </td><td>"));
    htmlButton( "On", "pinD", i, "1");
    client.print(F(" </td><td>"));
    client.print(F(" </td><td>"));
    htmlButton("Off", "pinD", i, "0");
    client.print(F(" </td><td>"));

    if(digitalRead(i) == LOW)
      //client.print("Low");
      printP(led_off);
    else
      //client.print("high");
      printP(led_on);
    client.println(F("</td></tr>"));
  }
  client.println(F("</table>"));
}

// create an HTML button
void htmlButton( char * label, char *name, int nameId, char *value)
{
  client.print(F("<form action='/change/' method='POST'><p><input type='hidden' name='"));
  client.print(name);
  client.print(nameId);
  client.print(F("' value='"));
  client.print(value);
  client.print(F("'><input type='submit' value='"));
  client.print(label);
  client.print(F("'/></form>"));
}

void unknownPage(char *page)
{
  Serial.print(F("Unknown : "));
  Serial.println(F("page"));
  Serial.println(page);

  sendHeader("Unknown Page");
  client.println(F("<h1>Unknown Page</h1>"));
  client.println(page);
  client.println(F("</body></html>"));
}

void sendJson(char *json)
{
  // send a standard http response header
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println();
  client.println(json);

}

void sendHeader(char *title)
{
  // send a standard http response header
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println();
  client.print(F("<html><head><title>"));
  client.println(title);
  client.println(F("</title><body>"));
  client.println(F("<a href='/change/'>Change</a>"));
  client.println(F("<a href='/analog/'>Analog</a>"));
  client.println(F("<a href='/digital/'>Digital</a>"));
  client.println(F("<a href='/json/'>Json view</a>"));
}

void printP(const prog_uchar *str)
{
  // copy data out of program memory into local storage, write out in
  // chunks of 32 bytes to avoid extra short TCP/IP packets
  // from webduino library Copyright 2009 Ben Combee, Ran Talbott
  uint8_t buffer[32];
  size_t bufferEnd = 0;

  while (buffer[bufferEnd++] = pgm_read_byte(str++))
  {
    if (bufferEnd == 32)
    {
      client.write(buffer, 32);
      bufferEnd = 0;
    }
  }

  // write out everything left but trailing NUL
  if (bufferEnd > 1)
    client.write(buffer, bufferEnd - 1);
}

