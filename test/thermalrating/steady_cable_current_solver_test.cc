// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/steady_cable_current_solver.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"
#include "test/factory.h"

class SteadyCableCurrentSolverTest : public ::testing::Test {
 protected:
  SteadyCableCurrentSolverTest() {
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
    s_.set_temperature_cable(100);
    s_.set_units(units::UnitSystem::kImperial);
    s_.set_weather(weather_);
  }

  ~SteadyCableCurrentSolverTest() {
    delete cable_;
    delete weather_;
  }

  // allocated dependency objects
  ThermalRatingCable* cable_;
  ThermalRatingWeather* weather_;

  // test fixture
  SteadyCableCurrentSolver s_;
};

TEST_F(SteadyCableCurrentSolverTest, Current) {
  double value = -999999;

  // checks ambient cable temperature
  s_.set_temperature_cable(40);
  value = s_.Current();
  EXPECT_EQ(-541.43, helper::Round(value, 2));

  // checks a temperature that equal zero current
  s_.set_temperature_cable(51.8793256);
  value = s_.Current();
  EXPECT_EQ(0, helper::Round(value, 1));

  // check elevated cable temperature
  s_.set_temperature_cable(100);
  value = s_.Current();
  EXPECT_EQ(1028.43, helper::Round(value, 2));
}

TEST_F(SteadyCableCurrentSolverTest, StateHeatTransfer) {
  CableHeatTransferState state = s_.StateHeatTransfer();

  EXPECT_EQ(-25.00, helper::Round(state.heat_convection, 2));
  EXPECT_EQ(-11.94, helper::Round(state.heat_radiation, 2));
  EXPECT_EQ(30.09, helper::Round(state.heat_resistance, 2));
  EXPECT_EQ(6.85, helper::Round(state.heat_solar, 2));
  EXPECT_EQ(0.00, helper::Round(state.heat_storage, 2));
}

TEST_F(SteadyCableCurrentSolverTest, Validate) {
  EXPECT_TRUE(s_.Validate(true, nullptr));
}
