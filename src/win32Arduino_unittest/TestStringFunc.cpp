#include "TestStringFunc.h"
#include "stringfunc.h"

namespace stringfunc { namespace test
{

  //--------------------------------------------------------------------------------------------------
  void TestStringFunc::SetUp()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void TestStringFunc::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  TEST(TestStringFunc, testIsNumeric)
  {
    //lazy test
    for(int i = -100; i <= 100; i++)
    {
      for(int j=0; j<=100; j++)
      {
        //build a string from i and j
        static const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        //integers
        sprintf(buffer, "%d", i);
        ASSERT_TRUE(stringfunc::isNumeric(buffer)) << "isNumeric(\"" << buffer << "\") returned false.";

        //floating point
        sprintf(buffer, "%d.%03d", i, j);
        ASSERT_TRUE(stringfunc::isNumeric(buffer)) << "isNumeric(\"" << buffer << "\") returned false.";
      }
    }

    //test special case
    ASSERT_TRUE(stringfunc::isNumeric("+12"));

    //plus or minus sign error
    ASSERT_FALSE(stringfunc::isNumeric("12+123"));
    ASSERT_FALSE(stringfunc::isNumeric("12-123"));
    ASSERT_FALSE(stringfunc::isNumeric("+12+123"));
    ASSERT_FALSE(stringfunc::isNumeric("+12-123"));

    //multiple dots
    ASSERT_FALSE(stringfunc::isNumeric("12.345.67"));
    ASSERT_FALSE(stringfunc::isNumeric("+12.345.67"));

    //alpha characters
    ASSERT_FALSE(stringfunc::isNumeric("+12.34a"));
    ASSERT_FALSE(stringfunc::isNumeric("+12.34!"));
  }
  //--------------------------------------------------------------------------------------------------

} // End namespace test
} // End namespace stringfunc
