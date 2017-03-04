// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/hardware.h"

#include "gtest/gtest.h"

#include "factory.h"

class HardwareTest : public ::testing::Test {
 protected:
  HardwareTest() {
    h_ = *factory::BuildHardware();
  }

  Hardware h_;
};

TEST_F(HardwareTest, Validate) {
  EXPECT_TRUE(h_.Validate(true, nullptr));
}