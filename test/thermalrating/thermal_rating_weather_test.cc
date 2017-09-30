// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/thermal_rating_weather.h"

#include "gtest/gtest.h"

class ThermalRatingWeatherTest : public ::testing::Test {
 protected:
  ThermalRatingWeatherTest() {
    // builds fixture object
    t_.angle_wind = 90;
    t_.elevation = 0;
    t_.radiation_solar = 92.69;
    t_.speed_wind = 2;
    t_.temperature_air = 40;
  }

  ~ThermalRatingWeatherTest() {
  }

  // test object
  ThermalRatingWeather t_;
};

TEST_F(ThermalRatingWeatherTest, Validate) {
  EXPECT_TRUE(t_.Validate(true, nullptr));
}
