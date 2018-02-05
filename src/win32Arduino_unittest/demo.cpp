// demo.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include "windows.h"
#undef min
#undef max

#include "arduino.h"

void demoSerial()
{
  uint16_t test = 0xFFFF;
  Serial.print(test);
  Serial.println();
}

void demoMillis()
{
  Serial.println();
  Serial.println("looping for 40ms...");

  for(int i=0; i<4; i++)
  {
    uint32_t start = millis();
    Sleep(40);
    uint32_t end = millis();
    uint32_t elapsed = end-start;
    Serial.print("loop #");
    Serial.print(i+1);
    Serial.print(" elapsed=");
    Serial.print(elapsed);
    Serial.println(" (expecting 40 ms)");
  }
}

void demoLog()
{
  testarduino::setLogFile("foo.log");
  Serial.println("this is line 1 of 3 in foo.txt");
  Serial.println("this is line 2 of 3 in foo.txt");
  Serial.println("this is line 3 of 3 in foo.txt");
  testarduino::setLogFile("bar.log");
  Serial.println("this is line 1 of 4 in bar.txt");
  Serial.println("this is line 2 of 4 in bar.txt");
  Serial.println("this is line 3 of 4 in bar.txt");
  Serial.println("this is line 4 of 4 in bar.txt");
}
