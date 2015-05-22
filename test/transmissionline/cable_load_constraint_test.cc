// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/cable_load_constraint.h"

#include "gtest/gtest.h"

class CableLoadConstraintTest : public ::testing::Test {
 protected:
  CableLoadConstraintTest() {

    // builds dependency object - load case
    CableLoadCase case_load;
    case_load.load_unit.set_x(0);
    case_load.load_unit.set_y(1.094);
    case_load.temperature_cable = 0;

    // builds fixture object
    c_.case_load = case_load;
    c_.condition = ConditionType::kInitial;
    c_.limit = 23625;
    c_.type_limit = ConstraintLimitType::kSupportTension;
  }

  CableLoadConstraint c_;
};

TEST_F(CableLoadConstraintTest, Validate) {

  EXPECT_TRUE(c_.Validate(true, nullptr));
}
