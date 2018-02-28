// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_structure.h"

#include "gtest/gtest.h"

#include "factory.h"

class LineStructureTest : public ::testing::Test {
 protected:
  LineStructureTest() {
    l_ = *factory::BuildLineStructure();
  }

  LineStructure l_;
};

TEST_F(LineStructureTest, AttachHardware) {
  // modifies the default hardware assembly and adds
  Hardware* hardware = factory::BuildHardware();
  hardware->length = 20;
  l_.AttachHardware(0, hardware);

  const Hardware* hardware_attached = l_.hardwares()->at(0);
  EXPECT_EQ(20, hardware_attached->length);
}

TEST_F(LineStructureTest, DetachHardware) {
  l_.DetachHardware(0);

  const Hardware* hardware_attached = l_.hardwares()->at(0);
  EXPECT_TRUE(hardware_attached == nullptr);
}

TEST_F(LineStructureTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
