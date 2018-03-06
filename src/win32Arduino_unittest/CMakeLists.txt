include_directories(${CMAKE_SOURCE_DIR}/win32Arduino)
include_directories($ENV{GTEST_INCLUDE})

link_directories(${LIBRARY_OUTPUT_PATH})
link_directories($ENV{GTEST_LIBRARY_DIR})

add_executable(win32Arduino_unittest demo.cpp demo.h gtesthelper.cpp gtesthelper.h main.cpp TestGetLastCommand.h TestGetLastCommand.cpp TestWin32Arduino.h TestWin32Arduino.cpp )

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(win32Arduino_unittest debug     win32Arduino.lib common.lib gtest.lib)
target_link_libraries(win32Arduino_unittest optimized win32Arduino.lib common.lib gtest.lib)
