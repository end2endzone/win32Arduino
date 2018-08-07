#undef max
#undef min

#include "TestButtonLibrary.h"
#include <string>
#include "arduino.h"
#include "IncrementalClockStrategy.h"
#include "ButtonLibrary.h"

using namespace testarduino;

namespace arduino { namespace test
{
  //--------------------------------------------------------------------------------------------------
  void TestButtonLibrary::SetUp()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void TestButtonLibrary::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void simulatePinLowISR() {
    testarduino::setPinDigitalValue(2, LOW);
  }
  TEST(TestButtonLibrary, testWaitForButtonPressTimeout) {
    testarduino::reset();
    IncrementalClockStrategy & clock = IncrementalClockStrategy::getInstance();
    clock.setMicrosecondsCounter(0);
    clock.setMicrosecondsResolution(100); //increase simulated clock by 0.1ms for every calls to micros()
    static const uint8_t buttonPin = 2;
    static const uint32_t MAX_WAIT_TIME = 5000; //ms
    testarduino::setPinDigitalValue(buttonPin, HIGH); //simulate pin pull-up resistor

    //assert that false is returned if button is not pressed
    uint32_t time1 = millis();
    bool result = waitForButtonPress(buttonPin, MAX_WAIT_TIME);
    uint32_t time2 = millis();
    uint32_t elapsed = time2 - time1;
    ASSERT_FALSE(result);
    ASSERT_EQ(MAX_WAIT_TIME, elapsed);

    //configure win32Arduino library to push a button in 2000 ms.
    static const uint32_t BUTTON_DELAY_TIME = 2000; //ms
    uint32_t buttonPressTime = millis() + BUTTON_DELAY_TIME;
    attachMillisecondsCallback(buttonPressTime, simulatePinLowISR); //in 2000 ms, the button pin will go HIGH

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
