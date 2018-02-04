#undef max
#undef min

#include "TestWin32Arduino.h"
#include <string>
#include "arduino.h"
#include "demo.h"

namespace arduino { namespace test
{

  //--------------------------------------------------------------------------------------------------
  void TestWin32Arduino::SetUp()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void TestWin32Arduino::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testDigitalPinString)
  {
    std::string sHIGH = arduino_stub::toDigitalPinString(HIGH);
    std::string sLOW = arduino_stub::toDigitalPinString(LOW);
    std::string sOTHER = arduino_stub::toDigitalPinString(234);

    ASSERT_EQ("HIGH", sHIGH);
    ASSERT_EQ("LOW", sLOW);
    ASSERT_TRUE(sHIGH != sOTHER && sLOW != sOTHER);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testBitOrderString)
  {
    std::string sMSBFIRST = arduino_stub::toBitOrderString(MSBFIRST);
    std::string sLSBFIRST = arduino_stub::toBitOrderString(LSBFIRST);
    std::string sOTHER = arduino_stub::toBitOrderString(234);

    ASSERT_EQ("MSBFIRST", sMSBFIRST);
    ASSERT_EQ("LSBFIRST", sLSBFIRST);
    ASSERT_TRUE(sMSBFIRST != sOTHER && sLSBFIRST != sOTHER);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testPinModeString)
  {
    std::string sOUTPUT = arduino_stub::toPinModeString(OUTPUT);
    std::string sINPUT = arduino_stub::toPinModeString(INPUT);
    std::string sINPUT_PULLUP = arduino_stub::toPinModeString(INPUT_PULLUP);
    std::string sOTHER = arduino_stub::toPinModeString(234);

    ASSERT_EQ("OUTPUT", sOUTPUT);
    ASSERT_EQ("INPUT", sINPUT);
    ASSERT_EQ("INPUT_PULLUP", sINPUT_PULLUP);
    ASSERT_TRUE(sOUTPUT != sOTHER && sINPUT != sOTHER && sINPUT_PULLUP != sOTHER);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMicrosRealtime)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_REALTIME);
    std::string lastCall;

    uint32_t value1 = micros();
    delay(10);
    uint32_t value2 = micros();

    ASSERT_GT(value2, value1);

    uint32_t elapsedMicros = value2-value1;
    ASSERT_NEAR(10000, elapsedMicros, 5000); //no usec precision, only milliseconds. Allows 5ms diff
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMillisRealtime)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_REALTIME);
    std::string lastCall;

    uint32_t value1 = millis();
    delay(30);
    uint32_t value2 = millis();

    ASSERT_GT(value2, value1);

    uint32_t elapsedMillis = value2-value1;
    ASSERT_NEAR(30, elapsedMillis, 4); //4ms epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMicrosSimulation)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_SIMULATION);
    std::string lastCall;

    uint32_t value1 = micros();
    delay(10);
    uint32_t value2 = micros();

    ASSERT_GT(value2, value1);

    uint32_t elapsedMicros = value2-value1;
    ASSERT_NEAR(10000, elapsedMicros, 10); //10 usec max epsilon (emulated)
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMillisSimulation)
  {
    arduino_stub::setClockStrategy(arduino_stub::CLOCK_SIMULATION);
    std::string lastCall;

    uint32_t value1 = millis();
    delay(30);
    uint32_t value2 = millis();

    ASSERT_GT(value2, value1);

    uint32_t elapsedMillis = value2-value1;
    ASSERT_EQ(30, elapsedMillis); //0ms epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMath)
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
  TEST_F(TestWin32Arduino, testInterruptModeString)
  {
    std::string sCHANGE = arduino_stub::toInterruptModeString(CHANGE);
    std::string sRISING = arduino_stub::toInterruptModeString(RISING);
    std::string sFALLING = arduino_stub::toInterruptModeString(FALLING);
    std::string sOTHER = arduino_stub::toInterruptModeString(234);

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
  TEST_F(TestWin32Arduino, testSetMicrosResolution)
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
  TEST_F(TestWin32Arduino, testSetMicrosCounter)
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
  TEST_F(TestWin32Arduino, testSerial)
  {
    Serial.print("Hello World from ");
    Serial.print(__FUNCTION__);
    Serial.println("() function!");
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testDemos)
  {
    demoSerial();
    demoMillis();
    demoLog();
  }
  //--------------------------------------------------------------------------------------------------
} // End namespace test
} // End namespace arduino
