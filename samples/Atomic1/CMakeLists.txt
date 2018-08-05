cmake_minimum_required(VERSION 3.4.3)
project(Atomic1)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

MESSAGE( STATUS "Adding sample project : " ${PROJECT_NAME} )

add_executable(Atomic1 main.cpp)

target_link_libraries(Atomic1 debug     win32Arduino rapidassist)
target_link_libraries(Atomic1 optimized win32Arduino rapidassist)
