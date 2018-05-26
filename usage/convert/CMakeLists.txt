cmake_minimum_required(VERSION 3.4.3)
project(TestProject)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# googletest
include_directories($ENV{GOOGLETEST_HOME}/include)
link_directories($ENV{GOOGLETEST_HOME}/build)

# win32Arduino
include_directories($ENV{WIN32ARDUINO_HOME}/include)
link_directories($ENV{WIN32ARDUINO_HOME}/build/bin)

# RapidAssist
include_directories($ENV{RAPIDASSIST_HOME}/include)
link_directories($ENV{RAPIDASSIST_HOME}/build/bin)

#Add source files.
file( GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h )
add_executable(TestProject ${SOURCE_FILES} )

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#link libraries
target_link_libraries(TestProject debug     win32Arduino rapidassist gtest)
target_link_libraries(TestProject optimized win32Arduino rapidassist gtest)
