// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/line_cable_to_catenary_cable_converter.h"

#include "../test/factory.h"
#include "base/helper.h"
#include "base/units.h"
#include "gtest/gtest.h"

class LineCableToCatenaryCableConverterTest : public ::testing::Test {
 protected:
  LineCableToCatenaryCableConverterTest() {

    // gets line cable from factory
    LineCable* line_cable = factory::BuildLineCable();

    // builds a stretch weather case
    WeatherLoadCase* case_stretch = new WeatherLoadCase();
    case_stretch->description = "0.5-8-0";
    case_stretch->thickness_ice =
        units::Convert(0.5, units::ConversionType::kInchesToFeet);
    case_stretch->density_ice = 57.3;
    case_stretch->pressure_wind = 8;
    case_stretch->temperature_cable = 0;

    // builds fixture object
    l_.set_line_cable(line_cable);
    l_.set_case_stretch(case_stretch);
    l_.set_type_stretch(CableConditionType::kLoad);
  }

  LineCableToCatenaryCableConverter l_;
};

TEST_F(LineCableToCatenaryCableConverterTest, CatenaryCableTest) {

  // tests cable constraint with initial condition
  CatenaryCable cc = l_.GetCatenaryCable();
  EXPECT_EQ(6000, helper::Round(cc.tension_horizontal(), 0));

  CableState state = *cc.state();
  EXPECT_EQ(0, state.load_stretch);
  EXPECT_EQ(0, state.temperature_stretch);
  EXPECT_EQ(60, state.temperature);

  // tests cable constraint with load condition
  LineCable line_cable;
  line_cable.cable = l_.line_cable()->cable;
  line_cable.constraint = l_.line_cable()->constraint;
  line_cable.spacing_attachments_ruling_span =
      l_.line_cable()->spacing_attachments_ruling_span;

  line_cable.constraint.condition = CableConditionType::kLoad;
  l_.set_line_cable(&line_cable);

  cc = l_.GetCatenaryCable();
  state = *cc.state();
  EXPECT_EQ(12912, helper::Round(state.load_stretch, 0));
}

TEST_F(LineCableToCatenaryCableConverterTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
