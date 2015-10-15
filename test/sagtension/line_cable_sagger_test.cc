// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_sagger.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"
#include "models/base/units.h"

class LineCableSaggerTest : public ::testing::Test {
 protected:
  LineCableSaggerTest() {
    // gets line cable from factory
    LineCable* line_cable = factory::BuildLineCable();

    // builds design constraints
    std::vector<CableConstraint> constraints;
    CableConstraint constraint;
    WeatherLoadCase* case_weather = new WeatherLoadCase();

    case_weather->description = "0-0-60";
    case_weather->thickness_ice = 0;
    case_weather->density_ice = 0;
    case_weather->pressure_wind = 0;
    case_weather->temperature_cable = 60;
    constraint.case_weather = case_weather;
    constraint.condition = CableConditionType::kInitial;
    constraint.limit = 6000;
    constraint.type_limit = CableConstraint::LimitType::kHorizontalTension;
    constraints.push_back(constraint);

    case_weather = new WeatherLoadCase();
    case_weather->description = "0.5-8-0";
    case_weather->thickness_ice =
        units::Convert(0.5, units::ConversionType::kInchesToFeet);
    case_weather->density_ice = 57.3;
    case_weather->pressure_wind = 8;
    case_weather->temperature_cable = 0;
    constraint.case_weather = case_weather;
    constraint.condition = CableConditionType::kInitial;
    constraint.limit = 12000;
    constraint.type_limit = CableConstraint::LimitType::kSupportTension;
    constraints.push_back(constraint);

    case_weather = new WeatherLoadCase();
    case_weather->description = "0-0-212";
    case_weather->thickness_ice = 0;
    case_weather->density_ice = 0;
    case_weather->pressure_wind = 0;
    case_weather->temperature_cable = 212;
    constraint.case_weather = case_weather;
    constraint.condition = CableConditionType::kLoad;
    constraint.limit = 5000;
    constraint.type_limit = CableConstraint::LimitType::kCatenaryConstant;
    constraints.push_back(constraint);

    // builds fixture object
    l_.set_line_cable(line_cable);
    l_.set_constraints_design(constraints);
  }

  LineCableSagger l_;
};

TEST_F(LineCableSaggerTest, CapacityAllowable) {
  double value = -999999;

  value = l_.CapacityAllowable(0);
  EXPECT_EQ(0.970, helper::Round(value, 3));

  value = l_.CapacityAllowable(1);
  EXPECT_EQ(1.000, helper::Round(value, 3));

  value = l_.CapacityAllowable(2);
  EXPECT_EQ(0.813, helper::Round(value, 3));
}

TEST_F(LineCableSaggerTest, CatenaryConstantActual) {
  double value = -999999;

  value = l_.CatenaryConstantActual(0);
  EXPECT_EQ(5320, helper::Round(value, 0));

  value = l_.CatenaryConstantActual(1);
  EXPECT_EQ(4712, helper::Round(value, 0));

  value = l_.CatenaryConstantActual(2);
  EXPECT_EQ(4066, helper::Round(value, 0));
}

TEST_F(LineCableSaggerTest, IndexConstraintControlling) {
  int value = -999;

  value = l_.IndexConstraintControlling();
  EXPECT_EQ(1, value);
}

TEST_F(LineCableSaggerTest, LimitContraintSaggedLineCable) {
  double value = -999999;

  value = l_.LimitConstraintSaggedLineCable();
  EXPECT_EQ(5820, helper::Round(value, 0));
}

TEST_F(LineCableSaggerTest, TensionHorizontalActual) {
  double value = -999999;

  value = l_.TensionHorizontalActual(0);
  EXPECT_EQ(5820, helper::Round(value, 0));

  value = l_.TensionHorizontalActual(1);
  EXPECT_EQ(11903, helper::Round(value, 0));

  value = l_.TensionHorizontalActual(2);
  EXPECT_EQ(4449, helper::Round(value, 0));
}

TEST_F(LineCableSaggerTest, TensionSupportActual) {
  double value = -999999;

  value = l_.TensionSupportActual(0);
  EXPECT_EQ(5857, helper::Round(value, 0));

  value = l_.TensionSupportActual(1);
  EXPECT_EQ(12000, helper::Round(value, 0));

  value = l_.TensionSupportActual(2);
  EXPECT_EQ(4497, helper::Round(value, 0));
}

TEST_F(LineCableSaggerTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
