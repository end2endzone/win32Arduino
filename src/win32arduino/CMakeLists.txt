set(WIN32ARDUINO_HEADER_FILES ""
  ${WIN32ARDUINO_INCLUDE_DIR}/arduino.h
  ${WIN32ARDUINO_INCLUDE_DIR}/IClockStrategy.h
  ${WIN32ARDUINO_INCLUDE_DIR}/IncrementalClockStrategy.h
  ${WIN32ARDUINO_INCLUDE_DIR}/RealtimeClockStrategy.h
  ${WIN32ARDUINO_INCLUDE_DIR}/SerialPrinter.h
  ${WIN32ARDUINO_INCLUDE_DIR}/avr/pgmspace.h
  ${WIN32ARDUINO_INCLUDE_DIR}/util/atomic.h
)

add_library(win32arduino STATIC
  ${WIN32ARDUINO_HEADER_FILES}
  ${WIN32ARDUINO_EXPORT_HEADER}
  ${WIN32ARDUINO_VERSION_HEADER}
  ${WIN32ARDUINO_CONFIG_HEADER}
  arduino.cpp
  IncrementalClockStrategy.cpp
  RealtimeClockStrategy.cpp
  SerialPrinter.cpp
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(win32arduino PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for exported code.
target_include_directories(win32arduino
  PUBLIC
    $<INSTALL_INTERFACE:${WIN32ARDUINO_INSTALL_INCLUDE_DIR}>  # for clients using the installed library.
  PRIVATE
    ${GTEST_INCLUDE_DIR}
)
target_link_libraries(win32arduino PUBLIC rapidassist PRIVATE ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

install(TARGETS win32arduino
        EXPORT win32arduino-targets
        ARCHIVE DESTINATION ${WIN32ARDUINO_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${WIN32ARDUINO_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${WIN32ARDUINO_INSTALL_BIN_DIR}
)
