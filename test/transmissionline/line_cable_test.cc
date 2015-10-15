// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_cable.h"

#include "gtest/gtest.h"

#include "factory.h"

class LineCableTest : public ::testing::Test {
 protected:
  LineCableTest() {
    l_ = *factory::BuildLineCable();
  }

  LineCable l_;
};

TEST_F(LineCableTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
