// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_sagger.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"
#include "models/base/units.h"
#include "test/factory.h"

class LineCableSaggerTest : public ::testing::Test {
 protected:
  LineCableSaggerTest() {
    // gets line cable from factory
    linecable_ = factory::BuildLineCable();

    // builds design constraints
    CableConstraint constraint;
    WeatherLoadCase* weathercase = nullptr;

    weathercase = new WeatherLoadCase();
    weathercase->description = "0-0-60";
    weathercase->thickness_ice = 0;
    weathercase->density_ice = 0;
    weathercase->pressure_wind = 0;
    weathercase->temperature_cable = 60;
    constraint.case_weather = weathercase;
    constraint.condition = CableConditionType::kInitial;
    constraint.limit = 6000;
    constraint.type_limit = CableConstraint::LimitType::kHorizontalTension;
    constraints_.push_back(constraint);

    weathercase = new WeatherLoadCase();
    weathercase->description = "0.5-8-0";
    weathercase->thickness_ice =
        units::ConvertLength(0.5, units::LengthConversionType::kInchesToFeet);
    weathercase->density_ice = 57.3;
    weathercase->pressure_wind = 8;
    weathercase->temperature_cable = 0;
    constraint.case_weather = weathercase;
    constraint.condition = CableConditionType::kInitial;
    constraint.limit = 12000;
    constraint.type_limit = CableConstraint::LimitType::kSupportTension;
    constraints_.push_back(constraint);

    weathercase = new WeatherLoadCase();
    weathercase->description = "0-0-212";
    weathercase->thickness_ice = 0;
    weathercase->density_ice = 0;
    weathercase->pressure_wind = 0;
    weathercase->temperature_cable = 212;
    constraint.case_weather = weathercase;
    constraint.condition = CableConditionType::kLoad;
    constraint.limit = 5000;
    constraint.type_limit = CableConstraint::LimitType::kCatenaryConstant;
    constraints_.push_back(constraint);

    // builds fixture object
    l_.set_line_cable(linecable_);
    l_.set_constraints_design(&constraints_);
  }

  ~LineCableSaggerTest() {
    factory::DestroyLineCable(linecable_);

    for (auto iter = constraints_.begin(); iter != constraints_.end(); iter++) {
      CableConstraint& constraint = *iter;
      delete constraint.case_weather;
    }
  }

  // allocated dependency objects
  std::vector<CableConstraint> constraints_;
  LineCable* linecable_;

  // test object
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
  EXPECT_EQ(4448, helper::Round(value, 0));
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
