// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/catenary.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

class Catenary2dTest : public ::testing::Test {
 protected:
  Catenary2dTest() {
    Vector2d spacing_endpoints(1000, 0);

    c_.set_spacing_endpoints(spacing_endpoints);
    c_.set_tension_horizontal(1000);
    c_.set_weight_unit(0.5);
  }

  // test object
  Catenary2d c_;
};

TEST_F(Catenary2dTest, Constant) {
  EXPECT_EQ(2000, helper::Round(c_.Constant(), 0));
}

TEST_F(Catenary2dTest, ConstantMinimum) {
  EXPECT_EQ(500, c_.ConstantMinimum(1000));
  EXPECT_EQ(1000, c_.ConstantMinimum(2000));
}

TEST_F(Catenary2dTest, Coordinate) {
  Point2d<double> coord;

  // unshfted points
  coord = c_.Coordinate(0, false);
  EXPECT_EQ(-500, helper::Round(coord.x, 1));
  EXPECT_EQ(62.8, helper::Round(coord.y, 1));

  coord = c_.Coordinate(0.5, false);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));

  coord = c_.Coordinate(1, false);
  EXPECT_EQ(500, helper::Round(coord.x, 1));
  EXPECT_EQ(62.8, helper::Round(coord.y, 1));

  // shifted point
  coord = c_.Coordinate(0, true);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));

  // inclined geometry - origin between end points
  Vector2d spacing(1000, 100);
  c_.set_spacing_endpoints(spacing);

  coord = c_.Coordinate(0, true);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));

  coord = c_.Coordinate(0.5, true);
  EXPECT_EQ(506.2, helper::Round(coord.x, 1));
  EXPECT_EQ(-12.5, helper::Round(coord.y, 1));

  coord = c_.Coordinate(1, true);
  EXPECT_EQ(1000, helper::Round(coord.x, 1));
  EXPECT_EQ(100, helper::Round(coord.y, 1));

  // inclined geometry - origin beyond end points
  spacing = Vector2d(1000, 300);
  c_.set_spacing_endpoints(spacing);

  coord = c_.Coordinate(0, true);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));

  coord = c_.Coordinate(0.5, true);
  EXPECT_EQ(517.9, helper::Round(coord.x, 1));
  EXPECT_EQ(89.8, helper::Round(coord.y, 1));

  coord = c_.Coordinate(1, true);
  EXPECT_EQ(1000, helper::Round(coord.x, 1));
  EXPECT_EQ(300, helper::Round(coord.y, 1));

  // declined geometry - origin between end points
  spacing = Vector2d(1000, -100);
  c_.set_spacing_endpoints(spacing);

  coord = c_.Coordinate(0, true);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));

  coord = c_.Coordinate(0.5, true);
  EXPECT_EQ(493.8, helper::Round(coord.x, 1));
  EXPECT_EQ(-112.5, helper::Round(coord.y, 1));

  coord = c_.Coordinate(1, true);
  EXPECT_EQ(1000, helper::Round(coord.x, 1));
  EXPECT_EQ(-100, helper::Round(coord.y, 1));

  // declined geometry - origin beyond end points
  spacing = Vector2d(1000, -300);
  c_.set_spacing_endpoints(spacing);

  coord = c_.Coordinate(0, true);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));

  coord = c_.Coordinate(0.5, true);
  EXPECT_EQ(482.1, helper::Round(coord.x, 1));
  EXPECT_EQ(-210.2, helper::Round(coord.y, 1));

  coord = c_.Coordinate(1, true);
  EXPECT_EQ(1000, helper::Round(coord.x, 1));
  EXPECT_EQ(-300, helper::Round(coord.y, 1));
}

TEST_F(Catenary2dTest, CoordinateChord) {
  Point2d<double> coord;

  // changes catenary spacing to inclined
  Vector2d spacing(1000, 100);
  c_.set_spacing_endpoints(spacing);

  // unshfted points
  coord = c_.CoordinateChord(0, false);
  EXPECT_EQ(-302.4, helper::Round(coord.x, 1));
  EXPECT_EQ(22.9, helper::Round(coord.y, 1));

  coord = c_.CoordinateChord(0.5, false);
  EXPECT_EQ(203.8, helper::Round(coord.x, 1));
  EXPECT_EQ(73.5, helper::Round(coord.y, 1));

  coord = c_.CoordinateChord(1, false);
  EXPECT_EQ(697.6, helper::Round(coord.x, 1));
  EXPECT_EQ(122.9, helper::Round(coord.y, 1));

  // shifted point
  coord = c_.CoordinateChord(0, true);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
}

TEST_F(Catenary2dTest, Length) {
  // flat - lowpoint within span
  EXPECT_EQ(1010.45, helper::Round(c_.Length(), 2));

  // inclined - lowpoint outside span
  Vector2d spacing_endpoints(1000, 500);
  c_.set_spacing_endpoints(spacing_endpoints);
  EXPECT_EQ(1127.39, helper::Round(c_.Length(), 2));
}

TEST_F(Catenary2dTest, LengthSlack) {
  // flat - lowpoint within span
  EXPECT_EQ(10.45, helper::Round(c_.LengthSlack(), 2));

  // inclined - lowpoint outside span
  Vector2d spacing_endpoints(1000, 500);
  c_.set_spacing_endpoints(spacing_endpoints);
  EXPECT_EQ(9.36, helper::Round(c_.LengthSlack(), 2));
}

TEST_F(Catenary2dTest, Sag) {
  double value = -999999;

  // checks max sag
  value = c_.Sag();
  EXPECT_EQ(62.83, helper::Round(value, 2));

  // checks sag at various position fractions
  value = c_.Sag(0.25);
  EXPECT_EQ(46.94, helper::Round(value, 2));

  value = c_.Sag(0.75);
  EXPECT_EQ(46.94, helper::Round(value, 2));
}

TEST_F(Catenary2dTest, Tension) {
  EXPECT_EQ(1031.41, helper::Round(c_.Tension(0), 2));
  EXPECT_EQ(1000.00, helper::Round(c_.Tension(0.5), 2));
}

TEST_F(Catenary2dTest, TensionAverage) {
  // approximation method
  EXPECT_EQ(1010.54, helper::Round(c_.TensionAverage(0), 2));

  // numerical method
  EXPECT_EQ(1010.74, helper::Round(c_.TensionAverage(100), 2));
  EXPECT_EQ(1010.56, helper::Round(c_.TensionAverage(1000), 2));
}

TEST_F(Catenary2dTest, TensionMax) {
  // flat - lowpoint within span
  EXPECT_EQ(1031.41, helper::Round(c_.TensionMax(), 2));

  // inclined - lowpoint outside span
  Vector2d spacing_endpoints(1000, 500);
  c_.set_spacing_endpoints(spacing_endpoints);
  EXPECT_EQ(1275.78, helper::Round(c_.TensionMax(), 2));
}

TEST_F(Catenary2dTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}


class Catenary3dTest : public ::testing::Test {
 protected:
  Catenary3dTest() {
    Vector3d spacing_endpoints(1000, 0, 0);

    c_.set_direction_transverse(AxisDirectionType::kPositive);
    c_.set_spacing_endpoints(spacing_endpoints);
    c_.set_tension_horizontal(1000);
    c_.set_weight_unit(Vector3d(0, 0, 0.5));
  }

  // test object
  Catenary3d c_;
};

TEST_F(Catenary3dTest, Coordinate) {
  Point3d<double> coord;

  // no transverse unit load
  coord = c_.Coordinate(0);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(0, helper::Round(coord.z, 1));

  coord = c_.Coordinate(0.5);
  EXPECT_EQ(500, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(-62.8, helper::Round(coord.z, 1));

  coord = c_.Coordinate(1);
  EXPECT_EQ(1000, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(0, helper::Round(coord.z, 1));

  // transverse load
  c_.set_weight_unit(Vector3d(0, 0.3535533, 0.3535533));

  coord = c_.Coordinate(0);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(0, helper::Round(coord.z, 1));

  coord = c_.Coordinate(0.5);
  EXPECT_EQ(500, helper::Round(coord.x, 1));
  EXPECT_EQ(44.4, helper::Round(coord.y, 1));
  EXPECT_EQ(-44.4, helper::Round(coord.z, 1));

  coord = c_.Coordinate(1);
  EXPECT_EQ(1000, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(0, helper::Round(coord.z, 1));

  // inclined geometry
  Vector3d spacing(1000, 0, 100);
  c_.set_spacing_endpoints(spacing);

  coord = c_.Coordinate(0);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(0, helper::Round(coord.z, 1));

  coord = c_.Coordinate(0.5);
  EXPECT_EQ(506.2, helper::Round(coord.x, 1));
  EXPECT_EQ(44.7, helper::Round(coord.y, 1));
  EXPECT_EQ(5.7, helper::Round(coord.z, 1));

  coord = c_.Coordinate(1);
  EXPECT_EQ(1000, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(100, helper::Round(coord.z, 1));

  // reverses transverse loading direction
  c_.set_direction_transverse(AxisDirectionType::kNegative);

  coord = c_.Coordinate(0);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(0, helper::Round(coord.z, 1));

  coord = c_.Coordinate(0.5);
  EXPECT_EQ(506.2, helper::Round(coord.x, 1));
  EXPECT_EQ(-44.7, helper::Round(coord.y, 1));
  EXPECT_EQ(5.7, helper::Round(coord.z, 1));

  coord = c_.Coordinate(1);
  EXPECT_EQ(1000, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(100, helper::Round(coord.z, 1));
}

TEST_F(Catenary3dTest, CoordinateChord) {
  Point3d<double> coord;

  // inclined geometry with transverse load
  c_.set_spacing_endpoints(Vector3d(1000, 0, 100));
  c_.set_weight_unit(Vector3d(0, 0.3535533, 0.3535533));

  coord = c_.CoordinateChord(0);
  EXPECT_EQ(0, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(0, helper::Round(coord.z, 1));

  coord = c_.CoordinateChord(0.5);
  EXPECT_EQ(504.4, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(50.4, helper::Round(coord.z, 1));

  coord = c_.CoordinateChord(1);
  EXPECT_EQ(1000, helper::Round(coord.x, 1));
  EXPECT_EQ(0, helper::Round(coord.y, 1));
  EXPECT_EQ(100, helper::Round(coord.z, 1));
}

TEST_F(Catenary3dTest, Sag) {
  double value = -999999;

  // checks max sag
  value = c_.Sag();
  EXPECT_EQ(62.83, helper::Round(value, 2));

  // checks sag at various position fractions
  value = c_.Sag(0.25);
  EXPECT_EQ(46.94, helper::Round(value, 2));

  value = c_.Sag(0.75);
  EXPECT_EQ(46.94, helper::Round(value, 2));
}

TEST_F(Catenary3dTest, SwingAngle) {
  double value;

  // checks with no transverse wind
  value = c_.SwingAngle();
  EXPECT_EQ(0, helper::Round(value, 0));

  // adds transverse load
  c_.set_weight_unit(Vector3d(0, 0.3535533, 0.3535533));
  value = c_.SwingAngle();
  EXPECT_EQ(45, helper::Round(value, 0));
}

TEST_F(Catenary3dTest, Tension) {
  Vector3d tension;

  // checks with no transverse wind
  tension = c_.Tension(0, AxisDirectionType::kPositive);
  EXPECT_EQ(1000, helper::Round(tension.x(), 2));
  EXPECT_EQ(0, helper::Round(tension.y(), 2));
  EXPECT_EQ(-252.61, helper::Round(tension.z(), 2));

  // adds transverse load and checks
  c_.set_weight_unit(Vector3d(0, 0.3535533, 0.3535533));

  tension = c_.Tension(0, AxisDirectionType::kPositive);
  EXPECT_EQ(1000, helper::Round(tension.x(), 2));
  EXPECT_EQ(178.62, helper::Round(tension.y(), 2));
  EXPECT_EQ(-178.62, helper::Round(tension.z(), 2));
}

TEST_F(Catenary3dTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
