// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/line_cable_reloader.h"

#include "../test/factory.h"
#include "base/helper.h"
#include "base/units.h"
#include "gtest/gtest.h"

class LineCableReloaderTest : public ::testing::Test {
 protected:
  LineCableReloaderTest() {

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

    // builds reloaded weather case
    WeatherLoadCase* case_reloaded = new WeatherLoadCase();
    case_reloaded->description = "0-0-60";
    case_reloaded->thickness_ice = 0;
    case_reloaded->density_ice = 0;
    case_reloaded->pressure_wind = 0;
    case_reloaded->temperature_cable = 60;

    // builds fixture object
    l_.set_line_cable(line_cable);
    l_.set_case_stretch(case_stretch);
    l_.set_case_reloaded(case_reloaded);
    l_.set_condition_reloaded(CableConditionType::kInitial);
    l_.set_type_stretch(CableConditionType::kLoad);
  }

  LineCableReloader l_;
};

TEST_F(LineCableReloaderTest, CatenaryReloaded) {

  Catenary3d catenary = l_.CatenaryReloaded();
  EXPECT_EQ(6000, helper::Round(catenary.tension_horizontal(), 0));

  // more reloading tests are done in the horizontal tension test
}

TEST_F(LineCableReloaderTest, LoadStretchTest) {

  // 'initial' reloaded condition
  l_.set_condition_reloaded(CableConditionType::kInitial);
  EXPECT_EQ(0, helper::Round(l_.LoadStretch(), 0));

  // 'load' reloaded condition
  l_.set_condition_reloaded(CableConditionType::kLoad);
  EXPECT_EQ(12180, helper::Round(l_.LoadStretch(), 0));

  // 'load' reloaded condition, but stretch case ice thickness is doubled
  WeatherLoadCase case_stretch = *l_.case_stretch();
  case_stretch.thickness_ice =
      units::Convert(1, units::ConversionType::kInchesToFeet);
  l_.set_case_stretch(&case_stretch);
  EXPECT_EQ(17192, helper::Round(l_.LoadStretch(), 0));
}

TEST_F(LineCableReloaderTest, TensionAverageComponent) {

  EXPECT_EQ(3174, helper::Round(
      l_.TensionAverageComponent(CableElongationModel::ComponentType::kCore),
      0));

  EXPECT_EQ(2838, helper::Round(
      l_.TensionAverageComponent(CableElongationModel::ComponentType::kShell),
      0));
}

TEST_F(LineCableReloaderTest, TensionHorizontal) {

  LineCable line_cable = *l_.line_cable();
  WeatherLoadCase* case_reloaded = new WeatherLoadCase();

  // changes the constraint type and limit to initial condition
  line_cable = *l_.line_cable();
  line_cable.constraint.condition = CableConditionType::kInitial;
  line_cable.constraint.limit = 6000;
  l_.set_line_cable(&line_cable);

  case_reloaded->description = "0-0-60";
  case_reloaded->thickness_ice = 0;
  case_reloaded->density_ice = 0;
  case_reloaded->pressure_wind = 0;
  case_reloaded->temperature_cable = 60;
  l_.set_case_reloaded(case_reloaded);

  // reloads at same case as constraint, checks all conditions
  l_.set_condition_reloaded(CableConditionType::kInitial);
  EXPECT_EQ(6000, helper::Round(l_.TensionHorizontal(), 0));
  l_.set_condition_reloaded(CableConditionType::kLoad);
  EXPECT_EQ(5561, helper::Round(l_.TensionHorizontal(), 0));

  // reloads at high temperature case, checks all conditions
  case_reloaded->description = "0-0-212";
  case_reloaded->thickness_ice = 0;
  case_reloaded->density_ice = 0;
  case_reloaded->pressure_wind = 0;
  case_reloaded->temperature_cable = 212;
  l_.set_case_reloaded(case_reloaded);

  l_.set_condition_reloaded(CableConditionType::kInitial);
  EXPECT_EQ(4702, helper::Round(l_.TensionHorizontal(), 0));
  l_.set_condition_reloaded(CableConditionType::kLoad);
  EXPECT_EQ(4528, helper::Round(l_.TensionHorizontal(), 0));

  // reloads at high load case, checks all conditions
  case_reloaded->description = "1-8-0";
  case_reloaded->thickness_ice =
    units::Convert(1, units::ConversionType::kInchesToFeet);
  case_reloaded->density_ice = 57.3;
  case_reloaded->pressure_wind = 8;
  case_reloaded->temperature_cable = 0;
  l_.set_case_reloaded(case_reloaded);

  l_.set_condition_reloaded(CableConditionType::kInitial);
  EXPECT_EQ(17126, helper::Round(l_.TensionHorizontal(), 0));
  l_.set_condition_reloaded(CableConditionType::kLoad);
  EXPECT_EQ(17126, helper::Round(l_.TensionHorizontal(), 0));

  // changes the constraint type and limit to stretched condition
  line_cable = *l_.line_cable();
  line_cable.constraint.condition = CableConditionType::kLoad;
  line_cable.constraint.limit = 5561.5;
  l_.set_line_cable(&line_cable);

  // reloads at same case as constraint, checks all conditions
  case_reloaded->description = "0-0-60";
  case_reloaded->thickness_ice = 0;
  case_reloaded->density_ice = 0;
  case_reloaded->pressure_wind = 0;
  case_reloaded->temperature_cable = 60;
  l_.set_case_reloaded(case_reloaded);

  l_.set_condition_reloaded(CableConditionType::kInitial);
  EXPECT_EQ(6000, helper::Round(l_.TensionHorizontal(), 0));
  l_.set_condition_reloaded(CableConditionType::kLoad);
  EXPECT_EQ(5561, helper::Round(l_.TensionHorizontal(), 0));
}

TEST_F(LineCableReloaderTest, TensionHorizontalComponent) {

  EXPECT_EQ(3174, helper::Round(
      l_.TensionAverageComponent(CableElongationModel::ComponentType::kCore),
      0));

  EXPECT_EQ(2838, helper::Round(
      l_.TensionAverageComponent(CableElongationModel::ComponentType::kShell),
      0));
}

TEST_F(LineCableReloaderTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
