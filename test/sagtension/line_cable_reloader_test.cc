// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_reloader.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"
#include "models/base/units.h"

class LineCableReloaderTest : public ::testing::Test {
 protected:
  LineCableReloaderTest() {
    // gets line cable from factory
    LineCable* line_cable = factory::BuildLineCable();

    // builds reloaded weather case
    WeatherLoadCase* weathercase_reloaded = new WeatherLoadCase();
    weathercase_reloaded->description = "0-0-60";
    weathercase_reloaded->thickness_ice = 0;
    weathercase_reloaded->density_ice = 0;
    weathercase_reloaded->pressure_wind = 0;
    weathercase_reloaded->temperature_cable = 60;

    // builds fixture object
    l_.set_line_cable(line_cable);
    l_.set_weathercase_reloaded(weathercase_reloaded);
    l_.set_condition_reloaded(CableConditionType::kInitial);
  }

  LineCableReloader l_;
};

TEST_F(LineCableReloaderTest, CatenaryReloaded) {
  Catenary3d catenary = l_.CatenaryReloaded();
  EXPECT_EQ(6000, helper::Round(catenary.tension_horizontal(), 0));

  // more reloading tests are done in the horizontal tension test
}

TEST_F(LineCableReloaderTest, LoadStretchTest) {
  double value = -999999;

  value = l_.LoadStretch(CableConditionType::kInitial);
  EXPECT_EQ(0, helper::Round(value, 0));

  value = l_.LoadStretch(CableConditionType::kLoad);
  EXPECT_EQ(12180, helper::Round(value, 0));
}

TEST_F(LineCableReloaderTest, TensionAverageComponent) {
  double value = -999999;

  value = l_.TensionAverageComponent(
      CableElongationModel::ComponentType::kCore);
  EXPECT_EQ(3174, helper::Round(value, 0));

  value = l_.TensionAverageComponent(
      CableElongationModel::ComponentType::kShell);
  EXPECT_EQ(2838, helper::Round(value, 0));
}

TEST_F(LineCableReloaderTest, TensionHorizontal) {
  double value = -999999;
  LineCable line_cable = *l_.line_cable();
  WeatherLoadCase* weathercase_reloaded = new WeatherLoadCase();

  // changes the constraint type and limit to initial condition
  line_cable = *l_.line_cable();
  line_cable.constraint.condition = CableConditionType::kInitial;
  line_cable.constraint.limit = 6000;
  l_.set_line_cable(&line_cable);

  weathercase_reloaded->description = "0-0-60";
  weathercase_reloaded->thickness_ice = 0;
  weathercase_reloaded->density_ice = 0;
  weathercase_reloaded->pressure_wind = 0;
  weathercase_reloaded->temperature_cable = 60;
  l_.set_weathercase_reloaded(weathercase_reloaded);

  // reloads at same case as constraint, checks all conditions
  l_.set_condition_reloaded(CableConditionType::kInitial);
  value = l_.TensionHorizontal();
  EXPECT_EQ(6000, helper::Round(value, 0));

  l_.set_condition_reloaded(CableConditionType::kLoad);
  value = l_.TensionHorizontal();
  EXPECT_EQ(5561, helper::Round(value, 0));

  // reloads at high temperature case, checks all conditions
  weathercase_reloaded->description = "0-0-212";
  weathercase_reloaded->thickness_ice = 0;
  weathercase_reloaded->density_ice = 0;
  weathercase_reloaded->pressure_wind = 0;
  weathercase_reloaded->temperature_cable = 212;
  l_.set_weathercase_reloaded(weathercase_reloaded);

  l_.set_condition_reloaded(CableConditionType::kInitial);
  value = l_.TensionHorizontal();
  EXPECT_EQ(4702, helper::Round(value, 0));

  l_.set_condition_reloaded(CableConditionType::kLoad);
  value = l_.TensionHorizontal();
  EXPECT_EQ(4528, helper::Round(value, 0));

  // reloads at high load case, checks all conditions
  weathercase_reloaded->description = "1-8-0";
  weathercase_reloaded->thickness_ice =
    units::Convert(1, units::ConversionType::kInchesToFeet);
  weathercase_reloaded->density_ice = 57.3;
  weathercase_reloaded->pressure_wind = 8;
  weathercase_reloaded->temperature_cable = 0;
  l_.set_weathercase_reloaded(weathercase_reloaded);

  l_.set_condition_reloaded(CableConditionType::kInitial);
  value = l_.TensionHorizontal();
  EXPECT_EQ(17126, helper::Round(value, 0));

  l_.set_condition_reloaded(CableConditionType::kLoad);
  value = l_.TensionHorizontal();
  EXPECT_EQ(17126, helper::Round(value, 0));

  // changes the constraint type and limit to stretched condition
  line_cable = *l_.line_cable();
  line_cable.constraint.condition = CableConditionType::kLoad;
  line_cable.constraint.limit = 5561.5;
  l_.set_line_cable(&line_cable);

  // reloads at same case as constraint, checks all conditions
  weathercase_reloaded->description = "0-0-60";
  weathercase_reloaded->thickness_ice = 0;
  weathercase_reloaded->density_ice = 0;
  weathercase_reloaded->pressure_wind = 0;
  weathercase_reloaded->temperature_cable = 60;
  l_.set_weathercase_reloaded(weathercase_reloaded);

  l_.set_condition_reloaded(CableConditionType::kInitial);
  value = l_.TensionHorizontal();
  EXPECT_EQ(6000, helper::Round(value, 0));

  l_.set_condition_reloaded(CableConditionType::kLoad);
  value = l_.TensionHorizontal();
  EXPECT_EQ(5562, helper::Round(value, 0));
}

TEST_F(LineCableReloaderTest, TensionHorizontalComponent) {
  double value = -999999;

  value = l_.TensionAverageComponent(
      CableElongationModel::ComponentType::kCore);
  EXPECT_EQ(3174, helper::Round(value, 0));

  value = l_.TensionAverageComponent(
      CableElongationModel::ComponentType::kShell);
  EXPECT_EQ(2838, helper::Round(value, 0));
}

TEST_F(LineCableReloaderTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
