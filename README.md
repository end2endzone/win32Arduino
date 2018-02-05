![win32Arduino logo](https://github.com/end2endzone/win32Arduino/raw/master/docs/win32Arduino-splashscreen.png)


[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/win32Arduino.svg)](https://github.com/end2endzone/win32Arduino/releases)
[![Build status](https://ci.appveyor.com/api/projects/status/4jhi6oqgaji5e7pl/branch/master?svg=true)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master)
[![Tests status](https://img.shields.io/appveyor/tests/end2endzone/win32Arduino/master.svg)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master/tests)

AppVeyor build statistics:

[![Build statistics](https://buildstats.info/appveyor/chart/end2endzone/win32Arduino)](https://ci.appveyor.com/project/end2endzone/win32Arduino/branch/master)



# win32Arduino

win32Arduino is a Windows implementation of the most used arduino functions which allows an arduino library developer to unit test his code outside of the arduino platform.

This library allows a windows user to easily unit test an arduino library using your testing framework of choice. For instance, the unit tests of win32Arduino library are executed using the [Google Test framework](http://github.com/google/googletest).

It's main features are:

*  Implements most arduino functions.
*  Allows a developer to tests a library outside of the arduino platform.
*  Quicker unit test execution.
*  Supports realtime millis() function or simulated millis() and micros() functions.

# Usage

An arduino library source code must be added to an existing win32 project to be tested. The following instructions show how to easily test an arduino library using the Google Test framework. It assumes that you are already familiar with the test API.

1)  Create an executable project and configure the main() function to launch Google Test’s RUN_ALL_TESTS() macro.

2)  Create a static library project which will "wrap" all the arduino files of the library you need to test.

3)  Modify the static library’s Additionnal Include Directories to point to win32Arduino library. This allows the wrapping library to resolve all arduino.h includes and all arduino symbols using the win32Arduino library.

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