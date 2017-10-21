// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_loader_base.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class LineCableLoaderBaseTest : public ::testing::Test {
 protected:
  LineCableLoaderBaseTest() {
    // gets line cable from factory
    linecable_ = factory::BuildLineCable();

    // builds fixture object
    l_.set_line_cable(linecable_);
  }

  ~LineCableLoaderBaseTest() {
    factory::DestroyLineCable(linecable_);
  }

  // allocated dependency objects
  LineCable* linecable_;

  // test object
  LineCableLoaderBase l_;
};

TEST_F(LineCableLoaderBaseTest, StateStretchCreep) {
  double value = -999999;
  CableConstraint constraint = linecable_->constraint();
  CableStretchState state;

  // checks state with constraint type and limit set to initial condition
  constraint.condition = CableConditionType::kInitial;
  constraint.limit = 6000;
  linecable_->set_constraint(constraint);
  l_.set_line_cable(linecable_);

  state = l_.StretchStateCreep();
  value = state.temperature;
  EXPECT_EQ(60, helper::Round(value, 0));
  EXPECT_EQ(SagTensionCableComponent::PolynomialType::kCreep,
            state.type_polynomial);
  value = state.load;
  EXPECT_EQ(5595, helper::Round(value, 0));

  // checks state with constraint type and limit set to load condition
  constraint.condition = CableConditionType::kLoad;
  constraint.limit = 5561.5;
  linecable_->set_constraint(constraint);
  l_.set_line_cable(linecable_);

  state = l_.StretchStateCreep();
  value = state.temperature;
  EXPECT_EQ(60, helper::Round(value, 0));
  EXPECT_EQ(SagTensionCableComponent::PolynomialType::kCreep,
            state.type_polynomial);
  value = state.load;
  EXPECT_EQ(5595, helper::Round(value, 0));

  // checks state with constraint type and limit set to creep condition
  constraint.condition = CableConditionType::kCreep;
  constraint.limit = 5582.25;
  linecable_->set_constraint(constraint);
  l_.set_line_cable(linecable_);

  state = l_.StretchStateCreep();
  value = state.temperature;
  EXPECT_EQ(60, helper::Round(value, 0));
  EXPECT_EQ(SagTensionCableComponent::PolynomialType::kCreep,
            state.type_polynomial);
  value = state.load;
  EXPECT_EQ(5595, helper::Round(value, 0));
}

TEST_F(LineCableLoaderBaseTest, StateStretchLoad) {
  double value = -999999;
  CableConstraint constraint = linecable_->constraint();
  CableStretchState state;

  // checks state with constraint type and limit set to initial condition
  constraint.condition = CableConditionType::kInitial;
  constraint.limit = 6000;
  linecable_->set_constraint(constraint);
  l_.set_line_cable(linecable_);

  state = l_.StretchStateLoad();
  value = state.temperature;
  EXPECT_EQ(0, helper::Round(value, 0));
  EXPECT_EQ(SagTensionCableComponent::PolynomialType::kLoadStrain,
            state.type_polynomial);
  value = state.load;
  EXPECT_EQ(12179, helper::Round(value, 0));

  // checks state with constraint type and limit set to load condition
  constraint.condition = CableConditionType::kLoad;
  constraint.limit = 5561.5;
  linecable_->set_constraint(constraint);
  l_.set_line_cable(linecable_);

  state = l_.StretchStateLoad();
  value = state.temperature;
  EXPECT_EQ(0, helper::Round(value, 0));
  EXPECT_EQ(SagTensionCableComponent::PolynomialType::kLoadStrain,
            state.type_polynomial);
  value = state.load;
  EXPECT_EQ(12179, helper::Round(value, 0));

  // checks state with constraint type and limit set to creep condition
  constraint.condition = CableConditionType::kCreep;
  constraint.limit = 5582.25;
  linecable_->set_constraint(constraint);
  l_.set_line_cable(linecable_);

  state = l_.StretchStateLoad();
  value = state.temperature;
  EXPECT_EQ(0, helper::Round(value, 0));
  EXPECT_EQ(SagTensionCableComponent::PolynomialType::kLoadStrain,
            state.type_polynomial);
  value = state.load;
  EXPECT_EQ(12179, helper::Round(value, 0));
}

TEST_F(LineCableLoaderBaseTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
