# win32Arduino #
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/win32Arduino.svg)](https://github.com/end2endzone/win32Arduino/releases)

win32Arduino is a Windows/Linux implementation of arduino functions. It allows a developer to unit test an arduino library outside of the arduino platform.

The library allows to easily unit test an arduino library using your testing framework of choice. For instance, the unit tests of win32Arduino library are executed using the [Google Test framework](http://github.com/google/googletest).



## Status ##

Build:

| Service | Build | Tests |
|----|-------|-------|
| AppVeyor | [![Build status](https://img.shields.io/appveyor/ci/end2endzone/win32Arduino/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/win32arduino) | [![Tests status](https://img.shields.io/appveyor/tests/end2endzone/win32arduino/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/win32arduino/branch/master/tests) |
| Travis CI | [![Build Status](https://img.shields.io/travis/end2endzone/win32Arduino/master.svg?logo=travis&style=flat)](https://travis-ci.org/end2endzone/win32Arduino) |  |

Statistics:

| AppVeyor | Travic CI |
|----------|-----------|
| [![Statistics](https://buildstats.info/appveyor/chart/end2endzone/win32arduino)](https://ci.appveyor.com/project/end2endzone/win32arduino/branch/master) | [![Statistics](https://buildstats.info/travisci/chart/end2endzone/win32Arduino)](https://travis-ci.org/end2endzone/win32Arduino) |




# Features #

The main features of the library are:

* Implements many arduino functions.
* Support all functions that reads or writes values to pins.
* Advanced time handling mechanism. Supports realtime, incremental or custom `millis()` and `micros()` functions to simulate time.
* Call history and logging support. Calls to the `Serial` class are also forwarded to the output log file.
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

Arduino library source code must be added to a win32/linux project to be compiled and tested outside of the arduino environment.

The following instructions show how to test your own library with win32Arduino.



### 1) Verify library prerequisites ###

To be compatible with the files provided by win32Arduino, an arduino library must implement the [Arduino IDE 1.5: Library specification](https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification).

Source files must be stored in `src/` directory.

Unit tests are expected to be written using the [Google Test framework](https://github.com/google/googletest/). This guide assumes that you are already familiar with the googletest API.

Unit test files must be stored in `test/` directory.



### 2) Copy CMake template files to your library project ###

The win32Arduino expect your arduino library project to be compatible with the CMake build system. CMake build system is used to generate a platform-specific (Windows or Linux) build environment.

win32Arduino provides template files that must be copied from `/usage/template/` directory into the arduino library project.

In case of conflict (if the copied files already exist or if your library is already using CMake), you must merge the files manually.



### 3) Build library unit tests ###

To generate the Windows or Linux project files, open a command prompt, browse to your arduino library directory and enter the following commands:

```
mkdir build
cd build
cmake ..
```

To build the project, enter the following command (based on your platform):
  1) On Windows, run `cmake --build . --config Release`.
  2) On Linux, run `make` command.
  

  
### Troubleshooting ###

The `CMakeLists.txt` file is expecting to find your test files in the `test/` directory and your library source files in the `src/` directory.

Note that `CMakeLists.txt` file does not automatically search for dependencies. If the tested arduino library depends on another, you must manually edit the `CMakeLists.txt` file and add the required lines to resolve all dependencies and have a successful build.




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
