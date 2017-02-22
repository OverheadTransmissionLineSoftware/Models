// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable_attachment_equilibrium_solver.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

class CableAttachmentEquilibriumSolverTest : public ::testing::Test {
 protected:
  CableAttachmentEquilibriumSolverTest() {
    // builds dependency objects - catenaries
    Vector3d spacing_endpoints(1000, 0, 0);
    Vector3d weight_unit(0, 0, 1);

    catenary_back_ = new Catenary3d();
    catenary_back_->set_spacing_endpoints(spacing_endpoints);
    catenary_back_->set_tension_horizontal(5000);
    catenary_back_->set_weight_unit(weight_unit);

    catenary_ahead_ = new Catenary3d();
    catenary_ahead_->set_spacing_endpoints(spacing_endpoints);
    catenary_ahead_->set_tension_horizontal(5000);
    catenary_ahead_->set_weight_unit(weight_unit);
    
    // builds fixture object
    c_.set_angle_catenaries(180);
    c_.set_angle_hardware(180);
    c_.set_catenary_ahead(catenary_ahead_);
    c_.set_catenary_back(catenary_back_);
  }

  ~CableAttachmentEquilibriumSolverTest() {
    delete catenary_ahead_;
    delete catenary_back_;
  }

  // allocated dependency objects
  Catenary3d* catenary_ahead_;
  Catenary3d* catenary_back_;

  // test object
  CableAttachmentEquilibriumSolver c_;
};

TEST_F(CableAttachmentEquilibriumSolverTest, AngleEquilibrium) {
  double value;

  // checks tangent catenaries
  value = c_.AngleEquilibrium();
  EXPECT_EQ(180, helper::Round(value, 2));

  // modifies the catenary angle to 90 and then checks
  c_.set_angle_catenaries(90);
  value = c_.AngleEquilibrium();
  EXPECT_EQ(98.06, helper::Round(value, 2));
}

TEST_F(CableAttachmentEquilibriumSolverTest, TensionImbalance) {
  double value;

  // checks tangent catenaries
  value = c_.TensionImbalance();
  EXPECT_EQ(0, helper::Round(value, 2));

  // modifies the catenary angle to 90 and then checks
  c_.set_angle_catenaries(90);
  value = c_.TensionImbalance();
  EXPECT_EQ(7071.07, helper::Round(value, 2));  
}

TEST_F(CableAttachmentEquilibriumSolverTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
