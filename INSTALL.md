# Installing

The library does not need to be installed on the system (it does not provide an installation package). It is deployed using a zip archive which only contains the source code which must be compiled to be used by other applications/libraries.

The following steps shows how to install the library:

1) Download the source code from an existing [tags](http://github.com/end2endzone/win32Arduino/tags) and extract the content to a local directory (for example `c:\projects\third_party\win32Arduino`).

2) Compile source code as specified in the [Building](#building) section.

3) Define the following environment variables on the system so that other projects may find the library:

| Name                     | Value                                        |
|--------------------------|----------------------------------------------|
|  WIN32ARDUINO_HOME       | c:\projects\third_party\win32Arduino         |

Note that the `WIN32ARDUINO_HOME` variable should match the actual directory where the source code was extracted.

# Building

This section explains how to compile and build the software and how to get a development environment ready.

## Prerequisites

The following software must be installed on the system for compiling source code:

* Visual Studio 2010 (or newer)
* [Google C++ Testing Framework v1.6.0](https://github.com/google/googletest/tree/release-1.6.0) (untested with other versions)
* [CMake](http://www.cmake.org/) v3.4.3 (or newer)
* [win32Arduino source code](http://github.com/end2endzone/win32Arduino/tags)


## Build steps

### Google C++ testing framework

1) Download googletest source code as a [zip file](https://github.com/google/googletest/archive/release-1.6.0.zip) to your computer and extract to a temporary directory (for example `c:\projects\third_party\googletest`).

2) Generate the Visual Studio 2010 solution using the following commands:
   * cd c:\projects\third_party\googletest
   * mkdir build
   * cd build
   * cmake -G "Visual Studio 10 2010" -Dgtest_force_shared_crt=ON -DCMAKE_CXX_FLAGS_DEBUG=/MDd -DCMAKE_CXX_FLAGS_RELEASE=/MD ..

3) Open the generated Visual Studio 2010 solution file located in `c:\projects\third_party\googletest\build\gtest.sln`.

4) Build the solution.

For building unit tests, the application needs to know where the googletest libraries (debug & release) are located.

The following environment variables should be defined:

| Name                     | Value                                        |
|--------------------------|----------------------------------------------|
|  GOOGLETEST_HOME         | c:\projects\third_party\googletest           |

Note that the `GOOGLETEST_HOME` variable should match the actual directory where the source code was extracted.
 
### win32Arduino

1) Download the [win32Arduino source code](https://github.com/end2endzone/win32Arduino/tags) and extract the content to a local directory (for example `c:\projects\win32Arduino`).

2) Generate the Visual Studio 2010 solution using the following commands:
   * cd c:\projects\win32Arduino
   * cd cmake
   * mkdir build
   * cd build
   * cmake -G "Visual Studio 10 2010" c:\projects\win32Arduino\src

3) Open the generated Visual Studio 2010 solution file located in `c:\projects\win32Arduino\cmake\build\win32Arduino.sln`.

4) Build the solution.

### win32Arduino deploy package

The library does not provide an installation package. It is deployed using a zip archive which only contains the source code.

GitHub automatically provides zip archives of all the source code hosted on its platform. No deploy package is required for the library.

## Disabling win32Arduino's unit tests

This section explains how to disable compilation of win32Arduino's own unit tests which are not required for testing another library.

1) Edit the file /src/CMakeLists.txt
2) Comment each lines that contains a reference to '*win32Arduino_unittest*' by adding a # character at the beginning of the line.
1) Compile source code according to instructions specified in [INSTALL.md](INSTALL.md) file.
3) Binaries are available in /cmake/build/bin/$(Configuration)
