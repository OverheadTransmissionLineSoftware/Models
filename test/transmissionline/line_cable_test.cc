// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/line_cable.h"

#include "../test/factory.h"
#include "gtest/gtest.h"

class LineCableTest : public ::testing::Test {
 protected:
  LineCableTest() {

    // gets cable
    Cable* cable = factory::BuildCable();

    // builds dependency object - cable constraint
    WeatherLoadCase* case_weather = new WeatherLoadCase();
    case_weather->description = "0-0-60";
    case_weather->thickness_ice = 0;
    case_weather->density_ice = 0;
    case_weather->pressure_wind = 0;
    case_weather->temperature_cable = 60;

    CableConstraint constraint;
    constraint.case_weather = case_weather;
    constraint.condition = CableConditionType::kInitial;
    constraint.limit = 6000;
    constraint.type_limit = CableConstraint::LimitType::kHorizontalTension;
    
    l_.cable = cable;
    l_.constraint = constraint;
    l_.spacing_attachments_ruling_span = Vector3d(1200, 0, 0);
  }

  LineCable l_;
};

TEST_F(LineCableTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}