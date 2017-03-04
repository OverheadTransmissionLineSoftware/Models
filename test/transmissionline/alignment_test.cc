// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/alignment.h"

#include "gtest/gtest.h"

class AlignmentPointTest : public ::testing::Test {
 protected:
  AlignmentPointTest() {
    // builds fixture object
    a_.elevation = 100;
    a_.rotation = 180;
    a_.station = 1000;
  }

  // test object
  AlignmentPoint a_;
};

TEST_F(AlignmentPointTest, Validate) {
  EXPECT_TRUE(a_.Validate(true, nullptr));
}

class AlignmentTest : public ::testing::Test {
 protected:
  AlignmentTest() {
    // builds dependency object - point
    AlignmentPoint point;
    point.elevation = 0;
    point.rotation = 0;
    point.station = 0;

    a_.AddPoint(point);
    
    // builds dependency object - point
    point = AlignmentPoint();
    point.elevation = 0;
    point.rotation = 0;
    point.station = 100;

    a_.AddPoint(point);

    // builds dependency object - point
    point = AlignmentPoint();
    point.elevation = 0;
    point.rotation = 0;
    point.station = 200;

    a_.AddPoint(point);
  }

  // test object
  Alignment a_;
};

TEST_F(AlignmentTest, AddPoint) {
  AlignmentPoint point;
  auto points = a_.points();
  int index = 0;

  // adds a point in the middle of the existing alignment
  point = AlignmentPoint();
  point.elevation = 0;
  point.rotation = 0;
  point.station = 150;
  index = a_.AddPoint(point);

  EXPECT_EQ(2, index);
  auto iter = std::next(points->cbegin(), index);
  point = *iter;
  EXPECT_EQ(150, point.station);

  // adds a point after the existing alignment
  point = AlignmentPoint();
  point.elevation = 0;
  point.rotation = 0;
  point.station = 250;
  index = a_.AddPoint(point);

  EXPECT_EQ(4, index);
  iter = std::next(points->cbegin(), index);
  point = *iter;
  EXPECT_EQ(250, point.station);

  // adds a duplicate point
  index = a_.AddPoint(point);
  EXPECT_EQ(-1, index);
}

TEST_F(AlignmentTest, DeletePoint) {
  AlignmentPoint point;
  auto points = a_.points();
  bool status = false;
  
  // deletes the middle point
  status = a_.DeletePoint(1);

  EXPECT_TRUE(status);
  auto iter = std::next(points->cbegin(), 1);
  point = *iter;
  EXPECT_EQ(200, point.station);

  // attempts invalid index
  status = a_.DeletePoint(2);
  EXPECT_FALSE(status);
}

TEST_F(AlignmentTest, IndexSegment) {
  double station;
  int index;

  station = -50;
  index = a_.IndexSegment(station);
  EXPECT_EQ(-1, index);

  station = 50;
  index = a_.IndexSegment(station);
  EXPECT_EQ(0, index);

  station = 100;
  index = a_.IndexSegment(station);
  EXPECT_EQ(0, index);

  station = 150;
  index = a_.IndexSegment(station);
  EXPECT_EQ(1, index);
}

TEST_F(AlignmentTest, IsValidStation) {
  double station;
  bool is_valid = false;

  station = -100;
  is_valid = a_.IsValidStation(station);
  EXPECT_FALSE(is_valid);

  station = 100;
  is_valid = a_.IsValidStation(station);
  EXPECT_TRUE(is_valid);

  station = 300;
  is_valid = a_.IsValidStation(station);
  EXPECT_FALSE(is_valid);
}

TEST_F(AlignmentTest, ModifyPoint) {
  AlignmentPoint point;
  auto points = a_.points();
  int index = 0;
  
  // modifies the middle point
  auto iter = std::next(points->cbegin(), 1);
  point = *iter;
  point.station = 400;
  index = a_.ModifyPoint(1, point);

  EXPECT_EQ(2, index);
  iter = std::next(points->cbegin(), index);
  point = *iter;
  EXPECT_EQ(400, point.station);

  // attempts invalid index
  index = a_.ModifyPoint(3, point);
  EXPECT_EQ(-1, index);
}

TEST_F(AlignmentTest, Validate) {
  EXPECT_TRUE(a_.Validate(true, nullptr));
}
