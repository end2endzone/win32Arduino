cmake_minimum_required(VERSION 3.4.3)
project(MicrosOverflow)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

MESSAGE( STATUS "Adding sample project : " ${PROJECT_NAME} )

add_executable(MicrosOverflow main.cpp)

target_link_libraries(MicrosOverflow debug     win32Arduino rapidassist)
target_link_libraries(MicrosOverflow optimized win32Arduino rapidassist)
