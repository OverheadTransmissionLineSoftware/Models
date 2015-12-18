// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable.h"

#include "gtest/gtest.h"

#include "factory.h"

class CableComponentTest : public ::testing::Test {
 protected:
  CableComponentTest() {
    cable_ = factory::BuildCable();
    c_ = cable_->component_shell;
  }

  ~CableComponentTest() {
    delete cable_;
  }

  // dependency object
  Cable* cable_;

  // test object
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

  ~CableTest() {
    delete c_;
  }

  // test object
  Cable* c_;
};

TEST_F(CableTest, Validate) {
  EXPECT_TRUE(c_->Validate(true, nullptr));
}
