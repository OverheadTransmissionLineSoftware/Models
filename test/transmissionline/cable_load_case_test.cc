// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/cable_load_case.h"

#include "gtest/gtest.h"

class CableLoadCaseTest : public ::testing::Test {
 protected:
  CableLoadCaseTest() {

    c_.load_unit.set_x(0);
    c_.load_unit.set_y(1.094);
    c_.temperature_cable = 212;
  }

  CableLoadCase c_;
};

TEST_F(CableLoadCaseTest, Validate) {

  EXPECT_TRUE(c_.Validate(true, nullptr));
}
