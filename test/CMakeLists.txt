add_executable(win32arduino_unittest
  ${RAPIDASSIST_EXPORT_HEADER}
  ${RAPIDASSIST_VERSION_HEADER}
  ${RAPIDASSIST_CONFIG_HEADER}
  main.cpp
  TestGetLastCommand.h
  TestGetLastCommand.cpp
  TestWin32Arduino.h
  TestWin32Arduino.cpp
  TestDemo.h
  TestDemo.cpp
)

# Unit test projects requires to link with pthread if also linking with gtest
if(NOT WIN32)
  set(PTHREAD_LIBRARIES -pthread)
endif()

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(win32arduino_unittest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(win32arduino_unittest PRIVATE rapidassist)
target_include_directories(win32arduino_unittest PRIVATE ${GTEST_INCLUDE_DIR})
add_dependencies(win32arduino_unittest rapidassist)
target_link_libraries(win32arduino_unittest PUBLIC rapidassist win32Arduino ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

install(TARGETS win32arduino_unittest
        EXPORT win32arduino-targets
        ARCHIVE DESTINATION ${WIN32ARDUINO_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${WIN32ARDUINO_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${WIN32ARDUINO_INSTALL_BIN_DIR}
)
