// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagging/dyno_sagger.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

class DynoSaggerTest : public ::testing::Test {
 protected:
  DynoSaggerTest() {
    // builds catenary
    Vector3d spacing_endpoints(2000, 0, 100);
    Vector3d weight_unit(0, 0, 1);

    Catenary3d catenary;
    catenary.set_direction_transverse(AxisDirectionType::kPositive);
    catenary.set_spacing_endpoints(spacing_endpoints);
    catenary.set_tension_horizontal(5000);
    catenary.set_weight_unit(weight_unit);

    // builds fixture
    d_.set_catenary(catenary);
    d_.set_location(DynoSagger::SpanEndLocation::kBack);
  }

  ~DynoSaggerTest() {
  }

  // test fixture
  DynoSagger d_;
};

TEST_F(DynoSaggerTest, Tension) {
  double value = -999999;

  // checks back location
  d_.set_location(DynoSagger::SpanEndLocation::kBack);
  value = d_.Tension();
  EXPECT_EQ(5056.6, helper::Round(value, 1));

  // checks ahead location
  d_.set_location(DynoSagger::SpanEndLocation::kAhead);
  value = d_.Tension();
  EXPECT_EQ(5156.6, helper::Round(value, 1));
}

TEST_F(DynoSaggerTest, Validate) {
  EXPECT_TRUE(d_.Validate(true, nullptr));
}
