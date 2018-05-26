cmake_minimum_required(VERSION 3.4.3)
project(Atomic2)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

MESSAGE( STATUS "Adding sample project : " ${PROJECT_NAME} )

# Validate mandatory environment variables.
if(NOT EXISTS "$ENV{WIN32ARDUINO_HOME}")
  message(FATAL_ERROR "Could not find WIN32ARDUINO_HOME environment variable")
endif()

include_directories($ENV{WIN32ARDUINO_HOME}/include)
link_directories($ENV{WIN32ARDUINO_HOME}/build/bin)

add_executable(Atomic2 main.cpp )

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(Atomic2 debug     win32Arduino rapidassist)
target_link_libraries(Atomic2 optimized win32Arduino rapidassist)
