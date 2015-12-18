// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable_constraint.h"

#include "gtest/gtest.h"

class CableConstraintTest : public ::testing::Test {
 protected:
  CableConstraintTest() {
    // builds dependency object - weather case
    weathercase_ = new WeatherLoadCase();
    weathercase_->description = "0-0-0 In";
    weathercase_->thickness_ice = 0;
    weathercase_->density_ice = 0;
    weathercase_->pressure_wind = 0;
    weathercase_->temperature_cable = 0;

    // builds fixture object
    c_.case_weather = weathercase_;
    c_.condition = CableConditionType::kInitial;
    c_.limit = 23625;
    c_.type_limit = CableConstraint::LimitType::kSupportTension;
  }

  ~CableConstraintTest() {
    delete weathercase_;
  }

  // allocated dependency object
  WeatherLoadCase* weathercase_;

  // test object
  CableConstraint c_;
};

TEST_F(CableConstraintTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
