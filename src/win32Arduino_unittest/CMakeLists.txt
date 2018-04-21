include_directories(${CMAKE_SOURCE_DIR}/src/win32Arduino)
link_directories(${LIBRARY_OUTPUT_PATH})

include_directories($ENV{GOOGLETEST_HOME}/include)
link_directories($ENV{GOOGLETEST_HOME}/build)

add_executable(win32Arduino_unittest
  gtesthelper.cpp
  gtesthelper.h
  main.cpp
  TestGetLastCommand.h
  TestGetLastCommand.cpp
  TestWin32Arduino.h
  TestWin32Arduino.cpp
  TestDemo.h
  TestDemo.cpp
  TestNativeFunc.h
  TestNativeFunc.cpp
  TestStringFunc.h
  TestStringFunc.cpp
)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(win32Arduino_unittest debug     win32Arduino gtest)
target_link_libraries(win32Arduino_unittest optimized win32Arduino gtest)
