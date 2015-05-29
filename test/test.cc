// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "gtest/gtest.h"

// This function calls the tests set up in the subfolders (for each library).
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
