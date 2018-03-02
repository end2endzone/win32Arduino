![win32Arduino logo](https://github.com/end2endzone/win32Arduino/raw/master/docs/win32Arduino-splashscreen.png)


[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/win32Arduino.svg)](https://github.com/end2endzone/win32Arduino/releases)
[![Build status](https://ci.appveyor.com/api/projects/status/4jhi6oqgaji5e7pl/branch/master?svg=true)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master)
[![Tests status](https://img.shields.io/appveyor/tests/end2endzone/win32Arduino/master.svg)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master/tests)

AppVeyor build statistics:

[![Build statistics](https://buildstats.info/appveyor/chart/end2endzone/win32Arduino)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master)



# win32Arduino

win32Arduino is a Windows (win32) implementation of arduino functions. It allows a developer to unit test an arduino library outside of the arduino platform.

The library allows to easily unit test an arduino library using your testing framework of choice. For instance, the unit tests of win32Arduino library are executed using the [Google Test framework](http://github.com/google/googletest).

It's main features are:

*  Implements most arduino functions.
*  Allows a developer to test a library outside of the arduino platform.
*  Quicker unit test execution.
*  Supports realtime millis() function or simulated millis() and micros() functions.

# Usage

The following instructions show how to test an arduino library. 

## Disabling win32Arduino's unit tests

This section explains how to disable compilation of win32Arduino's own unit tests which are not required for testing another library.

1) Edit the file /src/CMakeLists.txt
2) Comment each lines that contains a reference to '*win32Arduino_unittest*' by adding a # character at the beginning of the line.
1) Compile source code according to instructions specified in [INSTALL.md](INSTALL.md) file.
3) Binaries are available in /cmake/build/bin/$(Configuration)

## Create a test project

The following instructions show how to easily test an arduino library. For clarity, unit test are written using the Google Test framework. This section assumes that you are already familiar with the googletest API.
   
An arduino library source code must be added to a win32 project to be compiled and tested. The following instructions show how to compile an arduino library on the Windows platform. In the example below, the win32Arduino library is used to resolve all arduino functions called by the library.

1) Create a new win32 console application.

2) Configure source code according to the googletest practices. For more information on how googletest is working, see the [google test documentation primer](https://github.com/google/googletest/blob/release-1.8.0/googletest/docs/V1_6_Primer.md). Configure the main() function to launch Google Test’s RUN_ALL_TESTS() macro.

2) Create a static library project which will "wrap" all the arduino files of the library you need to test.

3) Modify the static library’s '*Additionnal Include Directories*' to point to win32Arduino project source code. This allows the wrapping library to resolve all arduino.h includes and all arduino symbols using the win32Arduino library.

## Example
The following section shows an example of using win32Arduino to test an arduino function.

Assume a developer needs to test a library which contains the following functions:
```cpp
void wait5Seconds() {
  unsigned long start = millis();
  while( millis() - start < 5000 )
  {
  }
}

bool waitForButtonPress(uint8_t buttonPin, unsigned long timeout) {
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
```

Using Google Test framework, one can write the following unit test to validate the expectations of each functions:

```cpp
TEST(TestMyLibrary, testWait5Seconds) {
  testarduino::setClockStrategy(testarduino::CLOCK_SIMULATION);
  testarduino::setMicrosecondsCounter(0);
  testarduino::setMicrosecondsResolution(1000); //increase simulated clock by 1ms for every calls to micros()
  uint32_t before = millis();
  wait5Seconds();
  uint32_t after = millis();
  ASSERT_GE(after - before, 5000);
}

TEST(TestMyLibrary, testWaitForButtonPressTimeout) {
  testarduino::setClockStrategy(testarduino::CLOCK_SIMULATION);
  testarduino::setMicrosecondsCounter(0);
  testarduino::setMicrosecondsResolution(1000); //increase simulated clock by 1ms for every calls to micros()
  uint32_t before = millis();
  uint8_t buttonPin = 2;
  waitForButtonPress(buttonPin, 5000);
  uint32_t after = millis();
  ASSERT_GE(after - before, 5000);
}
```

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
   1) Open a file navigator and browse to the output folder(for example c:\projects\win32Arduino\msvc\Win32\Release)
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