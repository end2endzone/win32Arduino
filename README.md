![win32Arduino logo](https://github.com/end2endzone/win32Arduino/raw/master/docs/win32Arduino-splashscreen.png)


[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/win32Arduino.svg)](https://github.com/end2endzone/win32Arduino/releases)
[![Build status](https://ci.appveyor.com/api/projects/status/4jhi6oqgaji5e7pl/branch/master?svg=true)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master)
[![Tests status](https://img.shields.io/appveyor/tests/end2endzone/win32Arduino/master.svg)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master/tests)

AppVeyor build statistics:

[![Build statistics](https://buildstats.info/appveyor/chart/end2endzone/win32Arduino)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master)



# win32Arduino

win32Arduino is a Windows implementation (win32) of arduino functions. It allows a developer to unit test an arduino library outside of the arduino platform.

The library allows to easily unit test an arduino library using your testing framework of choice. For instance, the unit tests of win32Arduino library are executed using the [Google Test framework](http://github.com/google/googletest).

It's main features are:
*  Implements many arduino functions.
*  Advanced time handling mechanism.
*  Pin value support. All functions that reads/writes values to pin is supported.
*  Logging support. See the history of all function call for your library. Calls to the `Serial` class are also forwarded to the output log file.
*  Supports realtime, incremental or custom millis() and micros() functions to simulate time.
*  Supports multiple type of callback function:
   *  Attach function callbacks to any native arduino function.
   *  Attach function callbacks when millis() function reach a specific value.
   *  Attach function callbacks when macros() function reach a specific value.
*  Supports interrupts functions: `attachInterrupt()`, `cli()`, `noInterrupts()`, `interrupts()` and `SREG` registry.

Limitations:
* Pins specific functionality is disabled. In other words, all pins are identical and shall considered supporting analog and digital values.
* AVR, POWER, SERIAL, SPI, I2C is not supported.
* PWM is not supported. Simulating PWM changes over time on each PWM pins is hard.
* Pins pullup and pulldown resistors are not supported.
* Pin resolution functions (`analogReadResolution()` and `analogWriteResolution()`) is not implemented. Pins values is always stored using a resolution of 10 bits.
* Shift functions (`shiftIn()` and `shiftOut()`) are not implemented.
* The following functions are also empty shells: `tone()`, `noTone()`, `pinMode()` and `pulseIn()`.


# Usage

The following instructions show how to use the library.

## Testing your own library

An arduino library source code must be added to a win32 project to be compiled and tested. The following instructions show how to easily test your own library with win32Arduino.

For clarity, unit test are written using the Google Test framework. This section assumes that you are already familiar with the googletest API.

Note that method #1 and #2 requires the cmake application.

### 1) All source files in same soup

The easiest way to create a working test project is to copy everything in the same directory and run your test from there. This technique requires cmake installed on the computer.
Here are the steps required for doing this:

1) Create a new directory where all the source files will be copied. For clarity, assume the directory `C:\projects\mylibrary` is used.

2) Copy all source and unit test files of your library to the new directory. For demonstration purpose, you can use the files from the `ButtonBibrary` available in `/samples/ButtonLibrary` folder.

3) Copy all source code files of the win32Arduino library from folder `/src/win32Arduino` to the new directory. Note that win32Arduino unit test source code is not required.

4) Copy cmake's configuration files from `/samples/TestProject1` to the new directory.

5) Launch cmake in the new directory with the following commands:
   * mkdir build
   * cd build
   * cmake -G "Visual Studio 10 2010" ..

### 2) Configure your project to be win32Arduino-aware
This is the recommended way to use win32Arduino.

This method assume that you aleary defined `GOOGLETEST_HOME` and `WIN32ARDUINO_HOME` environment variables.

**TODO**

**TODO**

**TODO**

**TODO**

**TODO**

**TODO**

**TODO**

**TODO**

**TODO**

**TODO**


### 3) Create your test project manually with Visual Studio

1) Create a new win32 console application.

2) Configure source code according to the googletest best practices. For more information on how googletest is working, see the [google test documentation primer](https://github.com/google/googletest/blob/release-1.8.0/googletest/docs/V1_6_Primer.md). Configure the main() function to launch Google Test’s RUN_ALL_TESTS() macro.

2) Create a static library project which contains all the arduino files of the library you need to test.

3) Modify the static library’s '*Additionnal Include Directories*' to point to win32Arduino project source code. This allows the wrapping library to resolve all arduino.h includes and all arduino symbols using the win32Arduino library.

**TODO**

**TODO**

**TODO**

**TODO**




## Source code example
The following section shows an example of using win32Arduino.

Assume a developer needs to test the following library function:
```cpp
bool waitForButtonPress(uint8_t buttonPin, uint32_t timeout) {
  uint32_t start = millis();
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
```

Using Google Test framework, one can write the following unit test to validate the expectations of the `waitForButtonPress()` function:

```cpp
void simulatePinHighISR() {
  testarduino::setPinDigitalValue(2, HIGH);
}
TEST(TestButtonLibrary, testWaitForButtonPressTimeout) {
  testarduino::reset();
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

  //configure win32Arduino library to push a button in 2000 ms.
  static const uint32_t BUTTON_DELAY_TIME = 2000; //ms
  uint32_t buttonPressTime = millis() + BUTTON_DELAY_TIME;
  attachMillisecondsCallback(buttonPressTime, simulatePinHighISR); //in 2000 ms, the button pin will go HIGH

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

## Disabling win32Arduino's unit tests

This section explains how to disable compilation of win32Arduino's own unit tests which are not required for testing another library.

1) Edit the file /src/CMakeLists.txt
2) Comment each lines that contains a reference to '*win32Arduino_unittest*' by adding a # character at the beginning of the line.
1) Compile source code according to instructions specified in [INSTALL.md](INSTALL.md) file.
3) Binaries are available in /cmake/build/bin/$(Configuration)

# Installing

Please refer to file [INSTALL.md](INSTALL.md) for details on how installing/building the application.

## Testing
win32Arduino comes with unit tests which tests for multiple combinations to make sure that all function behaves as expected.

Test are build using the Google Test v1.6.0 framework. For more information on how googletest is working, see the [google test documentation primer](https://github.com/google/googletest/blob/release-1.8.0/googletest/docs/V1_6_Primer.md).  

Test are automatically build when building the solution. Please see the '*build step*' section for details on how to build the software.

Test can be executed from the following two locations:

1) From the Visual Studio IDE:
   1) Select the project '*win32Arduino_unittest*' as StartUp project.
   2) Hit CTRL+F5 (Start Without Debugging)
2) From the output binaries folder:
   1) Open a file navigator and browse to the output folder(for example c:\projects\win32Arduino\cmake\build\bin\Release)
   2) Run the '*win32Arduino_unittest.exe*' executable.

See also the latest test results at the beginning of the document.

# Compatible with

win32Arduino is only available for the Windows platform and has been tested with the following version of Windows:

*   Windows XP
*   Windows Vista
*   Windows 7

# Versioning

We use [Semantic Versioning 2.0.0](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/end2endzone/win32Arduino/tags).

# Authors

* **Antoine Beauchamp** - *Initial work* - [end2endzone](https://github.com/end2endzone)

See also the list of [contributors](https://github.com/end2endzone/win32Arduino/blob/master/AUTHORS) who participated in this project.

# License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
