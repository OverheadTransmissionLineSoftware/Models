// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable_position_locator.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CablePositionLocatorTest : public ::testing::Test {
 protected:
  CablePositionLocatorTest() {
    // builds dependency object
    line_ = factory::BuildTransmissionLine();

    // builds dependency object - unit weight
    weight_unit_ = new Vector3d(0, 0, 1);

    // builds fixture object
    c_.set_direction_wind(AxisDirectionType::kPositive);
    c_.set_index_line_cable(2);
    c_.set_tension_horizontal(5000);
    c_.set_transmission_line(line_);
    c_.set_weight_unit(weight_unit_);
  }

  ~CablePositionLocatorTest() {
    delete weight_unit_;
  }

  // allocated dependency objects
  TransmissionLine* line_;
  Vector3d* weight_unit_;

  // test object
  CablePositionLocator c_;
};

TEST_F(CablePositionLocatorTest, PointsCable) {
  Point3d<double> point;

  // checks second span of line cable
  // calculates 3 points - start, middle, and end

  // checks with no wind
  std::list<Point3d<double>> points = c_.PointsCable(1, 3);

  point = *std::next(points.cbegin(), 0);
  EXPECT_EQ(1993.20, helper::Round(point.x, 2));
  EXPECT_EQ(1006.81, helper::Round(point.y, 2));
  EXPECT_EQ(197.28, helper::Round(point.z, 2));

  point = *std::next(points.cbegin(), 1);
  EXPECT_EQ(1996.58, helper::Round(point.x, 2));
  EXPECT_EQ(1500.5, helper::Round(point.y, 2));
  EXPECT_EQ(124.13, helper::Round(point.z, 2));

  point = *std::next(points.cbegin(), 2);
  EXPECT_EQ(2000.00, helper::Round(point.x, 2));
  EXPECT_EQ(1999.00, helper::Round(point.y, 2));
  EXPECT_EQ(100.00, helper::Round(point.z, 2));

  // checks with wind
  weight_unit_->set_y(10);
  c_.set_weight_unit(weight_unit_);
  points = c_.PointsCable(1, 3);

  point = *std::next(points.cbegin(), 0);
  EXPECT_EQ(1992.96, helper::Round(point.x, 2));
  EXPECT_EQ(1007.05, helper::Round(point.y, 2));
  EXPECT_EQ(199.17, helper::Round(point.z, 2));

  point = *std::next(points.cbegin(), 1);
  EXPECT_EQ(1727.45, helper::Round(point.x, 2));
  EXPECT_EQ(1478.35, helper::Round(point.y, 2));
  EXPECT_EQ(122.93, helper::Round(point.z, 2));

  point = *std::next(points.cbegin(), 2);
  EXPECT_EQ(2000.00, helper::Round(point.x, 2));
  EXPECT_EQ(1999.00, helper::Round(point.y, 2));
  EXPECT_EQ(100.00, helper::Round(point.z, 2));

  // reverses direction of wind
  c_.set_direction_wind(AxisDirectionType::kNegative);
  points = c_.PointsCable(1, 3);

  point = *std::next(points.cbegin(), 0);
  EXPECT_EQ(2006.37, helper::Round(point.x, 2));
  EXPECT_EQ(993.63, helper::Round(point.y, 2));
  EXPECT_EQ(195.67, helper::Round(point.z, 2));

  point = *std::next(points.cbegin(), 1);
  EXPECT_EQ(2279.96, helper::Round(point.x, 2));
  EXPECT_EQ(1472.04, helper::Round(point.y, 2));
  EXPECT_EQ(120.38, helper::Round(point.z, 2));

  point = *std::next(points.cbegin(), 2);
  EXPECT_EQ(2000.00, helper::Round(point.x, 2));
  EXPECT_EQ(1999.00, helper::Round(point.y, 2));
  EXPECT_EQ(100.00, helper::Round(point.z, 2));
}

TEST_F(CablePositionLocatorTest, PointsCableAttachment) {
  const std::vector<Point3d<double>>* points = c_.PointsCableAttachment();
  Point3d<double> point;

  // checks with no wind
  point = points->at(0);
  EXPECT_EQ(1000.71, helper::Round(point.x, 2));
  EXPECT_EQ(1000.71, helper::Round(point.y, 2));
  EXPECT_EQ(100.00, helper::Round(point.z, 2));

  point = points->at(1);
  EXPECT_EQ(1993.20, helper::Round(point.x, 2));
  EXPECT_EQ(1006.81, helper::Round(point.y, 2));
  EXPECT_EQ(197.28, helper::Round(point.z, 2));

  point = points->at(2);
  EXPECT_EQ(2000.00, helper::Round(point.x, 2));
  EXPECT_EQ(1999.00, helper::Round(point.y, 2));
  EXPECT_EQ(100.00, helper::Round(point.z, 2));

  // checks with wind
  weight_unit_->set_y(10);
  c_.set_weight_unit(weight_unit_);
  points = c_.PointsCableAttachment();

  point = points->at(0);
  EXPECT_EQ(1000.71, helper::Round(point.x, 2));
  EXPECT_EQ(1000.71, helper::Round(point.y, 2));
  EXPECT_EQ(100.00, helper::Round(point.z, 2));

  point = points->at(1);
  EXPECT_EQ(1992.96, helper::Round(point.x, 2));
  EXPECT_EQ(1007.05, helper::Round(point.y, 2));
  EXPECT_EQ(199.17, helper::Round(point.z, 2));

  point = points->at(2);
  EXPECT_EQ(2000.00, helper::Round(point.x, 2));
  EXPECT_EQ(1999.00, helper::Round(point.y, 2));
  EXPECT_EQ(100.00, helper::Round(point.z, 2));

  // reverses direction of wind
  c_.set_direction_wind(AxisDirectionType::kNegative);
  points = c_.PointsCableAttachment();

  point = points->at(0);
  EXPECT_EQ(1000.71, helper::Round(point.x, 2));
  EXPECT_EQ(1000.71, helper::Round(point.y, 2));
  EXPECT_EQ(100.00, helper::Round(point.z, 2));

  point = points->at(1);
  EXPECT_EQ(2006.37, helper::Round(point.x, 2));
  EXPECT_EQ(993.63, helper::Round(point.y, 2));
  EXPECT_EQ(195.67, helper::Round(point.z, 2));

  point = points->at(2);
  EXPECT_EQ(2000.00, helper::Round(point.x, 2));
  EXPECT_EQ(1999.00, helper::Round(point.y, 2));
  EXPECT_EQ(100.00, helper::Round(point.z, 2));
}

TEST_F(CablePositionLocatorTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
