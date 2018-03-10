#undef max
#undef min

#include "TestWin32Arduino.h"
#include <string>
#include "arduino.h"
#include "RealtimeClockStrategy.h"
#include "IncrementalClockStrategy.h"
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

    uint32_t value1 = micros();
    delay(1000);
    uint32_t value2 = micros();

    ASSERT_GT(value2, value1);

    uint32_t elapsedMicros = value2-value1;
    ASSERT_NEAR(1000000, elapsedMicros, 10000); //no usec precision, only milliseconds. Allows 10ms epsilon
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestWin32Arduino, testMillisRealtime)
  {
    testarduino::setClockStrategy(&testarduino::RealtimeClockStrategy::getInstance());

    uint32_t value1 = millis();
    delay(1000);
    uint32_t value2 = millis();

    ASSERT_GT(value2, value1);

    uint32_t elapsedMillis = value2-value1;
    ASSERT_NEAR(1000, elapsedMillis, 10); //10ms epsilon
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
  TEST_F(TestWin32Arduino, testClockDiff)
  {
    clock_t timeStart = 200;
    clock_t timeEnd   = 201;
    double diff = testarduino::RealtimeClockStrategy::clockDiff(timeEnd, timeStart);
    EXPECT_NEAR(diff, 1.0, 0.000001);
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

    for(int pin=0; pin<=255; pin++)
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
    for(int pin=0; pin<=255; pin++)
    {
      testarduino::setPinAnalogValue(pin, 111);
    }
    testarduino::reset();
    for(int pin=0; pin<=255; pin++)
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
  static int gTestFunctionCallbacks_MILLIS = 0;
  void testFunctionCallbacks_MILLIS()
  {
    gTestFunctionCallbacks_MILLIS++;
  }
  TEST_F(TestWin32Arduino, testFunctionCallbacks)
  {
    gTestFunctionCallbacks_MILLIS = 0;
 
    testarduino::attachPreFunctionCallback("millis", testFunctionCallbacks_MILLIS);
    ASSERT_EQ(0, gTestFunctionCallbacks_MILLIS);
    uint32_t time1 = millis();
    ASSERT_EQ(1, gTestFunctionCallbacks_MILLIS);
    uint32_t time2 = millis();
    ASSERT_EQ(2, gTestFunctionCallbacks_MILLIS);
    testarduino::detachFunctionCallback("millis");
    uint32_t time3 = millis();
    ASSERT_EQ(2, gTestFunctionCallbacks_MILLIS);
  }
  //--------------------------------------------------------------------------------------------------
} // End namespace test
} // End namespace arduino
