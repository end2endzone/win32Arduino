//
//  win32Arduino Library - v1.0 - 05/13/2016
//  Copyright (C) 2016 Antoine Beauchamp
//  The code & updates for the library can be found on http://end2endzone.com
//
// AUTHOR/LICENSE:
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License (LGPL-3.0) for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// DISCLAIMER:
//  This software is furnished "as is", without technical support, and with no 
//  warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
//  The win32Arduino is a win32 library that implementation of most used arduino
//  functions which allows a library developer to unit test his code outside of
//  the arduino platform.
//
//  This library allows a windows user to easily test an arduino library using
//  your testing framework of choice. For instance, the unit tests of win32Arduino
//  library are executed using the Google Test framework.
//
// USAGE:
//  The following instructions show how to easily test an arduino library using
//  the Google Test framework. It assumes that you are already familiar with the
//  test API.
//
//  1. Create an executable project and configure the main() function to launch
//     Google Test's RUN_ALL_TESTS() macro.
//  2. Create a second static library project and add all the arduino files of
//     the desired library you need to test.
//  3. The library files are expected to include arduino.h. Modify the project's
//     Additionnal Include Directories to point to the win32Arduino library.
//
//  The project should compile properly without errors or unresolved extensions
//  allowing you to properly unit test each functions.
//
// HISTORY:
// 05/13/2016 v1.0 - Initial release.
//

#undef max
#undef min

#include "Tests.h"
#include <string>
#include "arduino.h"
#include "demo.h"

namespace arduino { namespace test
{

  //--------------------------------------------------------------------------------------------------
  void Tests::SetUp()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void Tests::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testDigitalPinString)
  {
    std::string sHIGH = toDigitalPinString(HIGH);
    std::string sLOW = toDigitalPinString(LOW);
    std::string sOTHER = toDigitalPinString(234);

    ASSERT_EQ("HIGH", sHIGH);
    ASSERT_EQ("LOW", sLOW);
    ASSERT_TRUE(sHIGH != sOTHER && sLOW != sOTHER);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testBitOrderString)
  {
    std::string sMSBFIRST = toBitOrderString(MSBFIRST);
    std::string sLSBFIRST = toBitOrderString(LSBFIRST);
    std::string sOTHER = toBitOrderString(234);

    ASSERT_EQ("MSBFIRST", sMSBFIRST);
    ASSERT_EQ("LSBFIRST", sLSBFIRST);
    ASSERT_TRUE(sMSBFIRST != sOTHER && sLSBFIRST != sOTHER);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testPinModeString)
  {
    std::string sOUTPUT = toPinModeString(OUTPUT);
    std::string sINPUT = toPinModeString(INPUT);
    std::string sINPUT_PULLUP = toPinModeString(INPUT_PULLUP);
    std::string sOTHER = toPinModeString(234);

    ASSERT_EQ("OUTPUT", sOUTPUT);
    ASSERT_EQ("INPUT", sINPUT);
    ASSERT_EQ("INPUT_PULLUP", sINPUT_PULLUP);
    ASSERT_TRUE(sOUTPUT != sOTHER && sINPUT != sOTHER && sINPUT_PULLUP != sOTHER);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testPinMode)
  {
    pinMode(13, OUTPUT);      // sets the digital pin 13 as output
    std::string pinOutput = arduino_stub::getLastCommand();
    ASSERT_EQ("pinMode(13, OUTPUT);\n", pinOutput);

    pinMode(13, INPUT);      // sets the digital pin 7 as input
    std::string pinInput = arduino_stub::getLastCommand();
    ASSERT_EQ("pinMode(13, INPUT);\n", pinInput);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testDigitalWrite)
  {
    std::string lastCall;

    digitalWrite(13, HIGH);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("digitalWrite(13, HIGH);\n", lastCall);

    digitalWrite(13, LOW);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("digitalWrite(13, LOW);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testAnalogWrite)
  {
    std::string lastCall;

    analogWrite(13, 56);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("analogWrite(13, 56);\n", lastCall);

    analogWrite(13, 255);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("analogWrite(13, 255);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testDigitalRead)
  {
    std::string lastCall;
    uint8_t value;

    value = digitalRead(13);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("digitalRead(13);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testAnalogRead)
  {
    std::string lastCall;
    uint16_t value;

    value = analogRead(13);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("analogRead(13);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testAnalogReadResolution)
  {
    std::string lastCall;

    analogReadResolution(8);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("analogReadResolution(8);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testAnalogWriteResolution)
  {
    std::string lastCall;

    analogWriteResolution(7);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("analogWriteResolution(7);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testShiftOut)
  {
    std::string lastCall;

    shiftOut(3, 4, MSBFIRST, 78);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("shiftOut(3, 4, MSBFIRST, 78);\n", lastCall);

    shiftOut(5, 6, LSBFIRST, 99);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("shiftOut(5, 6, LSBFIRST, 99);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testShiftIn)
  {
    std::string lastCall;
    uint8_t value;

    value = shiftIn(3, 4, MSBFIRST);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("shiftIn(3, 4, MSBFIRST);\n", lastCall);

    value = shiftIn(5, 6, LSBFIRST);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("shiftIn(5, 6, LSBFIRST);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testPulseIn)
  {
    std::string lastCall;
    uint32_t value;

    value = pulseIn(3, HIGH);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("pulseIn(3, HIGH);\n", lastCall);

    value = pulseIn(4, LOW);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("pulseIn(4, LOW);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testMicrosRealtime)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_REALTIME);
    std::string lastCall;

    uint32_t value1 = micros();
    delay(10);
    uint32_t value2 = micros();

    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("micros();\n", lastCall);

    ASSERT_GT(value2, value1);

    uint32_t elapsedMicros = value2-value1;
    ASSERT_NEAR(10000, elapsedMicros, 5000); //no usec precision, only milliseconds. Allows 5ms diff
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testMillisRealtime)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_REALTIME);
    std::string lastCall;

    uint32_t value1 = millis();
    delay(30);
    uint32_t value2 = millis();

    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("millis();\n", lastCall);

    ASSERT_GT(value2, value1);

    uint32_t elapsedMillis = value2-value1;
    ASSERT_NEAR(30, elapsedMillis, 4); //4ms epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testMicrosSimulation)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_SIMULATION);
    std::string lastCall;

    uint32_t value1 = micros();
    delay(10);
    uint32_t value2 = micros();

    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("micros();\n", lastCall);

    ASSERT_GT(value2, value1);

    uint32_t elapsedMicros = value2-value1;
    ASSERT_NEAR(10000, elapsedMicros, 10); //10 usec max epsilon (emulated)
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testMillisSimulation)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_SIMULATION);
    std::string lastCall;

    uint32_t value1 = millis();
    delay(30);
    uint32_t value2 = millis();

    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("millis();\n", lastCall);

    ASSERT_GT(value2, value1);

    uint32_t elapsedMillis = value2-value1;
    ASSERT_EQ(30, elapsedMillis); //0ms epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testMath)
  {
    std::string lastCall;

    ASSERT_EQ(5, abs(5));
    ASSERT_EQ(5, abs(-5));

    ASSERT_EQ(8, max(5, 8));
    ASSERT_EQ(-5, max(-5, -7));

    ASSERT_EQ(5, min(5, 8));
    ASSERT_EQ(-7, min(-5, -7));

    ASSERT_EQ(5, constrain(3, 5, 8));
    ASSERT_EQ(6, constrain(6, 5, 8));
    ASSERT_EQ(8, constrain(9, 5, 8));

    ASSERT_EQ(10, map(1, 0, 10, 0, 100));
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testInterruptModeString)
  {
    std::string sCHANGE = toInterruptModeString(CHANGE);
    std::string sRISING = toInterruptModeString(RISING);
    std::string sFALLING = toInterruptModeString(FALLING);
    std::string sOTHER = toInterruptModeString(234);

    ASSERT_EQ("CHANGE", sCHANGE);
    ASSERT_EQ("RISING", sRISING);
    ASSERT_EQ("FALLING", sFALLING);

    ASSERT_TRUE(sCHANGE   != sOTHER &&
                sRISING   != sOTHER &&
                sFALLING  != sOTHER );
  }
  //--------------------------------------------------------------------------------------------------
  void myInterruptFunction()
  {
  }
  TEST_F(Tests, testAttachInterrupt)
  {
    std::string lastCall;

    attachInterrupt(2, &myInterruptFunction, RISING);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_NE(std::string::npos, lastCall.find("attachInterrupt(2, "));
    ASSERT_NE(std::string::npos, lastCall.find(", RISING);"));
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testDetachInterrupt)
  {
    std::string lastCall;

    detachInterrupt(3);
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("detachInterrupt(3);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testNoInterrupts)
  {
    std::string lastCall;

    noInterrupts();
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("noInterrupts();\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testInterrupts)
  {
    std::string lastCall;

    interrupts();
    lastCall = arduino_stub::getLastCommand();
    ASSERT_EQ("interrupts();\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testRandom)
  {
    int16_t seed = 0x5656;
    randomSeed(seed);

    bool found[15] = {0};
    for(int i=0; i<100; i++)
    {
      int32_t value = random(5, 10);
      found[value] = true;
    }

    ASSERT_EQ(false, found[0]);
    ASSERT_EQ(false, found[1]);
    ASSERT_EQ(false, found[2]);
    ASSERT_EQ(false, found[3]);
    ASSERT_EQ(false, found[4]);
    ASSERT_EQ( true, found[5]);
    ASSERT_EQ( true, found[6]);
    ASSERT_EQ( true, found[7]);
    ASSERT_EQ( true, found[8]);
    ASSERT_EQ( true, found[9]);
    ASSERT_EQ(false, found[10]);
    ASSERT_EQ(false, found[11]);
    ASSERT_EQ(false, found[12]);
    ASSERT_EQ(false, found[13]);
    ASSERT_EQ(false, found[14]);

    memset(&found, 0, sizeof(found));
    for(int i=0; i<100; i++)
    {
      int32_t value = random(5);
      found[value] = true;
    }

    ASSERT_EQ(true, found[0]);
    ASSERT_EQ(true, found[1]);
    ASSERT_EQ(true, found[2]);
    ASSERT_EQ(true, found[3]);
    ASSERT_EQ(true, found[4]);
    ASSERT_EQ(false, found[5]);
    ASSERT_EQ(false, found[6]);
    ASSERT_EQ(false, found[7]);
    ASSERT_EQ(false, found[8]);
    ASSERT_EQ(false, found[9]);
    ASSERT_EQ(false, found[10]);
    ASSERT_EQ(false, found[11]);
    ASSERT_EQ(false, found[12]);
    ASSERT_EQ(false, found[13]);
    ASSERT_EQ(false, found[14]);

  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testBitsAndBytes)
  {
    uint32_t value = 0xC2000044;

    ASSERT_EQ(0x44, lowByte(value));
    ASSERT_EQ(0xC2, highByte(value));

    //assert bitRead(), 0x44 low byte
    ASSERT_EQ(0, bitRead(value, (uint32_t)0));
    ASSERT_EQ(0, bitRead(value, (uint32_t)1));
    ASSERT_EQ(1, bitRead(value, (uint32_t)2));
    ASSERT_EQ(0, bitRead(value, (uint32_t)3));
    ASSERT_EQ(0, bitRead(value, (uint32_t)4));
    ASSERT_EQ(0, bitRead(value, (uint32_t)5));
    ASSERT_EQ(1, bitRead(value, (uint32_t)6));
    ASSERT_EQ(0, bitRead(value, (uint32_t)7));

    //assert bitRead(), 0xC2 high byte
    ASSERT_EQ(0, bitRead(value, (uint32_t)24));
    ASSERT_EQ(1, bitRead(value, (uint32_t)25));
    ASSERT_EQ(0, bitRead(value, (uint32_t)26));
    ASSERT_EQ(0, bitRead(value, (uint32_t)27));
    ASSERT_EQ(0, bitRead(value, (uint32_t)28));
    ASSERT_EQ(0, bitRead(value, (uint32_t)29));
    ASSERT_EQ(1, bitRead(value, (uint32_t)30));
    ASSERT_EQ(1, bitRead(value, (uint32_t)31));

    //assert bitWrite(), high byte
    ASSERT_EQ(0xC2000044, value);
    bitWrite(value, (uint32_t)31, (uint32_t)0);
    ASSERT_EQ(0x42000044, value);
    bitWrite(value, (uint32_t)30, (uint32_t)0);
    ASSERT_EQ(0x02000044, value);
    bitWrite(value, (uint32_t)31, (uint32_t)1);
    ASSERT_EQ(0x82000044, value);
    bitWrite(value, (uint32_t)30, (uint32_t)1);
    ASSERT_EQ(0xC2000044, value);

    //assert bitSet() is the same as bitWrite(x, n, 1)
    {
      uint32_t valueSet = 0;
      uint32_t valueWrite = 0;
      for(uint32_t i=0; i<=31; i++)
      {
        bitWrite(valueWrite, i, (uint32_t)1);
        bitSet(valueSet, i);
        ASSERT_EQ(valueWrite, valueSet);
      }
    }

    //assert bitClear() is the same as bitWrite(x, n, 0)
    {
      uint32_t valueSet = 0xFFFFFFFF;
      uint32_t valueWrite = 0xFFFFFFFF;
      for(uint32_t i=0; i<=31; i++)
      {
        bitWrite(valueWrite, i, (uint32_t)0);
        bitClear(valueSet, i);
        ASSERT_EQ(valueWrite, valueSet);
      }
    }

    //assert bit()
    ASSERT_EQ(    1, bit(0));
    ASSERT_EQ(    2, bit(1));
    ASSERT_EQ(    4, bit(2));
    ASSERT_EQ(    8, bit(3));
    ASSERT_EQ(   16, bit(4));
    ASSERT_EQ(   32, bit(5));
    ASSERT_EQ(   64, bit(6));
    ASSERT_EQ(  128, bit(7));
    ASSERT_EQ(  256, bit(8));
    ASSERT_EQ(  512, bit(9));
    ASSERT_EQ( 1024, bit(10));
    ASSERT_EQ( 2048, bit(11));
    ASSERT_EQ( 4096, bit(12));
    ASSERT_EQ( 8192, bit(13));
    ASSERT_EQ(16384, bit(14));
    ASSERT_EQ(32768, bit(15));
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testSetMicrosResolution)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_SIMULATION);
    arduino_stub::setMicrosResolution(1);

    uint32_t a = micros();
    uint32_t b = micros();

    EXPECT_NEAR(a, b, 1);

    //back to default
    arduino_stub::setMicrosResolution(8);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testSetMicrosCounter)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_SIMULATION);
    arduino_stub::setMicrosResolution(1);
    uint32_t before = micros();
    arduino_stub::setMicrosCounter(256);

    uint32_t a = micros();

    EXPECT_NEAR(a, 256, 1);

    //back to previous value
    arduino_stub::setMicrosCounter(before);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testStatusRegister)
  {
    uint8_t before = SREG;
    
    noInterrupts();
    uint8_t a = SREG;
    interrupts();
    uint8_t b = SREG;
    
    EXPECT_NE(a,b);

    //back to default
    SREG = before;
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testSerial)
  {
    Serial.print("Hello World from ");
    Serial.print(__FUNCTION__);
    Serial.println("() function!");
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(Tests, testDemos)
  {
    demoSerial();
    demoMillis();
    demoLog();
  }
  //--------------------------------------------------------------------------------------------------
} // End namespace test
} // End namespace arduino
