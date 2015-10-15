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
    Cable* cable = factory::BuildCable();

    WeatherLoadCase* case_weather = new WeatherLoadCase();
    case_weather->description = "0.5-8-0";
    case_weather->thickness_ice =
        units::Convert(0.5, units::ConversionType::kInchesToFeet);
    case_weather->density_ice = 57.3;
    case_weather->pressure_wind = 8;
    case_weather->temperature_cable = 0;

    CableConstraint* constraint = new CableConstraint();
    constraint->case_weather = case_weather;
    constraint->condition = CableConditionType::kInitial;
    constraint->limit = 12000;
    constraint->type_limit = CableConstraint::LimitType::kSupportTension;

    Vector3d* spacing_attachments = new Vector3d(1200, 0, 0);

    // builds fixture object
    c_.set_cable(cable);
    c_.set_constraint(constraint);
    c_.set_spacing_attachments(spacing_attachments);
  }

  CatenarySolver c_;
};

TEST_F(CatenarySolverTest, Catenary) {
  Catenary3d catenary = c_.Catenary();

  // horizontal tension
  EXPECT_EQ(11903, helper::Round(catenary.tension_horizontal(), 0));

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
}

TEST_F(CatenarySolverTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
