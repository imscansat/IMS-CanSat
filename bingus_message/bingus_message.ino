/**
#include <EEPROM.h>
void setup()
{
  Serial.begin(9600); 
  //Set serial baud rate to 9600
}

void loop()
{
  EEPROM.write("bingus");           //print out hello string
  delay(1000);                  //1 second delay
}

https://roboticsbackend.com/arduino-write-string-in-eeprom/
**/

#include <EEPROM.h>

void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);

  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

String readStringFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];

  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\ 0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)

  return String(data);
}

void setup() {
  Serial.begin(9600);

  writeStringToEEPROM(0, "Hello Arduino");
  
  String retrievedString = readStringFromEEPROM(0);

  Serial.print("The String we read from EEPROM: ");
  Serial.println(retrievedString);
}

void loop() {
{
  writeStringToEEPROM(0, "bingus");
  
  String retrievedString = readStringFromEEPROM(0);
  Serial.print("The String we read from EEPROM: ");
  Serial.println(retrievedString);

  delay(1000);                  //1 second delay
}
  }
