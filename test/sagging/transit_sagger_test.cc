// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagging/transit_sagger.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

class TransitSaggerTest : public ::testing::Test {
 protected:
  TransitSaggerTest() {
    // builds catenary
    Vector3d spacing_endpoints(2000, 0, 100);
    Vector3d weight_unit(0, 0, 1);

    Catenary3d catenary;
    catenary.set_direction_transverse(AxisDirectionType::kPositive);
    catenary.set_spacing_endpoints(spacing_endpoints);
    catenary.set_tension_horizontal(5000);
    catenary.set_weight_unit(weight_unit);

    // builds transit point
    Point3d<double> point_transit(0, 0, -50);

    // builds fixture
    t_.set_catenary(catenary);
    t_.set_point_transit(point_transit);
  }

  ~TransitSaggerTest() {
  }

  // test fixture
  TransitSagger t_;
};

TEST_F(TransitSaggerTest, AngleLow) {
  double value = -999999;

  // checks looking ahead-downward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(0, 0, -50));
  value = t_.AngleLow();
  EXPECT_EQ(-0.520, helper::Round(value, 3));

  // checks looking ahead-upward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(0, 0, -100));
  value = t_.AngleLow();
  EXPECT_EQ(2.836, helper::Round(value, 3));

  // checks looking ahead-upward from transit, transit offset from y-axis
  t_.set_point_transit(Point3d<double>(0, 100, -100));
  value = t_.AngleLow();
  EXPECT_EQ(2.822, helper::Round(value, 3));

  // checks looking back-upward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(2000, 0, -100));
  value = t_.AngleLow();
  EXPECT_EQ(1.868, helper::Round(value, 3));

  // checks looking back-downward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(2000, 0, -50));
  value = t_.AngleLow();
  EXPECT_EQ(-0.307, helper::Round(value, 3));

  // checks invalid transit location
  t_.set_point_transit(Point3d<double>(0, 0, 5));
  value = t_.AngleLow();
  EXPECT_EQ(-999999, helper::Round(value, 0));
}

TEST_F(TransitSaggerTest, FactorControl) {
  double value = -999999;

  // checks looking ahead-downward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(0, 0, -50));
  value = t_.FactorControl();
  EXPECT_EQ(0.913, helper::Round(value, 3));

  // checks looking ahead-downward from transit, transit offset from y-axis
  // moving transit away from y-axis gives a slightly better shot at true
  // catenary sag point
  t_.set_point_transit(Point3d<double>(0, 100, -50));
  value = t_.FactorControl();
  EXPECT_EQ(0.914, helper::Round(value, 3));

  // checks invalid transit location
  t_.set_point_transit(Point3d<double>(0, 0, 5));
  value = t_.FactorControl();
  EXPECT_EQ(-999999, helper::Round(value, 0));
}

TEST_F(TransitSaggerTest, PointCatenaryLow) {
  Point3d<double> point;

  // checks looking ahead-downward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(0, 0, -50));
  point = t_.PointCatenaryLow();
  EXPECT_EQ(706.35, helper::Round(point.x, 2));
  EXPECT_EQ(0.00, helper::Round(point.y, 2));
  EXPECT_EQ(-56.41, helper::Round(point.z, 2));

  // checks looking ahead-downward from transit, transit offset from y-axis
  // moving transit away from y-axis gives a slightly better shot at true
  // catenary sag point
  t_.set_point_transit(Point3d<double>(0, 100, -50));
  point = t_.PointCatenaryLow();
  EXPECT_EQ(707.24, helper::Round(point.x, 2));
  EXPECT_EQ(0.000, helper::Round(point.y, 2));
  EXPECT_EQ(-56.42, helper::Round(point.z, 2));
}

TEST_F(TransitSaggerTest, PointTarget) {
  Point3d<double> point;

  // checks looking ahead-downward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(0, 0, -50));
  point = t_.PointTarget();
  EXPECT_EQ(2000.00, helper::Round(point.x, 2));
  EXPECT_EQ(0.00, helper::Round(point.y, 2));
  EXPECT_EQ(-68.16, helper::Round(point.z, 2));

  // checks looking ahead-upward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(0, 0, -100));
  point = t_.PointTarget();
  EXPECT_EQ(2000.00, helper::Round(point.x, 2));
  EXPECT_EQ(0.00, helper::Round(point.y, 2));
  EXPECT_EQ(-0.91, helper::Round(point.z, 2));

  // checks looking back-upward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(2000, 0, -100));
  point = t_.PointTarget();
  EXPECT_EQ(0.00, helper::Round(point.x, 2));
  EXPECT_EQ(0.00, helper::Round(point.y, 2));
  EXPECT_EQ(-34.76, helper::Round(point.z, 2));

  // checks looking back-downward from transit, transit on y-axis
  t_.set_point_transit(Point3d<double>(2000, 0, -50));
  point = t_.PointTarget();
  EXPECT_EQ(0.00, helper::Round(point.x, 2));
  EXPECT_EQ(0.00, helper::Round(point.y, 2));
  EXPECT_EQ(-60.72, helper::Round(point.z, 2));

  // checks transit offset from y-axis
  t_.set_point_transit(Point3d<double>(0, 100, -100));
  point = t_.PointTarget();
  EXPECT_EQ(-999999.00, helper::Round(point.x, 2));
  EXPECT_EQ(-999999.00, helper::Round(point.y, 2));
  EXPECT_EQ(-999999.00, helper::Round(point.z, 2));
}

TEST_F(TransitSaggerTest, Validate) {
  EXPECT_TRUE(t_.Validate(true, nullptr));
}
