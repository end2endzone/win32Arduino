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

Note: this task can be automated using the `ant configure` command.

# Building

This section explains how to compile and build the software and how to get a development environment ready.

## Prerequisites

The following software must be installed on the system for compiling source code:

* Visual Studio 2010 (or newer)
* [Google C++ Testing Framework v1.6.0](https://github.com/google/googletest/tree/release-1.6.0) (untested with other versions)
* [CMake](http://www.cmake.org/) v3.4.3 (or newer)
* [win32Arduino source code](http://github.com/end2endzone/win32Arduino/tags)

For using [Continuous Integration](#continuous-integration) scripts, the following software must be installed on the system:

* [Java SE Runtime Environment](http://www.oracle.com/technetwork/java/javase/downloads/index.html) version 8 (or newer).
* [Apache Ant(TM)](https://ant.apache.org/bindownload.cgi) version 1.8.4 (or newer)

## Build steps

### Google C++ testing framework

1) Download googletest source code as a [zip file](https://github.com/google/googletest/archive/release-1.6.0.zip) to your computer and extract to a temporary directory (for example `c:\projects\third_party\googletest`).

2) Generate the Visual Studio solution using the following commands:
   * cd c:\projects\third_party\googletest
   * mkdir build
   * cd build
   * cmake -Dgtest_force_shared_crt=ON -DCMAKE_CXX_FLAGS_DEBUG=/MDd -DCMAKE_CXX_FLAGS_RELEASE=/MD ..

3) Open the generated Visual Studio solution file located in `c:\projects\third_party\googletest\build\gtest.sln`.

4) Build the solution.

For building unit tests, the application needs to know where the googletest libraries (debug & release) are located.

The following environment variables should be defined:

| Name                     | Value                                        |
|--------------------------|----------------------------------------------|
|  GOOGLETEST_HOME         | c:\projects\third_party\googletest           |

Note that the `GOOGLETEST_HOME` variable should match the actual directory where the source code was extracted.

Note: this task can be automated using the `ant install` command.

### win32Arduino

1) Download the [win32Arduino source code](https://github.com/end2endzone/win32Arduino/tags) and extract the content to a local directory (for example `c:\projects\win32Arduino`).

2) Generate the Visual Studio solution using the following commands:
   * cd c:\projects\win32Arduino
   * mkdir build
   * cd build
   * cmake ..

3) Open the generated Visual Studio solution file located in `c:\projects\win32Arduino\build\win32Arduino.sln`.

4) Build the solution.

Note: this task can be automated using the `ant compile` command.

### win32Arduino deploy package

The library does not provide an installation package. It is deployed using a zip archive which only contains the source code.

GitHub automatically provides zip archives of all the source code hosted on its platform. No deploy package is required for the library.

## Disabling win32Arduino's unit tests

This section explains how to disable compilation of win32Arduino's own unit tests which are not required for testing another library.

1) Edit the file /CMakeLists.txt
2) Add the following like at the end of the document: 

```CMake
set_target_properties(win32Arduino_unittest PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
```

3) Compile source code according to instructions specified in [Building](#building) section.

Note: this task can be automated using the `ant disable_unittest` command.

# Continuous Integration

win32Arduino is using [Apache Ant](https://ant.apache.org/index.html) command-line tool to help building software. It provides compatibility for all scripts across all supported platforms.

## Build servers

win32Arduino supports multiple continuous integration services (build server). To uniformalize the build process across all services, it is recommended to use the following ant scripts.

## Ant targets

The project support the following `ant target`:

| Target           | Description                                                                                 |
|------------------|---------------------------------------------------------------------------------------------|
| clean            | Delete build, deploy and publish directories.                                               |
| compile          | Build all source files.                                                                     |
| configure        | Generate a script file named 'setupenv' which setup the environment variables for building. |
| disable_unittest | Delete all build artifacts.                                                                 |
| install          | Installs required libraries in third_party directory                                        |
| reset            | Delete all build artifacts and third_parties.                                               |
| test             | Runs unit & system tests.                                                                   |

Note: the following project help information can be generated using the `ant -p` command.

To execute a target, navigate to the `/ci/ant` directory and enter the following command:
```
ant [targetname]
```
