cmake_minimum_required(VERSION 3.4.3)
project(Interrupts)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

MESSAGE( STATUS "Adding sample project : " ${PROJECT_NAME} )

add_executable(Interrupts main.cpp)

target_link_libraries(Interrupts debug     win32arduino rapidassist)
target_link_libraries(Interrupts optimized win32arduino rapidassist)
