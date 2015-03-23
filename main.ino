#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#define readButton A3
#define CS 4
RTC_DS1307 RTC;
char buffer[40];
char recvChar;
const int buttonPin = 2;
int buttonState = 0;
File myFile;
SoftwareSerial Genotronex( 0, 1);
void setup () {
    Serial.begin(9600);
    Genotronex.begin(9600);
    Serial.print("Initializing SD card...");
    pinMode(CS, OUTPUT);
    pinMode(buttonPin, INPUT); 
    pinMode(readButton, INPUT);
    if (!SD.begin(CS)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");
    Wire.begin();
    RTC.begin();

    if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    else{
      Serial.println("RTC is runnig");
    }
}
void loop () {
    DateTime now = RTC.now();
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      Serial.println("HIGH");
       myFile = SD.open("data3.txt", FILE_READ);
       if (myFile) {
       while (myFile.available()) {
        Genotronex.write(myFile.read());
        delay(1000);
        Genotronex.println("Bluetooth On please press 1 or 0 blink LED ..");
        delay(1000);
        Serial.println(myFile.read());
        delay(1000);
      }
      myFile.close();
      }else {
        Serial.println("error opening test.txt");
      }
    }
    else  {
      Serial.println("LOW");
      DateTime now = RTC.now();
      sprintf(buffer,  "%02d/%d/%d %d:%02d:%02d %d %d ", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second(), analogRead(A0),   analogRead(A1)) ;
      Serial.println( buffer );
      myFile = SD.open("data3.txt", FILE_WRITE);
      if (myFile) {
        myFile.println(buffer );
        myFile.close();
      } else {
        Serial.println("error opening data3.txt");
      }
      delay(1000);
    }
}
    

