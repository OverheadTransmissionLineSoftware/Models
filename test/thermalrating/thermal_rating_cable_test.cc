// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/thermal_rating_cable.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"
#include "test/factory.h"

class ThermalRatingCableTest : public ::testing::Test {
 protected:
  ThermalRatingCableTest() {
    cable_ = factory::BuildCable();
    t_.set_cable_base(cable_);
  }

  ~ThermalRatingCableTest() {
    delete cable_;
  }

  // dependency object
  Cable* cable_;

  // test fixture
  ThermalRatingCable t_;
};

TEST_F(ThermalRatingCableTest, Resistance) {
  double temperature = 0;
  double resistance = t_.Resistance(temperature);
  EXPECT_EQ(0.00001996, helper::Round(resistance, 8));

  temperature = 50;
  resistance = t_.Resistance(temperature);
  EXPECT_EQ(0.00002420, helper::Round(resistance, 8));

  temperature = 100;
  resistance = t_.Resistance(temperature);
  EXPECT_EQ(0.00002845, helper::Round(resistance, 8));
}

TEST_F(ThermalRatingCableTest, Validate) {
  EXPECT_TRUE(t_.Validate(true, nullptr));
}
