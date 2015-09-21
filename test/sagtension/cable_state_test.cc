// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_state.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

class CableStateTest : public ::testing::Test {
 protected:
  CableStateTest() {

    c_.load_stretch = 0;
    c_.temperature = 0;
    c_.temperature_stretch = 0;
  }

  CableState c_;
};

TEST_F(CableStateTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
