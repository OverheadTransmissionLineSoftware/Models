// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/units.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

TEST(Units, Convert) {

  // converts degrees to radians and back again
  double degrees = 180;
  const double radians = units::Convert(
      degrees,
      units::ConversionType::kDegreesToRadians);
  EXPECT_EQ(units::kPi, radians);

  degrees = units::Convert(
      radians,
      units::ConversionType::kRadiansToDegrees);
  EXPECT_EQ(180, degrees);

  // converts inches to feet and back again
  double inches = 24;
  const double feet = units::Convert(
      inches,
      units::ConversionType::kInchesToFeet);
  EXPECT_EQ(2, helper::Round(feet, 0));

  inches = units::Convert(
      feet,
      units::ConversionType::kFeetToInches);
  EXPECT_EQ(24, helper::Round(inches, 0));
}
