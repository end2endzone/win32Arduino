/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#include "TestWin32Arduino.h"
#include <string>
#include "Arduino.h"
#include "util/atomic.h" //for ATOMIC_BLOCK and NONATOMIC_BLOCK macros
#include "RealtimeClockStrategy.h"
#include "IncrementalClockStrategy.h"
#include "rapidassist/testing.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/timing.h"

namespace arduino { namespace test
{
  uint8_t SREG_INTERRUPTS_ENABLED;
  uint8_t SREG_INTERRUPTS_DISABLED;

  //--------------------------------------------------------------------------------------------------
  void TestWin32Arduino::SetUp()
  {
    //update SREG_INTERRUPTS_ENABLED and SREG_INTERRUPTS_DISABLED
    noInterrupts();
    SREG_INTERRUPTS_DISABLED = SREG;
    interrupts();
    SREG_INTERRUPTS_ENABLED = SREG;
  }
  //--------------------------------------------------------------------------------------------------
  void TestWin32Arduino::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testDigitalPinString)
  {
    std::string sHIGH = testarduino::toDigitalPinString(HIGH);
    std::string sLOW = testarduino::toDigitalPinString(LOW);
    std::string sOTHER = testarduino::toDigitalPinString(234);

    ASSERT_EQ("HIGH", sHIGH);
    ASSERT_EQ("LOW", sLOW);
    ASSERT_TRUE(sHIGH != sOTHER && sLOW != sOTHER);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testBitOrderString)
  {
    std::string sMSBFIRST = testarduino::toBitOrderString(MSBFIRST);
    std::string sLSBFIRST = testarduino::toBitOrderString(LSBFIRST);
    std::string sOTHER = testarduino::toBitOrderString(234);

    ASSERT_EQ("MSBFIRST", sMSBFIRST);
    ASSERT_EQ("LSBFIRST", sLSBFIRST);
    ASSERT_TRUE(sMSBFIRST != sOTHER && sLSBFIRST != sOTHER);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testPinModeString)
  {
    std::string sOUTPUT = testarduino::toPinModeString(OUTPUT);
    std::string sINPUT = testarduino::toPinModeString(INPUT);
    std::string sINPUT_PULLUP = testarduino::toPinModeString(INPUT_PULLUP);
    std::string sOTHER = testarduino::toPinModeString(234);

    ASSERT_EQ("OUTPUT", sOUTPUT);
    ASSERT_EQ("INPUT", sINPUT);
    ASSERT_EQ("INPUT_PULLUP", sINPUT_PULLUP);
    ASSERT_TRUE(sOUTPUT != sOTHER && sINPUT != sOTHER && sINPUT_PULLUP != sOTHER);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMicrosRealtime)
  {
    testarduino::setClockStrategy(&testarduino::RealtimeClockStrategy::getInstance());

    static const uint32_t EXPECTED_ELAPSED_MILLIS = 1000;
    static const uint32_t EXPECTED_ELAPSED_MICROS = EXPECTED_ELAPSED_MILLIS*1000;

    ra::timing::WaitNextSecond(); //synchronize to a new second in wall clock

    uint32_t value1 = micros();
    ra::timing::WaitNextSecond(); //loop for 1 second
    uint32_t value2 = micros();

    ASSERT_GT(value2, value1);

    uint32_t actualElapsedMicros = value2-value1;
    ASSERT_NEAR(EXPECTED_ELAPSED_MICROS, actualElapsedMicros, 15*1000); //Allows 15ms epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMillisRealtime)
  {
    testarduino::setClockStrategy(&testarduino::RealtimeClockStrategy::getInstance());

    static const uint32_t EXPECTED_ELAPSED_MILLIS = 1000; //1 second

    ra::timing::WaitNextSecond(); //synchronize to a new second in wall clock

    uint32_t value1 = millis();
    ra::timing::WaitNextSecond(); //loop for 1 second
    uint32_t value2 = millis();

    ASSERT_GT(value2, value1);

    uint32_t actualElapsedMillis = value2-value1;
    ASSERT_NEAR(EXPECTED_ELAPSED_MILLIS, actualElapsedMillis, 15); //Allows 15ms epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMicrosSimulation)
  {
    testarduino::IncrementalClockStrategy & clock = testarduino::IncrementalClockStrategy::getInstance();
    testarduino::setClockStrategy(&clock);
    clock.setMicrosecondsResolution(1);
    clock.setMicrosecondsCounter(9999); //next delay() call rounds to 1ms

    uint32_t value1 = micros();
    delay(10); //~10*1000 usec
    uint32_t value2 = micros(); // 1 usec

    ASSERT_GT(value2, value1);

    uint32_t elapsedMicros = value2-value1;
    ASSERT_EQ(10001, elapsedMicros); //0 usec epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMillisSimulation) // test delay() function
  {
    testarduino::IncrementalClockStrategy & clock = testarduino::IncrementalClockStrategy::getInstance();
    testarduino::setClockStrategy(&clock);
    clock.setMicrosecondsResolution(100); //0.1ms increments
    clock.setMicrosecondsCounter(9999); //next delay() call rounds to 1ms

    uint32_t value1 = millis();
    delay(30); // 30ms
    uint32_t value2 = millis(); // 1ms

    ASSERT_GT(value2, value1);

    uint32_t elapsedMillis = value2-value1;
    ASSERT_EQ(30, elapsedMillis); //0ms epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMath)
  {
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
  TEST_F(TestWin32Arduino, testInterruptModeString)
  {
    std::string sCHANGE = testarduino::toInterruptModeString(CHANGE);
    std::string sRISING = testarduino::toInterruptModeString(RISING);
    std::string sFALLING = testarduino::toInterruptModeString(FALLING);
    std::string sOTHER = testarduino::toInterruptModeString(234);

    ASSERT_EQ("CHANGE", sCHANGE);
    ASSERT_EQ("RISING", sRISING);
    ASSERT_EQ("FALLING", sFALLING);

    ASSERT_TRUE(sCHANGE   != sOTHER &&
                sRISING   != sOTHER &&
                sFALLING  != sOTHER );
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testRandom)
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
  TEST_F(TestWin32Arduino, testBitsAndBytes)
  {
    {
    uint16_t value = 0xC244;

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
    ASSERT_EQ(0, bitRead(value, (uint32_t)8));
    ASSERT_EQ(1, bitRead(value, (uint32_t)9));
    ASSERT_EQ(0, bitRead(value, (uint32_t)10));
    ASSERT_EQ(0, bitRead(value, (uint32_t)11));
    ASSERT_EQ(0, bitRead(value, (uint32_t)12));
    ASSERT_EQ(0, bitRead(value, (uint32_t)13));
    ASSERT_EQ(1, bitRead(value, (uint32_t)14));
    ASSERT_EQ(1, bitRead(value, (uint32_t)15));
    }

    //assert bitWrite(), high byte
    uint32_t value = 0xC2000044;
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
  TEST_F(TestWin32Arduino, testSetMicrosResolution)
  {
    testarduino::setClockStrategy(&testarduino::IncrementalClockStrategy::getInstance());
    testarduino::IncrementalClockStrategy::getInstance().setMicrosecondsResolution(1);

    uint32_t a = micros();
    uint32_t b = micros();

    EXPECT_NEAR(a, b, 1);

    //back to default
    testarduino::IncrementalClockStrategy::getInstance().setMicrosecondsResolution(8);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testSetMicrosCounter)
  {
    testarduino::setClockStrategy(&testarduino::IncrementalClockStrategy::getInstance());
    testarduino::IncrementalClockStrategy::getInstance().setMicrosecondsResolution(1);
    uint32_t before = micros();
    testarduino::IncrementalClockStrategy::getInstance().setMicrosecondsCounter(256);

    uint32_t a = micros();

    EXPECT_NEAR(a, 256, 1);

    //back to previous value
    testarduino::IncrementalClockStrategy::getInstance().setMicrosecondsCounter(before);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testStatusRegister)
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
  TEST_F(TestWin32Arduino, demoSerial)
  {
    Serial.print("Hello World from ");
    Serial.print(__FUNCTION__);
    Serial.println("() function!");

    int value = 35;
    Serial.println("int value = 35;");
    Serial.print("Serial.println(value, DEC): ");
    Serial.println(value, DEC);
    Serial.print("Serial.println(value, HEX): ");
    Serial.println(value, HEX);
    Serial.print("Serial.println(value, OCT): ");
    Serial.println(value, OCT);
    Serial.print("Serial.println(value, BIN): ");
    Serial.println(value, BIN);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testSerial)
  {
    //push current log file. Pop when leaving the function.
    struct LOG_FILE_PUSH_POP
    {
      LOG_FILE_PUSH_POP()
      {
        logFile = testarduino::getLogFile();
      }
      ~LOG_FILE_PUSH_POP()
      {
        testarduino::setLogFile(logFile.c_str());
      }
      std::string logFile;
    } __logFilePop;

    //create new log file
    std::string logFile = ra::testing::GetTestQualifiedName();
    if (ra::filesystem::FileExists(logFile.c_str()))
    {
      ASSERT_TRUE( ra::filesystem::DeleteFile(logFile.c_str()) );
    }
    testarduino::setLogFile(logFile.c_str());

    //print to the log file
    Serial.println(78, BIN); // gives "1001110"
    Serial.println(78, OCT); // gives "116"
    Serial.println(78, DEC); // gives "78"
    Serial.println(78, HEX); // gives "4E"
    Serial.println(1.23456, 0); // gives "1"
    Serial.println(1.23456, 2); // gives "1.23"
    Serial.println(1.23456, 4); // gives "1.2346"

    //disable logging at this point
    testarduino::setLogFile("");

    //load log file
    ra::strings::StringVector lines;
    ASSERT_TRUE( ra::filesystem::ReadTextFile(logFile.c_str(), lines) );
    ASSERT_EQ(std::string("1001110"), lines[0]);
    ASSERT_EQ(std::string("116"),     lines[1]);
    ASSERT_EQ(std::string("78"),      lines[2]);
    ASSERT_EQ(std::string("4E"),      lines[3]);
    ASSERT_EQ(std::string("1"),       lines[4]);
    ASSERT_EQ(std::string("1.23"),    lines[5]);
    ASSERT_EQ(std::string("1.2346"),  lines[6]);

    int a = 0;
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testDelayMicroseconds)
  {
    testarduino::IncrementalClockStrategy & clock = testarduino::IncrementalClockStrategy::getInstance();
    testarduino::setClockStrategy(&clock);
    clock.setMicrosecondsResolution(1);
    clock.setMicrosecondsCounter(50);

    uint32_t value1 = micros(); //51 usec
    delayMicroseconds(30); //30 usec
    uint32_t value2 = micros(); // 1 usec

    ASSERT_GT(value2, value1);

    uint32_t elapsedMicros = value2-value1;
    ASSERT_EQ(32, elapsedMicros); //0 usec epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testDelay)
  {
    testarduino::IncrementalClockStrategy & clock = testarduino::IncrementalClockStrategy::getInstance();
    testarduino::setClockStrategy(&clock);
    clock.setMicrosecondsResolution(100); //0.1ms increments
    clock.setMicrosecondsCounter(9999); //next delay() call rounds to 1ms

    uint32_t value1 = millis();
    delay(30); // 30ms
    uint32_t value2 = millis(); // 1ms

    ASSERT_GT(value2, value1);

    uint32_t elapsedMillis = value2-value1;
    ASSERT_EQ(30, elapsedMillis); //0ms epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testPinValues)
  {
    static const uint16_t POWER = 17;
    static const uint16_t MULTIPLICATOR = 31;
    static const uint16_t ADD = 59;

    for(uint8_t pin=0; pin<testarduino::getNumPins(); pin++)
    {
      uint16_t value = POWER*POWER*pin + MULTIPLICATOR*pin + ADD+pin;
      value = value % (1<<10);
      testarduino::setPinAnalogValue(pin, 123);
      ASSERT_EQ(123, testarduino::getPinAnalogValue(pin));
    }

    //test too big values
    static const uint8_t pin = 2;
    testarduino::setPinAnalogValue(pin, 1024);
    ASSERT_EQ(0, testarduino::getPinAnalogValue(pin));
    testarduino::setPinAnalogValue(pin, 1025);
    ASSERT_EQ(1, testarduino::getPinAnalogValue(pin));
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testReset)
  {
    //test pin values
    for(uint8_t pin=0; pin<testarduino::getNumPins(); pin++)
    {
      testarduino::setPinAnalogValue(pin, 111);
    }
    testarduino::reset();
    for(uint8_t pin=0; pin<testarduino::getNumPins(); pin++)
    {
      ASSERT_EQ(0, testarduino::getPinAnalogValue(pin));
    }

    //test interrupts
    //?
  }
  //--------------------------------------------------------------------------------------------------
  static int gTestInterrupts_CHANGE = 0;
  void testInterruptsISR_CHANGE()
  {
    gTestInterrupts_CHANGE++;
  }
  static int gTestInterrupts_FALLING = 0;
  void testInterruptsISR_FALLING()
  {
    gTestInterrupts_FALLING++;
  }
  static int gTestInterrupts_RISING = 0;
  void testInterruptsISR_RISING()
  {
    gTestInterrupts_RISING++;
  }
  TEST_F(TestWin32Arduino, testInterrupts)
  {
    static const uint8_t pin = 2;
 
    //CHANGE
    gTestInterrupts_CHANGE = 0;
    gTestInterrupts_FALLING = 0;
    gTestInterrupts_RISING = 0;
    testarduino::setPinDigitalValue(pin, LOW);
    attachInterrupt(pin, testInterruptsISR_CHANGE, CHANGE);
    testarduino::setPinDigitalValue(pin, HIGH);
    ASSERT_EQ(1, gTestInterrupts_CHANGE);
    ASSERT_EQ(0, gTestInterrupts_FALLING);
    ASSERT_EQ(0, gTestInterrupts_RISING);
 
    detachInterrupt(pin);
    testarduino::setPinDigitalValue(pin, LOW);
    testarduino::setPinDigitalValue(pin, HIGH);
    ASSERT_EQ(1, gTestInterrupts_CHANGE);
    ASSERT_EQ(0, gTestInterrupts_FALLING);
    ASSERT_EQ(0, gTestInterrupts_RISING);
 
 
    //FALLING
    gTestInterrupts_CHANGE = 0;
    gTestInterrupts_FALLING = 0;
    gTestInterrupts_RISING = 0;
    testarduino::setPinDigitalValue(pin, HIGH);
    attachInterrupt(pin, testInterruptsISR_FALLING, FALLING);
    testarduino::setPinDigitalValue(pin, LOW); //from HIGH to LOW
    ASSERT_EQ(0, gTestInterrupts_CHANGE);
    ASSERT_EQ(1, gTestInterrupts_FALLING);
    ASSERT_EQ(0, gTestInterrupts_RISING);
 
    detachInterrupt(pin);
    testarduino::setPinDigitalValue(pin, HIGH);
    testarduino::setPinDigitalValue(pin, LOW);
    ASSERT_EQ(0, gTestInterrupts_CHANGE);
    ASSERT_EQ(1, gTestInterrupts_FALLING);
    ASSERT_EQ(0, gTestInterrupts_RISING);
 
 
    //RISING
    gTestInterrupts_CHANGE = 0;
    gTestInterrupts_FALLING = 0;
    gTestInterrupts_RISING = 0;
    testarduino::setPinDigitalValue(pin, LOW);
    attachInterrupt(pin, testInterruptsISR_RISING, RISING);
    testarduino::setPinDigitalValue(pin, HIGH); //from LOW to HIGH
    ASSERT_EQ(0, gTestInterrupts_CHANGE);
    ASSERT_EQ(0, gTestInterrupts_FALLING);
    ASSERT_EQ(1, gTestInterrupts_RISING);
 
    detachInterrupt(pin);
    testarduino::setPinDigitalValue(pin, LOW);
    testarduino::setPinDigitalValue(pin, HIGH);
    ASSERT_EQ(0, gTestInterrupts_CHANGE);
    ASSERT_EQ(0, gTestInterrupts_FALLING);
    ASSERT_EQ(1, gTestInterrupts_RISING);
  }
  //--------------------------------------------------------------------------------------------------
  static int gTestDetachFunctionCallback_MILLIS = 0;
  void testDetachFunctionCallback_MILLIS()
  {
    gTestDetachFunctionCallback_MILLIS++; //should increase by 1 unit for every call to millis()
  }
  TEST_F(TestWin32Arduino, testDetachFunctionCallback)
  {
    //arrange
    testarduino::reset();
    gTestDetachFunctionCallback_MILLIS = 0;
 
    //act.
    testarduino::attachFunctionCallback("millis", testDetachFunctionCallback_MILLIS);
    ASSERT_EQ(0, gTestDetachFunctionCallback_MILLIS);
    uint32_t time1 = millis();
    ASSERT_EQ(1, gTestDetachFunctionCallback_MILLIS);
    uint32_t time2 = millis();
    ASSERT_EQ(2, gTestDetachFunctionCallback_MILLIS);
 
    //calling the function again should not increase the counter
    testarduino::detachFunctionCallback("millis");
    uint32_t time3 = millis();
    ASSERT_EQ(2, gTestDetachFunctionCallback_MILLIS);
  }
  //--------------------------------------------------------------------------------------------------
  struct TESTATTACHFUNCTIONCALLBACK_STRUCT
  {
    int enterCount;
    int leaveCount;
    uint16_t enterValue;
    uint16_t leaveValue;
    uint8_t pin;
  } gTestAttachFunctionCallback;
  void testAttachFunctionCallback_ENTER()
  {
    gTestAttachFunctionCallback.enterCount++;
    gTestAttachFunctionCallback.enterValue = testarduino::getPinAnalogValue(gTestAttachFunctionCallback.pin);
 
    //force the pin to change value when entering the function
    //this should change the return value of analogRead() to 50
    testarduino::setPinAnalogValue(gTestAttachFunctionCallback.pin, 50);
  }
  void testAttachFunctionCallback_LEAVE()
  {
    gTestAttachFunctionCallback.leaveCount++;
    gTestAttachFunctionCallback.leaveValue = testarduino::getPinAnalogValue(gTestAttachFunctionCallback.pin);
 
    //force the pin to change value when leaving the function
    testarduino::setPinAnalogValue(gTestAttachFunctionCallback.pin, 100);
  }
  TEST_F(TestWin32Arduino, testAttachFunctionCallback)
  {
    //arrange
    gTestAttachFunctionCallback.pin = 2;
    gTestAttachFunctionCallback.enterCount = 0;
    gTestAttachFunctionCallback.leaveCount = 0;
    gTestAttachFunctionCallback.enterValue = 0;
    gTestAttachFunctionCallback.leaveValue = 0;
    testarduino::reset();
   
    //act
    testarduino::attachFunctionCallback("analogRead", testAttachFunctionCallback_ENTER, testAttachFunctionCallback_LEAVE);
    uint16_t returnedValue = analogRead(gTestAttachFunctionCallback.pin);
 
    //assert
    ASSERT_EQ(  1, gTestAttachFunctionCallback.enterCount);
    ASSERT_EQ(  1, gTestAttachFunctionCallback.leaveCount);
    ASSERT_EQ(  0, gTestAttachFunctionCallback.enterValue);
    ASSERT_EQ( 50, returnedValue);
    ASSERT_EQ( 50, gTestAttachFunctionCallback.leaveValue);
    ASSERT_EQ(100, testarduino::getPinAnalogValue(gTestAttachFunctionCallback.pin));
  }
  //--------------------------------------------------------------------------------------------------
  struct TESTATTACHMILLISECONDSCALLBACK_STRUCT
  {
    int count;
    uint32_t callbackTime;
  } gTestAttachMillisecondsCallback;
  void testAttachMillisecondsCallback_ISR()
  {
    gTestAttachMillisecondsCallback.count++;
    gTestAttachMillisecondsCallback.callbackTime = millis();
  }
  TEST_F(TestWin32Arduino, testAttachMillisecondsCallback)
  {
    //arrange
    gTestAttachMillisecondsCallback.count = 0;
    gTestAttachMillisecondsCallback.callbackTime = 0;
    testarduino::reset();
   
    testarduino::IncrementalClockStrategy & clock = testarduino::IncrementalClockStrategy::getInstance();
    testarduino::setClockStrategy(&clock);
    clock.setMicrosecondsCounter(230*1000); //set current time at 230ms
    clock.setMicrosecondsResolution(100);

    //act
    const uint32_t EXPECTEDCALLBACKTIME = 2500;
    testarduino::attachMillisecondsCallback(EXPECTEDCALLBACKTIME, testAttachMillisecondsCallback_ISR);
 
    //wait for 5 seconds
    uint32_t startTime = millis();
    delay(5000);
    uint32_t exitTime = millis();

    //assert
    ASSERT_EQ(1, gTestAttachMillisecondsCallback.count); //assert callback was called
    ASSERT_EQ(exitTime, startTime+5000); //assert loop duration was really 5 seconds
    ASSERT_EQ(EXPECTEDCALLBACKTIME, gTestAttachMillisecondsCallback.callbackTime); //assert actual callback time
  }
  //--------------------------------------------------------------------------------------------------
  struct TESTATTACHMICROSECONDSCALLBACK_STRUCT
  {
    int count;
    uint32_t callbackTime;
  } gTestAttachMicrosecondsCallback;
  void testAttachMicrosecondsCallback_ISR()
  {
    gTestAttachMicrosecondsCallback.count++;
    gTestAttachMicrosecondsCallback.callbackTime = micros();
  }
  TEST_F(TestWin32Arduino, testAttachMicrosecondsCallback)
  {
    //arrange
    gTestAttachMicrosecondsCallback.count = 0;
    gTestAttachMicrosecondsCallback.callbackTime = 0;
    testarduino::reset();    
   
    testarduino::IncrementalClockStrategy & clock = testarduino::IncrementalClockStrategy::getInstance();
    testarduino::setClockStrategy(&clock);
    clock.setMicrosecondsCounter(270); //set current time at 270us
    clock.setMicrosecondsResolution(1);

    //act
    const uint32_t EXPECTEDCALLBACKTIME = 2500;  
    testarduino::attachMicrosecondsCallback(EXPECTEDCALLBACKTIME, testAttachMicrosecondsCallback_ISR);
 
    //wait for 3ms
    uint32_t startTime = micros();
    delayMicroseconds(3000);
    uint32_t exitTime = micros();

    //assert
    ASSERT_EQ(1, gTestAttachMicrosecondsCallback.count); //assert callback was called
    ASSERT_NEAR(exitTime, startTime+3000, 2); //assert loop duration was really 3ms (with 2us epsilon)
    ASSERT_NEAR(EXPECTEDCALLBACKTIME, gTestAttachMicrosecondsCallback.callbackTime, 1); //assert actual callback time (with 1us epsilon)
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testAtomicBlockRestore)
  {
    SREG = 4;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      ASSERT_EQ(SREG_INTERRUPTS_DISABLED, SREG); //assert that interrupts are disabled
    }
    ASSERT_EQ(4, SREG); //assert state restored
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testAtomicBlockForceOn)
  {
    SREG = 4;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
      ASSERT_EQ(SREG_INTERRUPTS_DISABLED, SREG); //assert that interrupts are disabled
    }
    ASSERT_NE(4, SREG); //assert state not restored
    ASSERT_TRUE(SREG_INTERRUPTS_ENABLED == SREG); //assert that interrupts are enabled
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testNonAtomicBlockRestore)
  {
    SREG = 4;
    NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE)
    {
      ASSERT_EQ(SREG_INTERRUPTS_ENABLED, SREG); //assert that interrupts are enabled
    }
    ASSERT_EQ(4, SREG); //assert state restored
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testAtomicBlockForceOff)
  {
    SREG = 4;
    NONATOMIC_BLOCK(NONATOMIC_FORCEOFF)
    {
      ASSERT_EQ(SREG_INTERRUPTS_ENABLED, SREG); //assert that interrupts are enabled
    }
    ASSERT_NE(4, SREG); //assert state not restored
    ASSERT_TRUE(SREG_INTERRUPTS_DISABLED == SREG); //assert that interrupts are disabled
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testPinModeDefault)
  {
    testarduino::reset();
    for(uint8_t i=0; i<testarduino::getNumPins(); i++)
    {
      ASSERT_EQ(INPUT, testarduino::getPinMode(i));
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testPinModeInput)
  {
    testarduino::reset();

    //init
    uint8_t pin = 0;
    ASSERT_EQ(INPUT, testarduino::getPinMode(pin));
    testarduino::setPinDigitalValue(pin, LOW);

    //define as pullup
    pinMode(pin, INPUT);

    //assert that internal pullup resistor is NOT active
    ASSERT_EQ( digitalRead(pin), LOW );

    //simulate a button press (pullup)
    testarduino::setPinDigitalValue(pin, HIGH);
    ASSERT_EQ( digitalRead(pin), HIGH );
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testPinModeInputPullUp)
  {
    testarduino::reset();

    //init
    uint8_t pin = 0;
    ASSERT_EQ(INPUT, testarduino::getPinMode(pin));
    testarduino::setPinDigitalValue(pin, LOW);

    //define as pullup
    pinMode(pin, INPUT_PULLUP);

    //assert that internal pullup resistor is active
    ASSERT_EQ( digitalRead(pin), HIGH );

    //simulate a button press
    testarduino::setPinDigitalValue(pin, 0); //pull-down
    ASSERT_EQ( digitalRead(pin), LOW );
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testPinModeInputPullUpManual)
  {
    testarduino::reset();

    //init
    uint8_t pin = 0;
    ASSERT_EQ(INPUT, testarduino::getPinMode(pin));
    testarduino::setPinDigitalValue(pin, LOW);

    //define as input
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH); //turn on internal pull-up resistor

    //assert that internal pullup resistor is active
    ASSERT_EQ( digitalRead(pin), HIGH );

    //simulate a button press
    testarduino::setPinDigitalValue(pin, 0); //pull-down
    ASSERT_EQ( digitalRead(pin), LOW );
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testAnalogToDigitalConversion)
  {
    testarduino::reset();

    //init
    uint8_t pin = 0;
    pinMode(pin, INPUT);

    //test
    testarduino::setPinAnalogValue(pin, 52); // 52/1023*5.0 = 0.25V
    ASSERT_EQ( LOW, digitalRead(pin) );
    
    //test
    testarduino::setPinAnalogValue(pin, 1000); // 1000/1023*5.0 = 4.88V
    ASSERT_EQ( HIGH, digitalRead(pin) );

    //test 3.3V HIGH as specified by https://www.arduino.cc/reference/en/language/variables/constants/constants/
    testarduino::setPinAnalogValue(pin, 620); // 2.0*1023/3.3 = 620 analog
    ASSERT_EQ( HIGH, digitalRead(pin) );

    //test 5.0V HIGH as specified by https://www.arduino.cc/reference/en/language/variables/constants/constants/
    testarduino::setPinAnalogValue(pin, 613); // 3.0*1023/5.0 = 613.8 analog
    ASSERT_EQ( HIGH, digitalRead(pin) );
    testarduino::setPinAnalogValue(pin, 614);
    ASSERT_EQ( HIGH, digitalRead(pin) );
  }
  //--------------------------------------------------------------------------------------------------
} // End namespace test
} // End namespace arduino
