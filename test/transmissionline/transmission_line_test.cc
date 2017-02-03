// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/transmission_line.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class TransmissionLineTest : public ::testing::Test {
 protected:
  TransmissionLineTest() {
    t_ = *factory::BuildTransmissionLine();
  }

  TransmissionLine t_;
};

TEST_F(TransmissionLineTest, AddAlignmentPoint) {
  AlignmentPoint point;
  point.elevation = 0;
  point.rotation = 0;
  point.station = 100;
  t_.AddAlignmentPoint(point);

  // checks to make sure point was sorted correctly
  auto iter = std::next(t_.alignment()->points()->cbegin(), 1);
  point = *iter;
  EXPECT_EQ(100, point.station);
}

TEST_F(TransmissionLineTest, AddLineStructure) {
  AlignmentPoint point;
  point.elevation = 0;
  point.rotation = 0;
  point.station = 100;
  t_.AddAlignmentPoint(point);

  // checks to make sure point was sorted correctly
  auto iter = std::next(t_.alignment()->points()->cbegin(), 1);
  point = *iter;
  EXPECT_EQ(100, point.station);
}

TEST_F(TransmissionLineTest, DeleteAlignmentPoint) {
  t_.DeleteAlignmentPoint(1);

  // checks the second alignment point, which was originally the third
  auto iter = std::next(t_.alignment()->points()->cbegin(), 1);
  const AlignmentPoint& point = *iter;
  EXPECT_EQ(2000, point.station);
}

TEST_F(TransmissionLineTest, ModifyAlignmentPoint) {
  AlignmentPoint point;
  point.elevation = 0;
  point.rotation = 0;
  point.station = 4000;

  t_.ModifyAlignmentPoint(1, point);

  // checks second alignment point, which was originally the third
  auto iter = std::next(t_.alignment()->points()->cbegin(), 1);
  point = *iter;
  EXPECT_EQ(2000, point.station);

  // checks fourth alignment point, which should be the re-sorted/modified one
  iter = std::next(t_.alignment()->points()->cbegin(), 3);
  point = *iter;
  EXPECT_EQ(4000, point.station);
}

TEST_F(TransmissionLineTest, PointsXyzAlignment) {
  const std::vector<Point3d>* points_xyz = t_.PointsXyzAlignment();
  Point3d point;

  // checks all xyz alignment points
  point = points_xyz->at(0);
  EXPECT_EQ(0, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = points_xyz->at(1);
  EXPECT_EQ(1000, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = points_xyz->at(2);
  EXPECT_EQ(1000, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = points_xyz->at(3);
  EXPECT_EQ(2000, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(100, helper::Round(point.z, 2));

  point = points_xyz->at(4);
  EXPECT_EQ(3000, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));
}

TEST_F(TransmissionLineTest, PointXyzAlignment) {
  Point3d point;

  // tests points in the middle of an alignment
  point = t_.PointXyzAlignment(500);
  EXPECT_EQ(500, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = t_.PointXyzAlignment(1500);
  EXPECT_EQ(1000, helper::Round(point.x, 2));
  EXPECT_EQ(500, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = t_.PointXyzAlignment(2500);
  EXPECT_EQ(1500, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(50, helper::Round(point.z, 2));

  point = t_.PointXyzAlignment(3500);
  EXPECT_EQ(2500, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(50, helper::Round(point.z, 2));
}

TEST_F(TransmissionLineTest, Validate) {
  EXPECT_TRUE(t_.Validate(true, nullptr));
}
