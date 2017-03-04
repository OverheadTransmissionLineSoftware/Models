// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_cable.h"

#include "gtest/gtest.h"

#include "factory.h"

class LineCableTest : public ::testing::Test {
 protected:
  LineCableTest() {
    l_ = factory::BuildLineCable();

    // builds dependency objects - line structure
    Hardware* hardware = factory::BuildHardware();
    hardware->type = Hardware::HardwareType::kSuspension;

    line_structure_ = factory::BuildLineStructure();
    line_structure_->DetachHardware(0);
    line_structure_->AttachHardware(0, hardware);
    line_structure_->set_station(500);

    // adds additional connections
    LineCableConnection connection;
    connection.line_structure = line_structure_;
    connection.index_attachment = 0;
    l_->AddConnection(connection);
  }

  ~LineCableTest() {
    factory::DestroyLineCable(l_);
  }

  // allocated dependency objects
  LineStructure* line_structure_;

  // test object
  LineCable* l_;
};

TEST_F(LineCableTest, AddConnection) {
  LineCableConnection connection;
  LineStructure* structure = factory::BuildLineStructure();
  structure->set_station(250);
  int index = 0;

  // adds a valid connection
  connection.line_structure = structure;
  connection.index_attachment = 0;
  index = l_->AddConnection(connection);
  EXPECT_EQ(1, index);
}

TEST_F(LineCableTest, DeleteConnection) {
  // deletes a valid connection
  EXPECT_TRUE(l_->DeleteConnection(1));

  // deletes an invalid connection
  EXPECT_FALSE(l_->DeleteConnection(2));
}

TEST_F(LineCableTest, ModifyConnection) {
  LineCableConnection connection;
  LineStructure* structure = factory::BuildLineStructure();
  structure->set_station(250);
  int index = 0;

  // modifies a valid connection
  connection.line_structure = structure;
  connection.index_attachment = 0;
  index = l_->ModifyConnection(1, connection);
  EXPECT_EQ(1, index);

  connection = *std::next(l_->connections()->cbegin(), index);
  EXPECT_EQ(250, connection.line_structure->station());

  // modifies an invalid connection
  index = l_->ModifyConnection(3, connection);
  EXPECT_EQ(-1, index);
}

TEST_F(LineCableTest, Validate) {
  EXPECT_TRUE(l_->Validate(true, nullptr));
}
