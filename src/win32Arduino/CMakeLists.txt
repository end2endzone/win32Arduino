add_library(win32Arduino STATIC arduino.cpp arduino.h IncrementalClockStrategy.cpp IncrementalClockStrategy.h IClockStrategy.h RealtimeClockStrategy.cpp RealtimeClockStrategy.h SerialPrinter.cpp SerialPrinter.h)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
