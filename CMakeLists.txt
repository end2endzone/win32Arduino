# CMakeLists.txt
cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)
project(win32Arduino)

##############################################################################################################################################
# Standard CMake variables
##############################################################################################################################################

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

# Set a default build type if none was specified.
# See https://blog.kitware.com/cmake-and-the-default-build-type/
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  mark_as_advanced(CMAKE_BUILD_TYPE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Export no symbols by default (if the compiler supports it). 
# This makes e.g. GCC's "visibility behavior" consistent with MSVC's.  
# On Windows/MSVC this is a noop. 
if (BUILD_SHARED_LIBS)
  set(CMAKE_C_VISIBILITY_PRESET hidden) 
  set(CMAKE_CXX_VISIBILITY_PRESET hidden) 
endif()

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

##############################################################################################################################################
# CMake properties
##############################################################################################################################################
MESSAGE( STATUS "PROJECT_NAME:             " ${PROJECT_NAME} )
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

##############################################################################################################################################
# Global settings
##############################################################################################################################################

# Product version according to Semantic Versioning v2.0.0 https://semver.org/
SET(WIN32ARDUINO_VERSION_MAJOR 2)
SET(WIN32ARDUINO_VERSION_MINOR 4)
SET(WIN32ARDUINO_VERSION_PATCH 0)
set(WIN32ARDUINO_VERSION ${WIN32ARDUINO_VERSION_MAJOR}.${WIN32ARDUINO_VERSION_MINOR}.${WIN32ARDUINO_VERSION_PATCH})

# read license file
file(READ ${CMAKE_SOURCE_DIR}/LICENSE.h LICENSE)

# version.h file
set(WIN32ARDUINO_VERSION_HEADER ${CMAKE_BINARY_DIR}/include/win32arduino/version.h)
message("Generating ${WIN32ARDUINO_VERSION_HEADER}...")
configure_file( ${CMAKE_SOURCE_DIR}/src/win32arduino/version.h.in ${WIN32ARDUINO_VERSION_HEADER} )

# config.h file
set(WIN32ARDUINO_CONFIG_HEADER ${CMAKE_BINARY_DIR}/include/win32arduino/config.h)
message("Generating ${WIN32ARDUINO_CONFIG_HEADER}...")
if (BUILD_SHARED_LIBS)
  set(WIN32ARDUINO_BUILD_TYPE_CPP_DEFINE "#define WIN32ARDUINO_BUILT_AS_SHARED")
else()
  set(WIN32ARDUINO_BUILD_TYPE_CPP_DEFINE "#define WIN32ARDUINO_BUILT_AS_STATIC")
endif()
configure_file( ${CMAKE_SOURCE_DIR}/src/win32arduino/config.h.in ${WIN32ARDUINO_CONFIG_HEADER} )
set(WIN32ARDUINO_BUILD_TYPE_CPP_DEFINE)

# Define installation directories
set(WIN32ARDUINO_INSTALL_BIN_DIR      "bin")
set(WIN32ARDUINO_INSTALL_LIB_DIR      "lib/win32arduino-${WIN32ARDUINO_VERSION}")
set(WIN32ARDUINO_INSTALL_INCLUDE_DIR  "include/win32arduino-${WIN32ARDUINO_VERSION}")
set(WIN32ARDUINO_INSTALL_CMAKE_DIR    ${WIN32ARDUINO_INSTALL_LIB_DIR}) # CMake files (*.cmake) should have the same destination as the library files. Some also prefers to use "cmake".

##############################################################################################################################################
# Project settings
##############################################################################################################################################

# Build options
option(WIN32ARDUINO_BUILD_TEST "Build all win32Arduino's unit tests" OFF)
option(WIN32ARDUINO_BUILD_SAMPLES "Build all samples projects" OFF)
option(WIN32ARDUINO_BUILD_DOC "Build documentation" OFF)

# Force a debug postfix if none specified.
# This allows publishing both release and debug binaries to the same location
# and it helps to prevent linking with the wrong library on Windows.
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "-d")
endif()

# Prevents annoying warnings on MSVC
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Define include directories for source code.
# The specified values will not be exported.
set( WIN32ARDUINO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include )
include_directories(${WIN32ARDUINO_INCLUDE_DIR}   # public header files, for source code.
                    ${CMAKE_BINARY_DIR}/include   # for ${WIN32ARDUINO_VERSION_HEADER} and ${WIN32ARDUINO_CONFIG_HEADER} generated files.
)

##############################################################################################################################################
# Dependencies
##############################################################################################################################################
find_package(GTest REQUIRED) #rapidassist requires GTest
find_package(rapidassist 0.9.1 REQUIRED)

##############################################################################################################################################
# Subprojects
##############################################################################################################################################

add_subdirectory(src/win32arduino)

# unit tests
if(WIN32ARDUINO_BUILD_TEST)
  add_subdirectory(test)
endif()

##############################################################################################################################################
# Support for static and shared library
##############################################################################################################################################

if (BUILD_SHARED_LIBS)
  set(WIN32ARDUINO_EXPORT_HEADER_FILENAME "export.h")
  set(WIN32ARDUINO_EXPORT_HEADER ${CMAKE_BINARY_DIR}/include/win32arduino/${WIN32ARDUINO_EXPORT_HEADER_FILENAME})
  message("Generating ${WIN32ARDUINO_EXPORT_HEADER_FILENAME} for shared library...")
  include (GenerateExportHeader) 
  GENERATE_EXPORT_HEADER(win32arduino 
               BASE_NAME win32arduino 
               EXPORT_MACRO_NAME WIN32ARDUINO_EXPORT 
               EXPORT_FILE_NAME ${WIN32ARDUINO_EXPORT_HEADER} 
               STATIC_DEFINE WIN32ARDUINO_BUILT_AS_STATIC
  )
endif()

##############################################################################################################################################
# Add all samples to the project unless the user has specified otherwise.
##############################################################################################################################################
if(WIN32ARDUINO_BUILD_SAMPLES)
  add_subdirectory(samples/Atomic1)
  add_subdirectory(samples/Atomic2)
  add_subdirectory(samples/Constants)
  add_subdirectory(samples/Interrupts)
  add_subdirectory(samples/MicrosOverflow)
  add_subdirectory(samples/Progmem1)
  add_subdirectory(samples/Progmem2)
  add_subdirectory(samples/Progmem3)

  add_dependencies(Atomic1        win32arduino)
  add_dependencies(Atomic2        win32arduino)
  add_dependencies(Constants      win32arduino)
  add_dependencies(Interrupts     win32arduino)
  add_dependencies(MicrosOverflow win32arduino)
  add_dependencies(Progmem1       win32arduino)
  add_dependencies(Progmem2       win32arduino)
  add_dependencies(Progmem3       win32arduino)

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

##############################################################################################################################################
# Generate doxygen documentation
# See https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
##############################################################################################################################################
if (WIN32ARDUINO_BUILD_DOC)
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
    add_custom_target( win32Arduino_doc ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM )
  else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)
endif()

##############################################################################################################################################
# Install
##############################################################################################################################################

# Install locations:   See https://unix.stackexchange.com/a/36874
#   On UNIX, installs to "/usr/local".
#   On Windows, installs to "C:\Program Files (x86)\${PROJECT_NAME}" or to "C:\Program Files\${PROJECT_NAME}" for 64 bit binaries

# Target config version verification file
configure_file(${CMAKE_SOURCE_DIR}/cmake/win32arduino-config-version.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/win32arduino-config-version.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/win32arduino-config-version.cmake DESTINATION ${WIN32ARDUINO_INSTALL_CMAKE_DIR})

# Target config file
configure_file(${CMAKE_SOURCE_DIR}/cmake/win32arduino-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/win32arduino-config.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/win32arduino-config.cmake DESTINATION ${WIN32ARDUINO_INSTALL_CMAKE_DIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${WIN32ARDUINO_INSTALL_INCLUDE_DIR})
install(FILES ${WIN32ARDUINO_EXPORT_HEADER}
              ${WIN32ARDUINO_VERSION_HEADER}
              ${WIN32ARDUINO_CONFIG_HEADER}
              DESTINATION ${WIN32ARDUINO_INSTALL_INCLUDE_DIR})
install(EXPORT win32arduino-targets DESTINATION ${WIN32ARDUINO_INSTALL_CMAKE_DIR})

##############################################################################################################################################
# Packaging
##############################################################################################################################################

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${WIN32ARDUINO_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR "${WIN32ARDUINO_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${WIN32ARDUINO_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${WIN32ARDUINO_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "win32Arduino - win32Arduino is a windows/linux implementation of many arduino functions to allow an arduino library developer to unit test code outside of the arduino platform.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# we don't want to split our program up into several things
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)
