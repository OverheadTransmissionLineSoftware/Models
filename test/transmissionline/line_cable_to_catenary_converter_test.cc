// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_cable_to_catenary_converter.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"
#include "models/base/units.h"

class LineCableToCatenaryConverterTest : public ::testing::Test {
 protected:
  LineCableToCatenaryConverterTest() {

    // builds dependency object - line cable
    Cable* cable = factory::BuildCable();

    WeatherLoadCase* case_weather = new WeatherLoadCase();
    case_weather->description = "0.5-8-0";
    case_weather->thickness_ice =
        units::Convert(0.5, units::ConversionType::kInchesToFeet);
    case_weather->density_ice = 57.3;
    case_weather->pressure_wind = 8;
    case_weather->temperature_cable = 0;

    CableConstraint constraint;
    constraint.case_weather = case_weather;
    constraint.condition = CableConditionType::kInitial;
    constraint.limit = 12000;
    constraint.type_limit = CableConstraint::LimitType::kSupportTension;

    LineCable* line_cable = new LineCable();
    line_cable->cable = cable;
    line_cable->constraint = constraint;
    line_cable->spacing_attachments_ruling_span = Vector3d(1200, 0, 0);

    // builds fixture object
    l_.set_line_cable(line_cable);
  }

  LineCableToCatenaryConverter l_;
};

TEST_F(LineCableToCatenaryConverterTest, Catenary) {

  Catenary3d catenary = l_.Catenary();

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

TEST_F(LineCableToCatenaryConverterTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
