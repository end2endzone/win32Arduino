#include "TestDemo.h"
#include <string>
#include "Arduino.h"
#include "IncrementalClockStrategy.h"

namespace arduino { namespace test
{
  using namespace testarduino;

  //assume an arduino library with the following function:
  bool waitForButtonPress(uint8_t buttonPin, uint32_t timeout) {
    unsigned long start = millis();
    while( millis() - start < timeout )
    {
      //look for button state
      int buttonValue = digitalRead(buttonPin);
      if (buttonValue == HIGH)
        return true;
    }
    //timeout
    return false;
  }

  //--------------------------------------------------------------------------------------------------
  void TestDemo::SetUp()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void TestDemo::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void simulatePinHighISR() {
    setPinDigitalValue(2, HIGH);
  }
  TEST(TestDemo, testWaitForButtonPressTimeout) {
    reset();
    IncrementalClockStrategy & clock = IncrementalClockStrategy::getInstance();
    clock.setMicrosecondsCounter(0);
    clock.setMicrosecondsResolution(100); //increase simulated clock by 0.1ms for every calls to micros()
    static const uint8_t buttonPin = 2;
    static const uint32_t MAX_WAIT_TIME = 5000; //ms
    
    //assert that false is returned if button is not pressed
    uint32_t time1 = millis();
    bool result = waitForButtonPress(buttonPin, MAX_WAIT_TIME);
    uint32_t time2 = millis();
    uint32_t elapsed = time2 - time1;
    ASSERT_FALSE(result);
    ASSERT_EQ(MAX_WAIT_TIME, elapsed);

    //configure win32Arduino library properly.
    static const uint32_t BUTTON_DELAY_TIME = 2000; //ms
    uint32_t buttonPressTime = millis() + BUTTON_DELAY_TIME; //in 2000 ms, the button pin will go HIGH
    attachMillisecondsCallback(buttonPressTime, simulatePinHighISR);

    //run the function again...
    //assert that function is interrupted when a button is pressed
    time1 = millis();
    result = waitForButtonPress(buttonPin, MAX_WAIT_TIME);
    time2 = millis();
    elapsed = time2 - time1;
    ASSERT_TRUE(result);
    ASSERT_EQ(BUTTON_DELAY_TIME, elapsed);
  }
  //--------------------------------------------------------------------------------------------------
} // End namespace test
} // End namespace arduino
