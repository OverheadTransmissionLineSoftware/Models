// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_state.h"

#include "gtest/gtest.h"

class CableStateTest : public ::testing::Test {
 protected:
  CableStateTest() {
    c_.temperature = 0;
    c_.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;
  }

  // test object
  CableState c_;
};

TEST_F(CableStateTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}

class CableStretchStateTest : public ::testing::Test {
 protected:
  CableStretchStateTest() {
    c_.load = 0;
    c_.temperature = 0;
    c_.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;
  }

  // test object
  CableStretchState c_;
};

TEST_F(CableStretchStateTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
