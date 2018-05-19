add_library(win32Arduino STATIC 
  ${PROJECT_BINARY_DIR}/src/win32Arduino/version.h
  avr/pgmspace.h
  arduino.cpp
  arduino.h
  IncrementalClockStrategy.cpp
  IncrementalClockStrategy.h
  IClockStrategy.h
  RealtimeClockStrategy.cpp
  RealtimeClockStrategy.h
  SerialPrinter.cpp
  SerialPrinter.h
)

include_directories($ENV{RAPIDASSIST_HOME}/include)
link_directories($ENV{RAPIDASSIST_HOME}/build)
link_directories(${LIBRARY_OUTPUT_PATH})

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(win32Arduino debug     rapidassist)
target_link_libraries(win32Arduino optimized rapidassist)
