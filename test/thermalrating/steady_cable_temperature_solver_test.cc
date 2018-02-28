// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/steady_cable_temperature_solver.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"
#include "test/factory.h"

class SteadyCableTemperatureSolverTest : public ::testing::Test {
 protected:
  SteadyCableTemperatureSolverTest() {
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
    s_.set_cable(cable_);
    s_.set_current(1028.43);
    s_.set_units(units::UnitSystem::kImperial);
    s_.set_weather(weather_);
  }

  ~SteadyCableTemperatureSolverTest() {
    delete cable_;
    delete weather_;
  }

  // allocated dependency objects
  ThermalRatingCable* cable_;
  ThermalRatingWeather* weather_;

  // test fixture
  SteadyCableTemperatureSolver s_;
};

TEST_F(SteadyCableTemperatureSolverTest, TemperatureCable) {
  double value = -999999;

  // checks a current that produces ambient temperature
  s_.set_current(-541.43);
  value = s_.TemperatureCable();
  EXPECT_EQ(40.0, helper::Round(value, 1));

  // checks zero current
  s_.set_current(0);
  value = s_.TemperatureCable();
  EXPECT_EQ(51.9, helper::Round(value, 1));

  // checks high current
  s_.set_current(1028.43);
  value = s_.TemperatureCable();
  EXPECT_EQ(100.0, helper::Round(value, 1));
}

TEST_F(SteadyCableTemperatureSolverTest, StateHeatTransfer) {
  CableHeatTransferState state = s_.StateHeatTransfer();

  EXPECT_EQ(-25.00, helper::Round(state.heat_convection, 2));
  EXPECT_EQ(-11.94, helper::Round(state.heat_radiation, 2));
  EXPECT_EQ(30.09, helper::Round(state.heat_resistance, 2));
  EXPECT_EQ(6.85, helper::Round(state.heat_solar, 2));
  EXPECT_EQ(0.00, helper::Round(state.heat_storage, 2));
}

TEST_F(SteadyCableTemperatureSolverTest, Validate) {
  EXPECT_TRUE(s_.Validate(true, nullptr));
}
