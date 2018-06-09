// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagging/cable_sag_position_corrector.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

#include "test/factory.h"

class CableSagPositionCorrectorTest : public ::testing::Test {
 protected:
  CableSagPositionCorrectorTest() {
    // gets line cable from factory
    linecable_ = factory::BuildLineCable();

    // builds attachment points
    // the terrain is symmetrical about the center span
    // the span lengths are staggered
    // the elevation increases to the center span
    points_attachment_ = new std::vector<Point3d<double>>(10);
    (*points_attachment_)[0] = Point3d<double>(0, 0, 0);
    (*points_attachment_)[1] = Point3d<double>(1200, 0, 50);
    (*points_attachment_)[2] = Point3d<double>(2200, 0, 150);
    (*points_attachment_)[3] = Point3d<double>(3800, 0, 300);
    (*points_attachment_)[4] = Point3d<double>(5200, 0, 500);
    //// center span
    (*points_attachment_)[5] = Point3d<double>(8200, 0, 500);
    (*points_attachment_)[6] = Point3d<double>(9600, 0, 300);
    (*points_attachment_)[7] = Point3d<double>(11200, 0, 150);
    (*points_attachment_)[8] = Point3d<double>(12200, 0, 50);
    (*points_attachment_)[9] = Point3d<double>(13400, 0, 0);

    // builds fixture
    c_.set_linecable(linecable_);
    c_.set_points_attachment(points_attachment_);
  }

  ~CableSagPositionCorrectorTest() {
    delete linecable_;
    delete points_attachment_;
  }

  // allocated dependency objects
  LineCable* linecable_;
  std::vector<Point3d<double>>* points_attachment_;

  // test fixture
  CableSagPositionCorrector c_;
};

TEST_F(CableSagPositionCorrectorTest, CatenariesClipped) {
  double value = -999999;

  // checks catenary horizontal tensions
  const std::vector<Catenary3d>* catenaries = c_.CatenariesClipped();

  value = (*catenaries)[0].tension_horizontal();
  EXPECT_EQ(6000.0, helper::Round(value, 1));

  value = (*catenaries)[1].tension_horizontal();
  EXPECT_EQ(6000.0, helper::Round(value, 1));

  value = (*catenaries)[2].tension_horizontal();
  EXPECT_EQ(6000.0, helper::Round(value, 1));

  value = (*catenaries)[3].tension_horizontal();
  EXPECT_EQ(6000.0, helper::Round(value, 1));

  value = (*catenaries)[4].tension_horizontal();
  EXPECT_EQ(6000.0, helper::Round(value, 1));

  value = (*catenaries)[5].tension_horizontal();
  EXPECT_EQ(6000.0, helper::Round(value, 1));

  value = (*catenaries)[6].tension_horizontal();
  EXPECT_EQ(6000.0, helper::Round(value, 1));

  value = (*catenaries)[7].tension_horizontal();
  EXPECT_EQ(6000.0, helper::Round(value, 1));

  value = (*catenaries)[8].tension_horizontal();
  EXPECT_EQ(6000.0, helper::Round(value, 1));
}

TEST_F(CableSagPositionCorrectorTest, CatenariesPulleyed) {
  double value = -999999;

  // checks catenary horizontal tensions
  const std::vector<Catenary3d>* catenaries = c_.CatenariesPulleyed();

  value = (*catenaries)[0].tension_horizontal();
  EXPECT_EQ(5730.5, helper::Round(value, 1));

  value = (*catenaries)[1].tension_horizontal();
  EXPECT_EQ(5800.4, helper::Round(value, 1));

  value = (*catenaries)[2].tension_horizontal();
  EXPECT_EQ(5900.9, helper::Round(value, 1));

  value = (*catenaries)[3].tension_horizontal();
  EXPECT_EQ(6073.2, helper::Round(value, 1));

  value = (*catenaries)[4].tension_horizontal();
  EXPECT_EQ(6069.6, helper::Round(value, 1));

  value = (*catenaries)[5].tension_horizontal();
  EXPECT_EQ(6073.2, helper::Round(value, 1));

  value = (*catenaries)[6].tension_horizontal();
  EXPECT_EQ(5900.9, helper::Round(value, 1));

  value = (*catenaries)[7].tension_horizontal();
  EXPECT_EQ(5800.4, helper::Round(value, 1));

  value = (*catenaries)[8].tension_horizontal();
  EXPECT_EQ(5730.5, helper::Round(value, 1));
}

TEST_F(CableSagPositionCorrectorTest, ClippingOffsets) {
  double value = -999999;

  // checks offsets
  std::vector<double> offsets = c_.ClippingOffsets();

  value = offsets[0];
  EXPECT_EQ(0.0, helper::Round(value, 1));

  value = offsets[1];
  EXPECT_EQ(-0.2, helper::Round(value, 1));

  value = offsets[2];
  EXPECT_EQ(-0.3, helper::Round(value, 1));

  value = offsets[3];
  EXPECT_EQ(-0.5, helper::Round(value, 1));

  value = offsets[4];
  EXPECT_EQ(-0.4, helper::Round(value, 1));

  value = offsets[5];
  EXPECT_EQ(0.4, helper::Round(value, 1));

  value = offsets[6];
  EXPECT_EQ(0.5, helper::Round(value, 1));

  value = offsets[7];
  EXPECT_EQ(0.3, helper::Round(value, 1));

  value = offsets[8];
  EXPECT_EQ(0.2, helper::Round(value, 1));

  value = offsets[9];
  EXPECT_EQ(0, helper::Round(value, 1));
}

TEST_F(CableSagPositionCorrectorTest, SagCorrections) {
  double value = -999999;

  // checks sag corrections
  std::vector<double> corrections = c_.SagCorrections();

  value = corrections[0];
  EXPECT_EQ(1.5, helper::Round(value, 1));

  value = corrections[1];
  EXPECT_EQ(0.8, helper::Round(value, 1));

  value = corrections[2];
  EXPECT_EQ(1.0, helper::Round(value, 1));

  value = corrections[3];
  EXPECT_EQ(-0.5, helper::Round(value, 1));

  value = corrections[4];
  EXPECT_EQ(-2.4, helper::Round(value, 1));

  value = corrections[5];
  EXPECT_EQ(-0.5, helper::Round(value, 1));

  value = corrections[6];
  EXPECT_EQ(1.0, helper::Round(value, 1));

  value = corrections[7];
  EXPECT_EQ(0.8, helper::Round(value, 1));

  value = corrections[8];
  EXPECT_EQ(1.5, helper::Round(value, 1));
}

TEST_F(CableSagPositionCorrectorTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
