// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/solar_radiation_solver.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"
#include "test/factory.h"

class SolarRadiationSolverTest : public ::testing::Test {
 protected:
  SolarRadiationSolverTest() {
    s_.set_azimuth_cable(90);
    s_.set_day(161);
    s_.set_elevation(0);
    s_.set_hour(11);
    s_.set_latitude(30);
    s_.set_quality_atmosphere(AtmosphereQualityType::kClear);
    s_.set_units(units::UnitSystem::kImperial);
  }

  ~SolarRadiationSolverTest() {
  }

  // test object
  SolarRadiationSolver s_;
};

TEST_F(SolarRadiationSolverTest, AltitudeSun) {
  const double altitude = s_.AltitudeSun();
  EXPECT_EQ(74.891, helper::Round(altitude, 3));
}

TEST_F(SolarRadiationSolverTest, AzimuthSun) {
  const double azimuth = s_.AzimuthSun();
  EXPECT_EQ(113.952, helper::Round(azimuth, 3));
}

TEST_F(SolarRadiationSolverTest, Radiation) {
  const double radiation = s_.Radiation();
  EXPECT_EQ(92.690, helper::Round(radiation, 3));
}

TEST_F(SolarRadiationSolverTest, Validate) {
  EXPECT_TRUE(s_.Validate(true, nullptr));
}
