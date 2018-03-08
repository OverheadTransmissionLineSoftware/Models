// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/units.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

// The starting value should be a primary base unit (feet/meters, etc). If
// converting between unit systems, the test should start with metric.
TEST(Units, ConvertAngle) {
  double value1 = 0;
  double value2 = 0;

  // converts degrees to radians and back again
  value1 = 180;
  value2 = units::ConvertAngle(value1,
                               units::AngleConversionType::kDegreesToRadians);
  EXPECT_EQ(units::kPi, value2);

  value1 = units::ConvertAngle(value2,
                               units::AngleConversionType::kRadiansToDegrees);
  EXPECT_EQ(180, value1);
}

TEST(Units, ConvertForce) {
  double value1 = 0;
  double value2 = 0;

  // converts newtons to pounds and back again
  value1 = 100;
  value2 = units::ConvertForce(value1,
                               units::ForceConversionType::kNewtonsToPounds);
  EXPECT_EQ(22.5, helper::Round(value2, 1));

  value1 = units::ConvertForce(value2,
                               units::ForceConversionType::kPoundsToNewtons);
  EXPECT_EQ(100, helper::Round(value1, 1));
}

TEST(Units, ConvertLength) {
  double value1 = 0;
  double value2 = 0;

  // converts feet to inches and back again
  value1 = 2;
  value2 = units::ConvertLength(value1,
                          units::LengthConversionType::kFeetToInches);
  EXPECT_EQ(24, helper::Round(value2, 0));

  value1 = units::ConvertLength(value2,
                          units::LengthConversionType::kInchesToFeet);
  EXPECT_EQ(2, helper::Round(value1, 0));

  // converts feet to miles and back again
  value1 = 2;
  value2 = units::ConvertLength(value1,
                          units::LengthConversionType::kFeetToMiles);
  EXPECT_EQ(0.000379, helper::Round(value2, 6));

  value1 = units::ConvertLength(value2,
                          units::LengthConversionType::kMilesToFeet);
  EXPECT_EQ(2, helper::Round(value1, 0));

  // converts meters to centimeters and back again
  value1 = 10;
  value2 = units::ConvertLength(value1,
                          units::LengthConversionType::kMetersToCentimeters);
  EXPECT_EQ(1000, helper::Round(value2, 0));

  value1 = units::ConvertLength(value2,
                          units::LengthConversionType::kCentimetersToMeters);
  EXPECT_EQ(10, helper::Round(value1, 0));

  // converts meters to feet and back again
  value1 = 10;
  value2 = units::ConvertLength(value1,
                          units::LengthConversionType::kMetersToFeet);
  EXPECT_EQ(32.8, helper::Round(value2, 1));

  value1 = units::ConvertLength(value2,
                          units::LengthConversionType::kFeetToMeters);
  EXPECT_EQ(10, helper::Round(value1, 1));

  // converts meters to kilometers and back again
  value1 = 10;
  value2 = units::ConvertLength(value1,
                          units::LengthConversionType::kMetersToKilometers);
  EXPECT_EQ(0.01, helper::Round(value2, 2));

  value1 = units::ConvertLength(value2,
                          units::LengthConversionType::kKilometersToMeters);
  EXPECT_EQ(10, helper::Round(value1, 0));

  // converts meters to millimeters and back again
  value1 = 10;
  value2 = units::ConvertLength(value1,
                          units::LengthConversionType::kMetersToMillimeters);
  EXPECT_EQ(10000, helper::Round(value2, 0));

  value1 = units::ConvertLength(value2,
                          units::LengthConversionType::kMillimetersToMeters);
  EXPECT_EQ(10, helper::Round(value1, 0));
}

TEST(Units, ConvertStress) {
  double value1 = 0;
  double value2 = 0;

  // converts pascal to megapascal and back again
  value1 = 1000;
  value2 = units::ConvertStress(
    value1,
    units::StressConversionType::kPascalToMegaPascal);
  EXPECT_EQ(0.001, helper::Round(value2, 3));

  value1 = units::ConvertStress(
    value2,
    units::StressConversionType::kMegaPascalToPascal);
  EXPECT_EQ(1000, helper::Round(value1, 0));

  // converts pascal to psf and back again
  value1 = 100;
  value2 = units::ConvertStress(value1,
                                units::StressConversionType::kPascalToPsf);
  EXPECT_EQ(2.1, helper::Round(value2, 1));

  value1 = units::ConvertStress(value2,
                                units::StressConversionType::kPsfToPascal);
  EXPECT_EQ(100, helper::Round(value1, 0));

  // converts psi to psf and back again
  value1 = 100;
  value2 = units::ConvertStress(value1,
                                units::StressConversionType::kPsiToPsf);
  EXPECT_EQ(14400, helper::Round(value2, 0));

  value1 = units::ConvertStress(value2,
                                units::StressConversionType::kPsfToPsi);
  EXPECT_EQ(100, helper::Round(value1, 0));
}

TEST(Units, ConvertTemperature) {
  double value1 = 0;
  double value2 = 0;

  // converts kelvin to rankine and back again
  value1 = 100;
  value2 = units::ConvertTemperature(
      value1,
      units::TemperatureConversionType::kKelvinToRankine);
  EXPECT_EQ(180.00, helper::Round(value2, 2));

  value1 = units::ConvertTemperature(
      value2,
      units::TemperatureConversionType::kRankineToKelvin);
  EXPECT_EQ(100.00, helper::Round(value1, 2));

  // converts kelvin to celsius and back again
  value1 = 0;
  value2 = units::ConvertTemperature(
      value1,
      units::TemperatureConversionType::kKelvinToCelsius);
  EXPECT_EQ(-273.15, helper::Round(value2, 2));

  value1 = units::ConvertTemperature(
      value2,
      units::TemperatureConversionType::kCelsiusToKelvin);
  EXPECT_EQ(0.00, helper::Round(value1, 2));

  // converts rankine to fahrenheit and back again
  value1 = 0;
  value2 = units::ConvertTemperature(
      value1,
      units::TemperatureConversionType::kRankineToFahrenheit);
  EXPECT_EQ(-459.67, helper::Round(value2, 2));

  value1 = units::ConvertTemperature(
      value2,
      units::TemperatureConversionType::kFahrenheitToRankine);
  EXPECT_EQ(0.00, helper::Round(value1, 2));

  // converts celsius to fahrenheit and back again
  value1 = 20;
  value2 = units::ConvertTemperature(
      value1,
      units::TemperatureConversionType::kCelsiusToFahrenheit);
  EXPECT_EQ(68.00, helper::Round(value2, 2));

  value1 = units::ConvertTemperature(
      value2,
      units::TemperatureConversionType::kFahrenheitToCelsius);
  EXPECT_EQ(20.00, helper::Round(value1, 2));
}
