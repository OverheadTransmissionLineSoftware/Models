// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/transient_cable_temperature_solver.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class TransientCableTemperatureSolverTest : public ::testing::Test {
 protected:
  TransientCableTemperatureSolverTest() {
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
    t_.set_cable(cable_);
    t_.set_current_steady(500);
    t_.set_current_step(1000);
    t_.set_duration(1200);
    t_.set_units(units::UnitSystem::kImperial);
    t_.set_weather(weather_);
  }

  ~TransientCableTemperatureSolverTest() {
    delete cable_;
    delete weather_;
  }

  // allocated dependency objects
  ThermalRatingCable* cable_;
  ThermalRatingWeather* weather_;

  // test fixture
  TransientCableTemperatureSolver t_;
};

TEST_F(TransientCableTemperatureSolverTest, PointsTemperature) {
  const std::list<TemperaturePoint>* points = nullptr;
  TemperaturePoint point;

  // checks increasing temperature
  points = t_.PointsTemperature();

  point = points->front();
  EXPECT_EQ(0, point.time);
  EXPECT_EQ(62.5, helper::Round(point.temperature, 1));

  point = *std::next(points->cbegin(), 600);
  EXPECT_EQ(600, point.time);
  EXPECT_EQ(82.2, helper::Round(point.temperature, 1));

  point = points->back();
  EXPECT_EQ(1200, point.time);
  EXPECT_EQ(90.8, helper::Round(point.temperature, 1));

  // checks decreasing temperature
  t_.set_current_steady(1000);
  t_.set_current_step(500);
  points = t_.PointsTemperature();

  point = points->front();
  EXPECT_EQ(0, point.time);
  EXPECT_EQ(97.2, helper::Round(point.temperature, 1));

  point = *std::next(points->cbegin(), 600);
  EXPECT_EQ(600, point.time);
  EXPECT_EQ(76.6, helper::Round(point.temperature, 1));

  point = points->back();
  EXPECT_EQ(1200, point.time);
  EXPECT_EQ(68.3, helper::Round(point.temperature, 1));
}

TEST_F(TransientCableTemperatureSolverTest, Validate) {
  EXPECT_TRUE(t_.Validate(true, nullptr));
}
