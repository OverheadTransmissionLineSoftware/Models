// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagging/stopwatch_sagger.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

class StopwatchSaggerTest : public ::testing::Test {
 protected:
  StopwatchSaggerTest() {
    // builds catenary
    Vector3d spacing_endpoints(600, 0, 0);
    Vector3d weight_unit(0, 0, 1);

    Catenary3d catenary;
    catenary.set_direction_transverse(AxisDirectionType::kPositive);
    catenary.set_spacing_endpoints(spacing_endpoints);
    catenary.set_tension_horizontal(5000);
    catenary.set_weight_unit(weight_unit);

    // builds fixture
    s_.set_catenary(catenary);
    s_.set_units(units::UnitSystem::kImperial);
  }

  ~StopwatchSaggerTest() {
  }

  // test fixture
  StopwatchSagger s_;
};

TEST_F(StopwatchSaggerTest, TimeReturn) {
  double value = -999999;

  // checks first return wave
  value = s_.TimeReturn(1);
  EXPECT_EQ(3.0, helper::Round(value, 1));

  // checks third return wave
  value = s_.TimeReturn(3);
  EXPECT_EQ(9.0, helper::Round(value, 1));

  // checks fifth return wave
  value = s_.TimeReturn(5);
  EXPECT_EQ(15.0, helper::Round(value, 1));
}

TEST_F(StopwatchSaggerTest, Validate) {
  EXPECT_TRUE(s_.Validate(true, nullptr));
}
