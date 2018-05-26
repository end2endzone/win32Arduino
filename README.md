![win32Arduino logo](https://github.com/end2endzone/win32Arduino/raw/master/docs/win32Arduino-splashscreen.png)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/win32Arduino.svg)](https://github.com/end2endzone/win32Arduino/releases)
[![AppVeyor Build Status](https://img.shields.io/appveyor/ci/end2endzone/win32Arduino/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/win32Arduino)
[![AppVeyor Tests Status](https://img.shields.io/appveyor/tests/end2endzone/win32Arduino/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master/tests)
[![Travis Build Status](https://img.shields.io/travis/end2endzone/win32Arduino/master.svg?logo=travis&style=flat)](https://travis-ci.org/end2endzone/win32Arduino)

Statistics:

| AppVeyor | Travic CI |
|----------|-----------|
| [![Statistics](https://buildstats.info/appveyor/chart/end2endzone/win32Arduino)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master) | [![Statistics](https://buildstats.info/travisci/chart/end2endzone/win32Arduino)](https://travis-ci.org/end2endzone/win32Arduino) |



# win32Arduino #

win32Arduino is a Windows/Linux implementation of arduino functions. It allows a developer to unit test an arduino library outside of the arduino platform.

The library allows to easily unit test an arduino library using your testing framework of choice. For instance, the unit tests of win32Arduino library are executed using the [Google Test framework](http://github.com/google/googletest).

It's main features are:
*  Implements many arduino functions.
*  Support all functions that reads or writes values to pins.
*  Advanced time handling mechanism. Supports realtime, incremental or custom `millis()` and `micros()` functions to simulate time.
*  Call history and logging support. Calls to the `Serial` class are also forwarded to the output log file.
* Attach function callbacks to any native arduino function.
* Attach function callbacks to `millis()` or `macros()` functions for time based event programming.
* Supports interrupts functions: `attachInterrupt()`, `cli()`, `noInterrupts()`, `interrupts()` and `SREG` registry.

Limitations:
* Pins specific functionality is disabled. In other words, all pins are identical and shall considered supporting analog and digital values.
* AVR, POWER, SERIAL, SPI, I2C is not supported.
* PWM is not supported. Simulating PWM changes over time on each PWM pins is hard.
* Pin resolution functions (`analogReadResolution()` and `analogWriteResolution()`) is not implemented. Pins values is always stored using a resolution of 10 bits.
* Shift functions (`shiftIn()` and `shiftOut()`) are not implemented.
* The following functions are also empty shells: `tone()`, `noTone()` and `pulseIn()`.




# Usage #

The following instructions show how to use the library.




## Testing your own library ##

Arduino library source code must be added to a win32/linux project to be compiled and tested. The following instructions show how to easily test your own library with win32Arduino.

For clarity, unit test are written using the Google Test framework. This section assumes that you are already familiar with the googletest API.



### 1) Convert your library project to cmake ###

This is the recommended way to use win32Arduino. This method assumes that you already defined `GOOGLETEST_HOME` and `WIN32ARDUINO_HOME` environment variables. It also assumes your library source code and your unit tests are located in the same directory.

The following instructions show how to create a unit test executable for your library:

1) Copy the files from directory `/usage/convert` to your existing project.

2) Open a command prompt and navigate to your project.

3) Configure the _Visual Studio solution_ or the _Makefile_ using the following commands:

   * mkdir build
   * cd build
   * cmake ..

4) Build the source code:
   1) On Windows, run `cmake --build . --config Release` or open the solution file with Visual Studio.
   2) On Linux, run `make` command.   



### 2) Create your test project manually with Visual Studio ###

This method assume that you already defined `GOOGLETEST_HOME` and `WIN32ARDUINO_HOME` environment variables.

1) Compile googletest and win32Arduino libraries as specified in [INSTALL.md](INSTALL.md).

2) Open Visual Studio and create a new win32 console application.

3) Modify the project properties to find Google Test and win32Arduino include and library. Add the following values to the project properties:

| Name                             | Value                                                                                                                                      |
|----------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------|
|  Additional Include Directories  | $(GOOGLETEST_HOME)/include;$(WIN32ARDUINO_HOME)/include;                                                                                   |
|  Additional Library Directories  | $(GOOGLETEST_HOME)/build/$(Configuration);$(WIN32ARDUINO_HOME)/build/bin/$(Configuration);$(RAPIDASSIST_HOME)/build/bin/$(Configuration);  |
|  Additional Dependencies         | gtest.lib;win32Arduino.lib;rapidassist.lib;                                                                                                |

4) Add your library and unit test source code files to the project by drag and dropping the files on the project.

5) Configure the main() function to launch Google Test's RUN_ALL_TESTS() macro. The file `/usage/manual/main.cpp` is a good start point.

6) Compile the project.




## Source code example ##

The following section shows an example of using win32Arduino.

Assume a developer needs to test the following library function:
```cpp
bool waitForButtonPress(uint8_t buttonPin, uint32_t timeout) {
  uint32_t start = millis();
  while( millis() - start < timeout )
  {
    //look for button state
    int buttonValue = digitalRead(buttonPin);
    if (buttonValue == LOW)
      return true;
  }
  //timeout
  return false;
}
```

Using Google Test framework, one can write the following unit test to validate the behavior of the `waitForButtonPress()` function:

```cpp
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
  attachMillisecondsCallback(buttonPressTime, simulatePinLowISR); //in 2000 ms, the button pin will go LOW

  //run the function again...
  //assert that function is interrupted when a button is pressed
  time1 = millis();
  result = waitForButtonPress(buttonPin, MAX_WAIT_TIME);
  time2 = millis();
  elapsed = time2 - time1;
  ASSERT_TRUE(result);
  ASSERT_EQ(BUTTON_DELAY_TIME, elapsed);
}
```



# Build / Install / Testing #

Please refer to file [INSTALL.md](INSTALL.md) for details on how installing/building the application.




# Platforms #

win32Arduino has been tested with the following platform:

  * Linux x86/x64
  * Windows x86/x64


  
  
# Versioning #

We use [Semantic Versioning 2.0.0](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/end2endzone/win32Arduino/tags).




# Authors #

* **Antoine Beauchamp** - *Initial work* - [end2endzone](https://github.com/end2endzone)

See also the list of [contributors](https://github.com/end2endzone/win32Arduino/blob/master/AUTHORS) who participated in this project.




# License #

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
