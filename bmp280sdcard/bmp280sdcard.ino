/*
SD card datalogger

logging real time from RTC and  data from sensors to an SD card using the SD library.

programmed by Wolfgang Lex based on some example programs for BMP280 with Adafruit_BMP280.h
and example program for data logging shield from MAKERFACORY / Velleman  

*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10
 
Adafruit_BMP280 bme; // I2C

const int chipSelect = 10;
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
float temp1, temp2, press1, press2, diffp, difft;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  delay(3000); // wait for console opening
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

if (! rtc.begin()) {
   Serial.println("Couldn't find RTC");
   while (1);
}
// rtc.adjust(DateTime(2019, 12, 11, 10, 39, 10));
 if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
   rtc.adjust(DateTime(2019, 12, 11, 9, 48, 30));
  }
  if (!bme.begin(0x76)) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
}

void loop() {
  DateTime now = rtc.now();

// make a string for assembling the data to log:
    String dataString = "";
   
    dataString = dataString + now.day()+ ('.')+ now.month()+ ('.')+ now.year() + (',') + now.hour() + (':') + now.minute() + (':') + now.second()+ (',');
    delay(5000);
    
    temp1=bme.readTemperature();  // read out now actual temperature from BMP280
    
   press1=bme.readPressure()/100;  // read out now actual air pressure from BMP280 and divide by 100 in order to get hPa = mbar out of original value in Pascal

// compare old value 5 seconds ago with new value and decide, if value has changed significantly = more than 2% 
   diffp=press1-press2;
   difft=temp1-temp2;
   
    if ((abs(difft)>temp2/50) or (abs(diffp)>press2/50))  { 
      temp2=temp1;      // save the old temperature value, measured at last measurement 
      press2=press1;    // save the old pressure value 
      dataString = dataString + temp1;
      dataString += ",";
      dataString = dataString + press1;

      // open the file for logging of data
      File dataFile = SD.open("logdata.txt", FILE_WRITE);

    // if the file is available, write to it:
      if (dataFile) {
                    dataFile.println(dataString);
                    dataFile.close();
                    // print to the serial port too:
                    Serial.println(dataString);
                    }
      // if the file isn't open, pop up an error:
      else {
           Serial.println("error opening datalog.txt");
           }
    }
}
