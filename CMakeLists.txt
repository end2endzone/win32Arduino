cmake_minimum_required(VERSION 3.4.3)
project(win32Arduino)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

MESSAGE( STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR} )
MESSAGE( STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR} )
MESSAGE( STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( STATUS "PROJECT_BINARY_DIR:       " ${PROJECT_BINARY_DIR} )
MESSAGE( STATUS "PROJECT_SOURCE_DIR:       " ${PROJECT_SOURCE_DIR} )
MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH:   " ${EXECUTABLE_OUTPUT_PATH} )
MESSAGE( STATUS "LIBRARY_OUTPUT_PATH:      " ${LIBRARY_OUTPUT_PATH} )
MESSAGE( STATUS "CMAKE_MODULE_PATH:        " ${CMAKE_MODULE_PATH} )
MESSAGE( STATUS "CMAKE_COMMAND:            " ${CMAKE_COMMAND} )
MESSAGE( STATUS "CMAKE_ROOT:               " ${CMAKE_ROOT} )
MESSAGE( STATUS "CMAKE_CURRENT_LIST_FILE:  " ${CMAKE_CURRENT_LIST_FILE} )
MESSAGE( STATUS "CMAKE_CURRENT_LIST_LINE:  " ${CMAKE_CURRENT_LIST_LINE} )
MESSAGE( STATUS "CMAKE_INCLUDE_PATH:       " ${CMAKE_INCLUDE_PATH} )
MESSAGE( STATUS "CMAKE_LIBRARY_PATH:       " ${CMAKE_LIBRARY_PATH} )
MESSAGE( STATUS "CMAKE_SYSTEM:             " ${CMAKE_SYSTEM} )
MESSAGE( STATUS "CMAKE_SYSTEM_NAME:        " ${CMAKE_SYSTEM_NAME} )
MESSAGE( STATUS "CMAKE_SYSTEM_VERSION:     " ${CMAKE_SYSTEM_VERSION} )
MESSAGE( STATUS "CMAKE_SYSTEM_PROCESSOR:   " ${CMAKE_SYSTEM_PROCESSOR} )

#------------------------------------------------------------------------------------------------------------
# Set a default build type if none was specified.
# See https://blog.kitware.com/cmake-and-the-default-build-type/
#------------------------------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  mark_as_advanced(CMAKE_BUILD_TYPE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#------------------------------------------------------------------------------------------------------------
# Define product version according to Semantic Versioning v2.0.0 https://semver.org/
#------------------------------------------------------------------------------------------------------------
MESSAGE("")
SET(WIN32ARDUINO_MAJOR_VERSION 2)
SET(WIN32ARDUINO_MINOR_VERSION 3)
SET(WIN32ARDUINO_PATCH_VERSION 0)
SET(WIN32ARDUINO_PRERELEASE_VERSION "") #i.e: rc1
SET(WIN32ARDUINO_METADATA_VERSION "")   #i.e: build.45
SET(WIN32ARDUINO_VERSION_STRING ${WIN32ARDUINO_MAJOR_VERSION}.${WIN32ARDUINO_MINOR_VERSION}.${WIN32ARDUINO_PATCH_VERSION})
IF(NOT ${WIN32ARDUINO_PRERELEASE_VERSION} STREQUAL "")
  SET(WIN32ARDUINO_VERSION_STRING ${WIN32ARDUINO_VERSION_STRING}-${WIN32ARDUINO_PRERELEASE_VERSION})
ENDIF()
IF(NOT ${WIN32ARDUINO_METADATA_VERSION} STREQUAL "")
  SET(WIN32ARDUINO_VERSION_STRING ${WIN32ARDUINO_VERSION_STRING}+${WIN32ARDUINO_METADATA_VERSION})
ENDIF()
MESSAGE("WIN32ARDUINO_VERSION_STRING: ${WIN32ARDUINO_VERSION_STRING}")
MESSAGE("")

# Generate version files for different systems
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/src/win32Arduino/version.h.in"        "${PROJECT_BINARY_DIR}/include/version.h")
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/version.in"                           "${CMAKE_CURRENT_SOURCE_DIR}/version")

#------------------------------------------------------------------------------------------------------------
# Validate mandatory environment variables.
#------------------------------------------------------------------------------------------------------------
if(NOT EXISTS "$ENV{GOOGLETEST_HOME}")
  message(FATAL_ERROR "Could not find GOOGLETEST_HOME environment variable")
endif()
if(NOT EXISTS "$ENV{RAPIDASSIST_HOME}")
  message(FATAL_ERROR "Could not find RAPIDASSIST_HOME environment variable")
endif()

#------------------------------------------------------------------------------------------------------------
# Add source directories
#------------------------------------------------------------------------------------------------------------
add_subdirectory(src/win32Arduino)

#------------------------------------------------------------------------------------------------------------
# unit tests
#------------------------------------------------------------------------------------------------------------
option(WIN32ARDUINO_BUILD_TEST "Build all win32Arduino tests" OFF)
if(WIN32ARDUINO_BUILD_TEST)
  add_subdirectory(test)

  # googletest
  if (NOT TARGET gtest)
    if (WIN32)
      # https://stackoverflow.com/a/12546288
      # Force building googletest in `Multi-threaded Debug DLL (/MDd)` and `Multi-threaded DLL (/MD)`
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
    add_subdirectory($ENV{GOOGLETEST_HOME})
    set_target_properties(gtest_main PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
  endif()

  # Define project dependencies
  add_dependencies(win32Arduino_unittest win32Arduino)
  add_dependencies(win32Arduino_unittest gtest)
endif()

#------------------------------------------------------------------------------------------------------------
# Add dependencies
#------------------------------------------------------------------------------------------------------------
# RapidAssist
if (NOT TARGET rapidassist)
  # Force not building RapidAssist unit test
  SET(RAPIDASSIST_BUILD_TEST OFF)

  add_subdirectory($ENV{RAPIDASSIST_HOME})
endif()
add_dependencies(win32Arduino rapidassist)

if(WIN32ARDUINO_BUILD_TEST)
  add_dependencies(win32Arduino_unittest rapidassist)
endif()
# Remove WIN32ARDUINO_BUILD_TEST from the cache so calling `cmake ..` again will reset the flag to default value.
unset(WIN32ARDUINO_BUILD_TEST CACHE)

#------------------------------------------------------------------------------------------------------------
# Add all samples to the project unless the user has specified otherwise.
#------------------------------------------------------------------------------------------------------------
option(WIN32ARDUINO_BUILD_SAMPLES "Build all samples projects" OFF)
if(WIN32ARDUINO_BUILD_SAMPLES)
  add_subdirectory(samples/Atomic1)
  add_subdirectory(samples/Atomic2)
  add_subdirectory(samples/Constants)
  add_subdirectory(samples/Interrupts)
  add_subdirectory(samples/MicrosOverflow)
  add_subdirectory(samples/Progmem1)
  add_subdirectory(samples/Progmem2)
  add_subdirectory(samples/Progmem3)

  add_dependencies(Atomic1 win32Arduino)
  add_dependencies(Atomic2 win32Arduino)
  add_dependencies(Constants      win32Arduino)
  add_dependencies(Interrupts     win32Arduino)
  add_dependencies(MicrosOverflow win32Arduino)
  add_dependencies(Progmem1       win32Arduino)
  add_dependencies(Progmem2       win32Arduino)
  add_dependencies(Progmem3       win32Arduino)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_target_properties(Atomic1        PROPERTIES FOLDER "win32Arduino samples")
  set_target_properties(Atomic2        PROPERTIES FOLDER "win32Arduino samples")
  set_target_properties(Constants      PROPERTIES FOLDER "win32Arduino samples")
  set_target_properties(Interrupts     PROPERTIES FOLDER "win32Arduino samples")
  set_target_properties(MicrosOverflow PROPERTIES FOLDER "win32Arduino samples")
  set_target_properties(Progmem1       PROPERTIES FOLDER "win32Arduino samples")
  set_target_properties(Progmem2       PROPERTIES FOLDER "win32Arduino samples")
  set_target_properties(Progmem3       PROPERTIES FOLDER "win32Arduino samples")
endif()

# Remove WIN32ARDUINO_BUILD_SAMPLES from the cache so calling `cmake ..` again will reset the flag to default value.
unset(WIN32ARDUINO_BUILD_SAMPLES CACHE)

#------------------------------------------------------------------------------------------------------------
# Generate doxygen documentation
# See https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
#------------------------------------------------------------------------------------------------------------
option(BUILD_DOC "Build documentation" ON)
# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
  # set input and output files
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
 
  # request to configure the file
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  message("Doxygen build started")
 
  # note the option ALL which allows to build the docs together with the application
  add_custom_target( doc ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
 


