#include <stdio.h>
#include <iostream>
#include <stdio.h>

#include <gtest/gtest.h>

#include "rapidassist/gtesthelp.h"

using namespace ra::gtesthelp;

int main(int argc, char **argv)
{
  //define default values for xml output report
  if (isProcessorX86())
  {
    if (isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:win32arduino_unittest.x86.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:win32arduino_unittest.x86.release.xml";
  }
  else if (isProcessorX64())
  {
    if (isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:win32arduino_unittest.x64.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:win32arduino_unittest.x64.release.xml";
  }

  ::testing::GTEST_FLAG(filter) = "*";
  ::testing::InitGoogleTest(&argc, argv);

  int wResult = RUN_ALL_TESTS(); //Find and run all tests
  return wResult; // returns 0 if all the tests are successful, or 1 otherwise
}
