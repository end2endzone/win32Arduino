add_library(common STATIC stringfunc.cpp stringfunc.h)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
