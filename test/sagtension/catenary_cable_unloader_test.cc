// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/catenary_cable_unloader.h"

#include "gtest/gtest.h"

#include "base/helper.h"
#include "factory.h"

class CatenaryCableUnloaderTest : public ::testing::Test {
 protected:
  CatenaryCableUnloaderTest() {

    Cable cable = factory::BuildCable();
    Vector3d spacing_endpoints(1200, 0, 0);

    CableState state;
    state.load_stretch = 0;
    state.temperature = 60;
    state.temperature_stretch = 0;

    Vector3d weight_unit(0, 0, 1.094);

    CatenaryCable catenary_cable;
    catenary_cable.set_cable(cable);
    catenary_cable.set_spacing_endpoints(spacing_endpoints);
    catenary_cable.set_state(state);
    catenary_cable.set_tension_horizontal(6000);
    catenary_cable.set_weight_unit(weight_unit);

    // builds dependency object - unloaded state
    CableState state_unloaded;
    state_unloaded.load_stretch = 0;
    state_unloaded.temperature = 32;
    state_unloaded.temperature_stretch = 0;

    // builds fixture object
    c_.set_catenary_cable(catenary_cable);
    c_.set_state_unloaded(state_unloaded);
  }

  CatenaryCableUnloader c_;
};

TEST_F(CatenaryCableUnloaderTest, LengthUnloaded) {

  CatenaryCable catenary_cable = c_.catenary_cable();
  CableState state = catenary_cable.state();

  // unstretched catenary cable state
  EXPECT_EQ(1200.82, helper::Round(c_.LengthUnloaded(), 2));

  // stretched catenary cable state
  state.load_stretch = 12179.2;
  catenary_cable.set_state(state);
  c_.set_catenary_cable(catenary_cable);
  EXPECT_EQ(1200.35, helper::Round(c_.LengthUnloaded(), 2));
}

TEST_F(CatenaryCableUnloaderTest, Validate) {

  EXPECT_TRUE(c_.Validate(false, nullptr));
}
