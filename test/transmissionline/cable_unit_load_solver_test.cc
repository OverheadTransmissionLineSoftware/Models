// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable_unit_load_solver.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"
#include "models/base/units.h"
#include "test/factory.h"

class CableUnitLoadSolverTest : public ::testing::Test {
 protected:
  CableUnitLoadSolverTest() {
    cable_ = factory::BuildCable();

    c_.set_diameter_cable(&cable_->diameter);
    c_.set_weight_unit_cable(&cable_->weight_unit);
  }

  ~CableUnitLoadSolverTest() {
    delete cable_;
  }

  // allocated dependency object
  const Cable* cable_;

  // test object
  CableUnitLoadSolver c_;
};

TEST_F(CableUnitLoadSolverTest, UnitCableLoad) {
  Vector3d load_unit;

  // ice only
  WeatherLoadCase case_ice;
  case_ice.description = "1-0-0";
  case_ice.density_ice = 57.3;
  case_ice.pressure_wind = 0;
  case_ice.temperature_cable = 0;
  case_ice.thickness_ice =
    units::ConvertLength(1, units::LengthConversionType::kInchesToFeet);

  load_unit = c_.UnitCableLoad(case_ice);
  EXPECT_EQ(0, helper::Round(load_unit.y(), 3));
  EXPECT_EQ(3.729, helper::Round(load_unit.z(), 3));

  // wind only
  WeatherLoadCase case_wind;
  case_wind.description = "0-25-0";
  case_wind.density_ice = 0;
  case_wind.pressure_wind = 25;
  case_wind.temperature_cable = 0;
  case_wind.thickness_ice = 0;

  load_unit = c_.UnitCableLoad(case_wind);
  EXPECT_EQ(2.308, helper::Round(load_unit.y(), 3));
  EXPECT_EQ(1.094, helper::Round(load_unit.z(), 3));

  // ice and wind
  WeatherLoadCase case_both;
  case_both.description = "0.5-8-0";
  case_both.density_ice = 57.3;
  case_both.pressure_wind = 8;
  case_both.temperature_cable = 0;
  case_both.thickness_ice =
    units::ConvertLength(0.5, units::LengthConversionType::kInchesToFeet);

  load_unit = c_.UnitCableLoad(case_both);
  EXPECT_EQ(1.405, helper::Round(load_unit.y(), 3));
  EXPECT_EQ(2.099, helper::Round(load_unit.z(), 3));
}

TEST_F(CableUnitLoadSolverTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
