include_directories(${PROJECT_SOURCE_DIR}/include)

include_directories($ENV{RAPIDASSIST_HOME}/include)
link_directories($ENV{RAPIDASSIST_HOME}/build)

link_directories(${LIBRARY_OUTPUT_PATH})

file(GLOB_RECURSE WIN32ARDUINO_HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)

add_library(win32Arduino STATIC 
  ${PROJECT_BINARY_DIR}/include/version.h
  ${WIN32ARDUINO_HEADERS}
  arduino.cpp
  IncrementalClockStrategy.cpp
  RealtimeClockStrategy.cpp
  SerialPrinter.cpp
)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(win32Arduino debug     rapidassist)
target_link_libraries(win32Arduino optimized rapidassist)
