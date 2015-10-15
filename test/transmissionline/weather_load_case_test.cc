// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/weather_load_case.h"

#include "gtest/gtest.h"

class WeatherLoadCaseTest : public ::testing::Test {
 protected:
  WeatherLoadCaseTest() {
    w_.description = "0-0-60";
    w_.thickness_ice = 0;
    w_.density_ice = 0;
    w_.pressure_wind = 0;
    w_.temperature_cable = 60;
  }

  WeatherLoadCase w_;
};

TEST_F(WeatherLoadCaseTest, Validate) {
  EXPECT_TRUE(w_.Validate(true, nullptr));
}
