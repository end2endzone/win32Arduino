#include <stdio.h>
#include <iostream>
#include <stdio.h>

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
  //define default values for xml output report
#ifdef _DEBUG
  ::testing::GTEST_FLAG(output) = "xml:TestProject_unittest.debug.xml";
#else
  ::testing::GTEST_FLAG(output) = "xml:TestProjecto_unittest.release.xml";
#endif

  ::testing::GTEST_FLAG(filter) = "*";
  ::testing::InitGoogleTest(&argc, argv);

  int wResult = RUN_ALL_TESTS(); //Find and run all tests
  return wResult; // returns 0 if all the tests are successful, or 1 otherwise
}
