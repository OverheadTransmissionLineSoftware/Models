// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/cable_heat_transfer_state.h"

#include "gtest/gtest.h"

class CableHeatTransferStateTest : public ::testing::Test {
 protected:
  CableHeatTransferStateTest() {
    // builds fixture object
    c_.heat_convection = -100;
    c_.heat_radiation = -100;
    c_.heat_resistance = 100;
    c_.heat_solar = 100;
    c_.heat_storage = 0;
  }

  ~CableHeatTransferStateTest() {
  }

  // test object
  CableHeatTransferState c_;
};

TEST_F(CableHeatTransferStateTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
