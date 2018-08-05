set(WIN32ARDUINO_HEADER_FILES ""
  ${CMAKE_SOURCE_DIR}/include/arduino.h
  ${CMAKE_SOURCE_DIR}/include/IClockStrategy.h
  ${CMAKE_SOURCE_DIR}/include/IncrementalClockStrategy.h
  ${CMAKE_SOURCE_DIR}/include/RealtimeClockStrategy.h
  ${CMAKE_SOURCE_DIR}/include/SerialPrinter.h
  ${CMAKE_SOURCE_DIR}/include/avr/pgmspace.h
  ${CMAKE_SOURCE_DIR}/include/util/atomic.h
)

add_library(win32Arduino STATIC
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
set_target_properties(win32Arduino PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for exported code.
target_include_directories(win32Arduino
  PUBLIC
    $<INSTALL_INTERFACE:${WIN32ARDUINO_INSTALL_INCLUDE_DIR}>  # for clients using the installed library.
  PRIVATE
    ${GTEST_INCLUDE_DIR}
)
target_link_libraries(win32Arduino PUBLIC rapidassist PRIVATE ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

install(TARGETS win32Arduino
        EXPORT win32arduino-targets
        ARCHIVE DESTINATION ${WIN32ARDUINO_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${WIN32ARDUINO_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${WIN32ARDUINO_INSTALL_BIN_DIR}
)
