#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>


// BMP280 //
Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
// //

int eeAddress = 0; //EEPROM address to start reading from

float temp = 0.00;
float pres = 0.00;

void setup() { // put your setup code here, to run once:

  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // BMP280 //
  Serial.println(F("BMP280 Sensor event test"));


  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  bmp_temp->printSensorDetails();
  // //



}
void loop() { // put your main code here, to run repeatedly:
   for (int index = 0 ; index < EEPROM.length() ; index+=2) {
      sensors_event_t temp_event, pressure_event;
      temp = bmp_temp->getEvent(&temp_event);
      pres = bmp_pressure->getEvent(&pressure_event);
      
      EEPROM.write(eeAddress, temp);
      Serial.print(F("Temperature = "));
      EEPROM.get(eeAddress, temp);
      eeAddress += 1;
      EEPROM.write(eeAddress, pres);
      Serial.print(F("Pressure = "));
      EEPROM.get(eeAddress, pres);
      eeAddress += 1;

      /*Serial.print(F("Temperature = "));
      Serial.print(temp_event.temperature);
      Serial.println(" *C");

      Serial.print(F("Pressure = "));
      Serial.print(pressure_event.pressure);
      Serial.println(" hPa");

      Serial.println();*/

      delay(1000);
  }
  
  
  /*
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

  Serial.println();
  delay(2000);
  */
}
