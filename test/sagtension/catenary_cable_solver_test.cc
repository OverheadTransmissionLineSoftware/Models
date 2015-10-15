// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_solver.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"
#include "models/base/units.h"

class CatenaryCableSolverTest : public ::testing::Test {
 protected:
  CatenaryCableSolverTest() {
    // builds dependency object - line cable
    SagTensionCable* cable = factory::BuildSagTensionCable();

    WeatherLoadCase* weathercase = new WeatherLoadCase();
    weathercase->description = "0-0-60";
    weathercase->thickness_ice = 0;
    weathercase->density_ice = 0;
    weathercase->pressure_wind = 0;
    weathercase->temperature_cable = 60;

    CableConstraint* constraint = new CableConstraint();
    constraint->case_weather = weathercase;
    constraint->condition = CableConditionType::kInitial;
    constraint->limit = 6000;
    constraint->type_limit = CableConstraint::LimitType::kHorizontalTension;

    Vector3d* spacing_attachments = new Vector3d(1200, 0, 0);

    // builds a stretch weather case
    WeatherLoadCase* weathercase_stretch = new WeatherLoadCase();
    weathercase_stretch->description = "0.5-8-0";
    weathercase_stretch->thickness_ice =
        units::Convert(0.5, units::ConversionType::kInchesToFeet);
    weathercase_stretch->density_ice = 57.3;
    weathercase_stretch->pressure_wind = 8;
    weathercase_stretch->temperature_cable = 0;

    // builds fixture object
    c_.set_cable(cable);
    c_.set_constraint(constraint);
    c_.set_spacing_attachments(spacing_attachments);
    c_.set_weathercase_stretch_creep(weathercase_stretch);
    c_.set_weathercase_stretch_load(weathercase_stretch);
  }

  CatenaryCableSolver c_;
};

TEST_F(CatenaryCableSolverTest, CatenaryCableTest) {
  // tests cable constraint with initial condition
  CatenaryCable cc = c_.GetCatenaryCable();
  EXPECT_EQ(6000, helper::Round(cc.tension_horizontal(), 0));

  CableState state = *cc.state();
  EXPECT_EQ(0, state.load_stretch);
  EXPECT_EQ(0, state.temperature_stretch);
  EXPECT_EQ(60, state.temperature);

  // tests cable constraint with load condition
  CableConstraint constraint = *c_.constraint();
  constraint.condition = CableConditionType::kLoad;
  c_.set_constraint(&constraint);

  cc = c_.GetCatenaryCable();
  state = *cc.state();
  EXPECT_EQ(12912, helper::Round(state.load_stretch, 0));
}

TEST_F(CatenaryCableSolverTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
