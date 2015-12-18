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
    cable_ = factory::BuildSagTensionCable();

    weathercase_ = new WeatherLoadCase();
    weathercase_->description = "0-0-60";
    weathercase_->thickness_ice = 0;
    weathercase_->density_ice = 0;
    weathercase_->pressure_wind = 0;
    weathercase_->temperature_cable = 60;

    constraint_ = new CableConstraint();
    constraint_->case_weather = weathercase_;
    constraint_->condition = CableConditionType::kInitial;
    constraint_->limit = 6000;
    constraint_->type_limit = CableConstraint::LimitType::kHorizontalTension;

    spacing_attachments_ = new Vector3d(1200, 0, 0);

    // builds a stretch weather case
    weathercase_stretch_ = new WeatherLoadCase();
    weathercase_stretch_->description = "0.5-8-0";
    weathercase_stretch_->thickness_ice =
        units::Convert(0.5, units::ConversionType::kInchesToFeet);
    weathercase_stretch_->density_ice = 57.3;
    weathercase_stretch_->pressure_wind = 8;
    weathercase_stretch_->temperature_cable = 0;

    // builds fixture object
    c_.set_cable(cable_);
    c_.set_constraint(constraint_);
    c_.set_spacing_attachments(spacing_attachments_);
    c_.set_weathercase_stretch_creep(weathercase_stretch_);
    c_.set_weathercase_stretch_load(weathercase_stretch_);
  }

  ~CatenaryCableSolverTest() {
    factory::DestroySagTensionCable(cable_);
    delete constraint_;
    delete spacing_attachments_;
    delete weathercase_;
    delete weathercase_stretch_;
  }

  // allocated dependency objects
  SagTensionCable* cable_;
  CableConstraint* constraint_;
  Vector3d* spacing_attachments_;
  WeatherLoadCase* weathercase_;
  WeatherLoadCase* weathercase_stretch_;

  // test object
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
  constraint_->condition = CableConditionType::kLoad;
  c_.set_constraint(constraint_);

  cc = c_.GetCatenaryCable();
  state = *cc.state();
  EXPECT_EQ(12912, helper::Round(state.load_stretch, 0));
}

TEST_F(CatenaryCableSolverTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
