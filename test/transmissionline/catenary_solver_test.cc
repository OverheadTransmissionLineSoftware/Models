// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/catenary_solver.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"
#include "models/base/units.h"

class CatenarySolverTest : public ::testing::Test {
 protected:
  CatenarySolverTest() {
    // builds dependency object - line cable
    cable_ = factory::BuildCable();

    weathercase_ = new WeatherLoadCase();
    weathercase_->description = "0.5-8-0";
    weathercase_->thickness_ice =
        units::ConvertLength(0.5, units::LengthConversionType::kInchesToFeet);
    weathercase_->density_ice = 57.3;
    weathercase_->pressure_wind = 8;
    weathercase_->temperature_cable = 0;

    constraint_ = new CableConstraint();
    constraint_->case_weather = weathercase_;
    constraint_->condition = CableConditionType::kInitial;
    constraint_->limit = 4000;
    constraint_->type_limit = CableConstraint::LimitType::kCatenaryConstant;

    spacing_attachments_ = new Vector3d(1200, 0, 0);

    // builds fixture object
    c_.set_cable(cable_);
    c_.set_constraint(constraint_);
    c_.set_spacing_attachments(spacing_attachments_);
  }

  ~CatenarySolverTest() {
    delete cable_;
    delete constraint_;
    delete spacing_attachments_;
    delete weathercase_;
  }

  // allocated dependency objects
  const Cable* cable_;
  CableConstraint* constraint_;
  Vector3d* spacing_attachments_;
  WeatherLoadCase* weathercase_;

  // test object
  CatenarySolver c_;
};

TEST_F(CatenarySolverTest, Catenary) {
  // gets catenary and saves values for comparison
  Catenary3d catenary = c_.Catenary();

  // weight
  Vector3d weight_unit = catenary.weight_unit();
  EXPECT_EQ(0, helper::Round(weight_unit.x(), 3));
  EXPECT_EQ(1.405, helper::Round(weight_unit.y(), 3));
  EXPECT_EQ(2.099, helper::Round(weight_unit.z(), 3));

  // spacing
  Vector3d spacing = catenary.spacing_endpoints();
  EXPECT_EQ(1200, helper::Round(spacing.x(), 0));
  EXPECT_EQ(0, helper::Round(spacing.y(), 0));
  EXPECT_EQ(0, helper::Round(spacing.z(), 0));

  // horizontal tension - from catenary constant type constraint
  EXPECT_EQ(10104.3, helper::Round(catenary.tension_horizontal(), 1));

  // horizontal tension - from horizontal tension type constraint
  constraint_->limit = 10104.3;
  constraint_->type_limit = CableConstraint::LimitType::kHorizontalTension;
  c_.set_constraint(constraint_);
  catenary = c_.Catenary();
  EXPECT_EQ(10104.3, helper::Round(catenary.tension_horizontal(), 1));

  // horizontal tension - from length type constraint
  constraint_->limit = 1204.5051;
  constraint_->type_limit = CableConstraint::LimitType::kLength;
  c_.set_constraint(constraint_);
  catenary = c_.Catenary();
  EXPECT_EQ(10104.3, helper::Round(catenary.tension_horizontal(), 1));

  // horizontal tension - from sag type constraint
  constraint_->limit = 45.0844;
  constraint_->type_limit = CableConstraint::LimitType::kSag;
  c_.set_constraint(constraint_);
  catenary = c_.Catenary();
  EXPECT_EQ(10104.3, helper::Round(catenary.tension_horizontal(), 1));

  // horizontal tension - from support tension type constraint
  constraint_->limit = 10218.2;
  constraint_->type_limit = CableConstraint::LimitType::kSupportTension;
  c_.set_constraint(constraint_);
  catenary = c_.Catenary();
  EXPECT_EQ(10104.3, helper::Round(catenary.tension_horizontal(), 1));
}

TEST_F(CatenarySolverTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
