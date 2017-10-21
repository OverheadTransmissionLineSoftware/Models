// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_unloader.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class LineCableUnloaderTest : public ::testing::Test {
 protected:
  LineCableUnloaderTest() {
    // gets a sag-tension cable from factory and removes compression
    cable_ = factory::BuildCable();
    cable_->component_core.modulus_compression_elastic_area = 0;
    cable_->component_shell.modulus_compression_elastic_area = 0;

    // gets line cable from factory
    // replaces with custom cable
    linecable_ = factory::BuildLineCable();
    linecable_->set_cable(cable_);

    // builds fixture object
    l_.set_line_cable(linecable_);
    l_.set_condition_unloaded(CableConditionType::kInitial);
    l_.set_spacing_attachments(Vector3d(1500, 0, 0));
    l_.set_temperature_unloaded(60);
  }

  ~LineCableUnloaderTest() {
    factory::DestroyLineCable(linecable_);
  }

  // allocated dependency objects
  Cable* cable_;
  LineCable* linecable_;

  // test object
  LineCableUnloader l_;
};

TEST_F(LineCableUnloaderTest, LengthUnloaded) {
  double value = -999999;

  // the following results aren't intuitive, but still provide a benchmark
  //
  // for this fixture temperature, the cable core defines the unloaded strain
  // the cable core slope is above and below the tension modulus at varying
  // strain points, making the unloaded lengths unpredictable between the
  // different conditions

  // checks all conditions
  l_.set_condition_unloaded(CableConditionType::kInitial);
  value = l_.LengthUnloaded();
  EXPECT_EQ(1502.973, helper::Round(value, 3));

  l_.set_condition_unloaded(CableConditionType::kCreep);
  value = l_.LengthUnloaded();
  EXPECT_EQ(1502.951, helper::Round(value, 3));

  l_.set_condition_unloaded(CableConditionType::kLoad);
  value = l_.LengthUnloaded();
  EXPECT_EQ(1502.916, helper::Round(value, 3));
}

TEST_F(LineCableUnloaderTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
