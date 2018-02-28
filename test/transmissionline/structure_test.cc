// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/structure.h"

#include "gtest/gtest.h"

#include "test/factory.h"

class StructureAttachmentTest : public ::testing::Test {
 protected:
  StructureAttachmentTest() {
    Structure* structure = factory::BuildStructure();
    s_ = structure->attachments.at(0);
  }

  StructureAttachment s_;
};

TEST_F(StructureAttachmentTest, Validate) {
  EXPECT_TRUE(s_.Validate(true, nullptr));
}

class StructureTest : public ::testing::Test {
 protected:
  StructureTest() {
    s_ = *factory::BuildStructure();
  }

  Structure s_;
};

TEST_F(StructureTest, Validate) {
  EXPECT_TRUE(s_.Validate(true, nullptr));
}
