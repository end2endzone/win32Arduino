#ifndef TESTWIN32ARDUINO_H
#define TESTWIN32ARDUINO_H

#include <gtest/gtest.h>

namespace arduino { namespace test
{
  class TestWin32Arduino : public ::testing::Test
  {
  public:
    virtual void SetUp();
    virtual void TearDown();
  };

} // End namespace test
} // End namespace arduino

#endif //TESTWIN32ARDUINO_H
