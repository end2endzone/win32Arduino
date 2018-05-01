# Installing #

The library does not need to be installed on the system (it does not provide an installation package). It is deployed using a zip archive which only contains the source code which must be compiled to be used by other applications and libraries.

The following steps shows how to install the library:

1) Download the source code from an existing [tags](http://github.com/end2endzone/win32Arduino/tags) and extract the content to a local directory (for example `c:\projects\third_party\win32Arduino`).

2) Compile source code as specified in the [Building](#building) section.

3) Define the following environment variables on the system so that other projects may find the library:

| Name                     | Value                                        |
|--------------------------|----------------------------------------------|
|  WIN32ARDUINO_HOME       | c:\projects\third_party\win32Arduino         |

Note that the `WIN32ARDUINO_HOME` variable should match the actual directory where the source code was extracted.




# Building #

This section explains how to compile and build the software and how to get a development environment ready.




## Prerequisites ##


### Software Requirements ###
The following software must be installed on the system for compiling source code:

* [Google C++ Testing Framework v1.6.0](https://github.com/google/googletest/tree/release-1.6.0) (untested with other versions)
* [CMake](http://www.cmake.org/) v3.4.3 (or newer)


For using [Continuous Integration](#continuous-integration) scripts, the following software must be installed on the system:

* [Java SE Runtime Environment](http://www.oracle.com/technetwork/java/javase/downloads/index.html) version 8 (or newer).
* [Apache Ant(TM)](https://ant.apache.org/bindownload.cgi) version 1.8.4 (or newer)



### Linux Requirements ###

These are the base requirements to build and use win32Arduino:

  * GNU-compatible Make or gmake
  * POSIX-standard shell
  * A C++98-standard-compliant compiler



### Windows Requirements ###

* Microsoft Visual C++ 2010 or newer




## Build steps ##

### Google C++ testing framework ###

1) Download googletest source code as a [zip file](https://github.com/google/googletest/archive/release-1.6.0.zip) to your computer and extract to a temporary directory (for example `c:\projects\third_party\googletest` or `~/dev/third_party/googletest`).

2) Define the following environment variable so that other projects knows where to find googletest package:

| Name                     | Value                                                                  |
|--------------------------|------------------------------------------------------------------------|
|  GOOGLETEST_HOME         | `c:\projects\third_party\googletest` or `~/dev/third_party/googletest` |

Note that the `GOOGLETEST_HOME` variable should match the actual directory where the source code was extracted.
 
3) Generate the Visual Studio solution or the Makefile using the following commands:

   * cd $GOOGLETEST_HOME
   * mkdir build
   * cd build
   * cmake ..

Note: Windows users should use the following cmake options for compilation: `-Dgtest_force_shared_crt=ON -DCMAKE_CXX_FLAGS_DEBUG=/MDd -DCMAKE_CXX_FLAGS_RELEASE=/MD`

4) Build the source code
   1) On Windows, open the generated Visual Studio solution file located in `$GOOGLETEST_HOME\build\gtest.sln`.
   2) On Linux, build the project using `make` command.

Note: The compilation of _Google C++ testing framework_ can be automated using the `ant install` command. See [Continuous Integration](#continuous-integration) for details.

 
 

### win32Arduino ###

1) Follow [install](#Installing) instructions specified at the beginning of this document.
 
2) Generate the _Visual Studio solution_ or the _Makefile_ using the following commands:

   * cd $WIN32ARDUINO_HOME
   * mkdir build
   * cd build
   * cmake ..

3) Build the source code
   1) On Windows, open the generated Visual Studio solution file located in `$WIN32ARDUINO_HOME\build\win32Arduino.sln`.
   2) On Linux, build the project using `make` command.

Note: The compilation of _win32Arduino_ can be automated using the `ant compile` command. See [Continuous Integration](#continuous-integration) for details.




### win32Arduino deploy package ###

The library does not provide an installation package. It is deployed using a zip archive which only contains the source code.

GitHub automatically provides zip archives of all the source code hosted on its platform. No deploy package is required for the library.




## Disabling win32Arduino's unit tests ##

This section explains how to disable compilation of win32Arduino's own unit tests which are not required for testing another library.

1) Edit the file `/CMakeLists.txt`.
2) Add the following at the end of the document: 

```CMake
set_target_properties(win32Arduino_unittest PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
```

3) Compile source code according to instructions specified in [Building](#building) section.

Note: Disabling win32Arduino's unit tests can be automated using the `ant disable_unittest` command. See [Continuous Integration](#continuous-integration) for details.




# Testing #
win32Arduino comes with unit tests which help maintaining the product stability and level of quality.

Test are build using the Google Test v1.6.0 framework. For more information on how googletest is working, see the [google test documentation primer](https://github.com/google/googletest/blob/release-1.8.0/googletest/docs/V1_6_Primer.md).  

Test are automatically build when building the solution.

To run tests, open a shell prompt and browse to the `build/bin` folder and run `win32Arduino_unittest` executable. Windows users must also specify the configuration name (for example `build\bin\Release`).

Test results are saved in junit format in file `win32Arduino_unittest.x86.debug.xml` or `win32Arduino_unittest.x86.release.xml` depending on the selected configuration.

The latest test results are available at the beginning of the [README.md](README.md) file.

Note: Test case execution can be automated using the `ant test` command. See [Continuous Integration](#continuous-integration) for details.




# Continuous Integration #

win32Arduino is using [Apache Ant](https://ant.apache.org/index.html) command-line tool to help building software. It provides compatibility for all scripts across all supported platforms.




## Build servers ##

win32Arduino supports multiple continuous integration services (build server). To make the build process across all services the same, it is recommended to use the following ant scripts.




## Ant targets ##

The project support the following `ant target`:

| Target           | Description                                                                                      |
|------------------|--------------------------------------------------------------------------------------------------|
| clean            | Delete build, deploy and publish directories.                                                    |
| compile          | Build all source files.                                                                          |
| demos            | Build all arduino demo files using win32Arduino.                                                 |
| disable_unittest | Disable the compilation of project `win32Arduino_unittest`.                                      |
| install          | Installs required software dependencies in third_party directory.                                |
| reset            | Delete all build artifacts and third_parties.                                                    |
| test             | Executes unit tests.                                                                             |


To execute a target, navigate to the `/ci/ant` directory and enter the following command:
```
ant [targetname]
```

Note: the following project help information can be generated using the `ant -p` command.