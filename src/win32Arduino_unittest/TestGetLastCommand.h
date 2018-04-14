#ifndef TESTGETLASTCOMMAND_H
#define TESTGETLASTCOMMAND_H

#include <gtest/gtest.h>

namespace arduino { namespace test
{
  class TestGetLastCommand : public ::testing::Test
  {
  public:
    virtual void SetUp();
    virtual void TearDown();
  };

} // End namespace test
} // End namespace arduino

#endif //TESTGETLASTCOMMAND_H
