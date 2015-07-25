// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/cable.h"

#include "gtest/gtest.h"

#include "factory.h"

class CableComponentTest : public ::testing::Test {
 protected:
  CableComponentTest() {
    c_ = factory::BuildCableComponent();
  }

  CableComponent c_;
};

TEST_F(CableComponentTest, Validate) {

  EXPECT_TRUE(c_.Validate(true, nullptr));
}

class CableTest : public ::testing::Test {
 protected:
  CableTest() {
    c_ = factory::BuildCable();
  }

  Cable c_;
};

TEST_F(CableTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
