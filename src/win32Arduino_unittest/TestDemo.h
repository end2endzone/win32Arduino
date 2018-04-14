#ifndef TESTDEMO_H
#define TESTDEMO_H

#include <gtest/gtest.h>

namespace arduino { namespace test
{
  class TestDemo : public ::testing::Test
  {
  public:
    virtual void SetUp();
    virtual void TearDown();
  };

} // End namespace test
} // End namespace arduino

#endif //TESTDEMO_H
