cmake_minimum_required(VERSION 3.4.3)
project(Progmem3)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

MESSAGE( STATUS "Adding sample project : " ${PROJECT_NAME} )

add_executable(Progmem3 main.cpp)

target_link_libraries(Progmem3 debug     win32arduino rapidassist)
target_link_libraries(Progmem3 optimized win32arduino rapidassist)
