// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable_constraint.h"

#include "gtest/gtest.h"

class CableConstraintTest : public ::testing::Test {
 protected:
  CableConstraintTest() {
    // builds dependency object - weather case
    WeatherLoadCase* case_weather = new WeatherLoadCase();
    case_weather->description = "0-0-0 In";
    case_weather->thickness_ice = 0;
    case_weather->density_ice = 0;
    case_weather->pressure_wind = 0;
    case_weather->temperature_cable = 0;

    // builds fixture object
    c_.case_weather = case_weather;
    c_.condition = CableConditionType::kInitial;
    c_.limit = 23625;
    c_.type_limit = CableConstraint::LimitType::kSupportTension;
  }

  CableConstraint c_;
};

TEST_F(CableConstraintTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
