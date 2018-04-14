#include "TestGetLastCommand.h"
#include <string>
#include "arduino.h"
#include "demo.h"

namespace arduino { namespace test
{

  //--------------------------------------------------------------------------------------------------
  void TestGetLastCommand::SetUp()
  {
    testarduino::setLogFile("arduino.log");
  }
  //--------------------------------------------------------------------------------------------------
  void TestGetLastCommand::TearDown()
  {
    testarduino::setLogFile("");
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testPinMode)
  {
    pinMode(13, OUTPUT);      // sets the digital pin 13 as output
    std::string pinOutput = testarduino::getLastCommand();
    ASSERT_EQ("pinMode(13,OUTPUT);\n", pinOutput);

    pinMode(13, INPUT);      // sets the digital pin 7 as input
    std::string pinInput = testarduino::getLastCommand();
    ASSERT_EQ("pinMode(13,INPUT);\n", pinInput);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testDigitalWrite)
  {
    std::string lastCall;

    digitalWrite(13, HIGH);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("digitalWrite(13,HIGH);\n", lastCall);

    digitalWrite(13, LOW);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("digitalWrite(13,LOW);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testAnalogWrite)
  {
    std::string lastCall;

    analogWrite(13, 56);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("analogWrite(13,56);\n", lastCall);

    analogWrite(13, 255);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("analogWrite(13,255);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testDigitalRead)
  {
    std::string lastCall;
    uint8_t value;

    value = digitalRead(13);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("digitalRead(13);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testAnalogRead)
  {
    std::string lastCall;
    uint16_t value;

    value = analogRead(13);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("analogRead(13);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testAnalogReadResolution)
  {
    std::string lastCall;

    analogReadResolution(8);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("analogReadResolution(8);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testAnalogWriteResolution)
  {
    std::string lastCall;

    analogWriteResolution(7);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("analogWriteResolution(7);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testShiftOut)
  {
    std::string lastCall;

    shiftOut(3, 4, MSBFIRST, 78);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("shiftOut(3,4,MSBFIRST,78);\n", lastCall);

    shiftOut(5, 6, LSBFIRST, 99);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("shiftOut(5,6,LSBFIRST,99);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testShiftIn)
  {
    std::string lastCall;
    uint8_t value;

    value = shiftIn(3, 4, MSBFIRST);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("shiftIn(3,4,MSBFIRST);\n", lastCall);

    value = shiftIn(5, 6, LSBFIRST);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("shiftIn(5,6,LSBFIRST);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testPulseIn)
  {
    std::string lastCall;
    uint32_t value;

    value = pulseIn(3, HIGH);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("pulseIn(3,HIGH);\n", lastCall);

    value = pulseIn(4, LOW);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("pulseIn(4,LOW);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testMicros)
  {
    std::string lastCall;

    micros();
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("micros();\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testMillisRealtime)
  {
    std::string lastCall;

    millis();
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("millis();\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  void myInterruptFunction()
  {
  }
  TEST_F(TestGetLastCommand, testAttachInterrupt)
  {
    std::string lastCall;

    attachInterrupt(2, &myInterruptFunction, RISING);
    lastCall = testarduino::getLastCommand();
    ASSERT_NE(std::string::npos, lastCall.find("attachInterrupt(2,"));
    ASSERT_NE(std::string::npos, lastCall.find(",RISING);"));
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testDetachInterrupt)
  {
    std::string lastCall;

    detachInterrupt(3);
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("detachInterrupt(3);\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testNoInterrupts)
  {
    std::string lastCall;

    noInterrupts();
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("noInterrupts();\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestGetLastCommand, testInterrupts)
  {
    std::string lastCall;

    interrupts();
    lastCall = testarduino::getLastCommand();
    ASSERT_EQ("interrupts();\n", lastCall);
  }
  //--------------------------------------------------------------------------------------------------
} // End namespace test
} // End namespace arduino
