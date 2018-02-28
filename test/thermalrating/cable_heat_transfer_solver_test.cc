// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/cable_heat_transfer_solver.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CableHeatTransferSolverTest : public ::testing::Test {
 protected:
  CableHeatTransferSolverTest() {
    // gets thermal rating cable from factory
    cable_ = factory::BuildThermalRatingCable();

    // builds weather
    weather_ = new ThermalRatingWeather();
    weather_->angle_wind = 90;
    weather_->elevation = 0;
    weather_->radiation_solar = 92.69;
    weather_->speed_wind = 2;
    weather_->temperature_air = 40;

    // builds fixture
    c_.set_cable(cable_);
    c_.set_units(units::UnitSystem::kImperial);
    c_.set_weather(weather_);
  }

  ~CableHeatTransferSolverTest() {
    delete cable_;
    delete weather_;
  }

  // allocated dependency objects
  ThermalRatingCable* cable_;
  ThermalRatingWeather* weather_;

  // test fixture
  CableHeatTransferSolver c_;
};

TEST_F(CableHeatTransferSolverTest, HeatConvection) {
  const double heat_convection = c_.HeatConvection(100);
  EXPECT_EQ(-24.997, helper::Round(heat_convection, 3));
}

TEST_F(CableHeatTransferSolverTest, HeatRadiation) {
  const double heat_radiation = c_.HeatRadiation(100);
  EXPECT_EQ(-11.937, helper::Round(heat_radiation, 3));
}

TEST_F(CableHeatTransferSolverTest, HeatResistance) {
  const double heat_resistance = c_.HeatResistance(100, 1028.43);
  EXPECT_EQ(30.087, helper::Round(heat_resistance, 3));
}

TEST_F(CableHeatTransferSolverTest, HeatSolar) {
  const double heat_solar = c_.HeatSolar();
  EXPECT_EQ(6.847, helper::Round(heat_solar, 3));
}

TEST_F(CableHeatTransferSolverTest, HeatStored) {
  double value = -999999;

  value = c_.HeatStored(36.934, -36.934);
  EXPECT_EQ(0.000, helper::Round(value, 3));

  value = c_.HeatStored(20.0, -10.0);
  EXPECT_EQ(10.000, helper::Round(value, 3));

  value = c_.HeatStored(10.0, -20.0);
  EXPECT_EQ(-10.000, helper::Round(value, 3));
}
