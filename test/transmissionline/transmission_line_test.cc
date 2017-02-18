// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/transmission_line.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class TransmissionLineTest : public ::testing::Test {
 protected:
  TransmissionLineTest() {
    t_ = *factory::BuildTransmissionLine();
  }

  TransmissionLine t_;
};

TEST_F(TransmissionLineTest, AddAlignmentPoint) {
  AlignmentPoint point;
  point.elevation = 0;
  point.rotation = 0;
  point.station = 100;
  t_.AddAlignmentPoint(point);

  // checks to make sure point was sorted correctly
  auto iter = std::next(t_.alignment()->points()->cbegin(), 1);
  point = *iter;
  EXPECT_EQ(100, point.station);
}

TEST_F(TransmissionLineTest, AddLineCable) {
  Hardware* hardware_deadend = factory::BuildHardware();
  hardware_deadend->type = Hardware::HardwareType::kDeadEnd;

  const std::list<LineStructure>* line_structures = t_.line_structures();
  LineStructure line_structure;

  // modifies first and second line structures and adds dead-end hardware
  line_structure = *std::next(line_structures->cbegin(), 0);
  line_structure.AttachHardware(1, hardware_deadend);
  t_.ModifyLineStructure(0, line_structure);

  line_structure = *std::next(line_structures->cbegin(), 1);
  line_structure.AttachHardware(1, hardware_deadend);
  t_.ModifyLineStructure(1, line_structure);

  // builds a line cable
  LineCable line_cable = *factory::BuildLineCable();
  LineCableConnection connection;

  line_cable.ClearConnections();

  connection.line_structure = &(*std::next(line_structures->cbegin(), 0));
  connection.index_attachment = 1;
  line_cable.AddConnection(connection);

  connection.line_structure = &(*std::next(line_structures->cbegin(), 1));
  connection.index_attachment = 1;
  line_cable.AddConnection(connection);

  // adds to transmission line and tests if it was sorted correctly
  const int index = t_.AddLineCable(line_cable);
  EXPECT_EQ(0, index);
}

TEST_F(TransmissionLineTest, AddLineStructure) {
  LineStructure line_structure = *factory::BuildLineStructure();
  int index = 0;

  // adds a structure in the middle of an existing alignment segment
  line_structure.set_station(100);
  index = t_.AddLineStructure(line_structure);

  EXPECT_EQ(1, index);
  auto iter = std::next(t_.line_structures()->cbegin(), index);
  line_structure = *iter;
  EXPECT_EQ(100, line_structure.station());

  // attempts to add a structure outside alignment
  line_structure.set_station(5000);
  index = t_.AddLineStructure(line_structure);

  EXPECT_EQ(-1, index);

  // attempts to add a structure at an existing station
  line_structure.set_station(100);
  index = t_.AddLineStructure(line_structure);

  EXPECT_EQ(-1, index);
}

TEST_F(TransmissionLineTest, DeleteAlignmentPoint) {
  t_.DeleteAlignmentPoint(1);

  // checks the second alignment point, which was originally the third
  auto iter = std::next(t_.alignment()->points()->cbegin(), 1);
  const AlignmentPoint& point = *iter;
  EXPECT_EQ(2000, point.station);
}

TEST_F(TransmissionLineTest, DeleteLineCable) {
  bool status = false;

  // deletes the second line cable
  status = t_.DeleteLineCable(1);
  EXPECT_TRUE(status);

  // attempts invalid index
  status = t_.DeleteLineCable(5);
  EXPECT_FALSE(status);
}

TEST_F(TransmissionLineTest, DeleteLineStructure) {
  LineStructure line_structure;
  bool status = false;

  // deletes the second structure
  status = t_.DeleteLineStructure(1);

  EXPECT_TRUE(status);
  auto iter = std::next(t_.line_structures()->cbegin(), 1);
  line_structure = *iter;
  EXPECT_EQ(2000, line_structure.station());

  // attempts invalid index
  status = t_.DeleteLineStructure(5);
  EXPECT_FALSE(status);
}

TEST_F(TransmissionLineTest, ModifyAlignmentPoint) {
  int index = 0;

  AlignmentPoint point;
  point.elevation = 0;
  point.rotation = 0;
  point.station = 5000;

  // modifies the second alignment point and tests re-sort
  index = t_.ModifyAlignmentPoint(1, point);
  EXPECT_EQ(4, index);

  // checks second alignment point, which was originally the third
  auto iter = std::next(t_.alignment()->points()->cbegin(), 1);
  point = *iter;
  EXPECT_EQ(2000, point.station);

  // checks fifth alignment point, which should be the re-sorted/modified one
  iter = std::next(t_.alignment()->points()->cbegin(), 4);
  point = *iter;
  EXPECT_EQ(5000, point.station);
}

TEST_F(TransmissionLineTest, ModifyLineCable) {
  Hardware* hardware_deadend = factory::BuildHardware();
  hardware_deadend->type = Hardware::HardwareType::kDeadEnd;

  const std::list<LineStructure>* line_structures = t_.line_structures();
  LineStructure line_structure;

  // modifies first line structures and adds dead-end hardware
  line_structure = *std::next(line_structures->cbegin(), 0);
  line_structure.AttachHardware(1, hardware_deadend);
  t_.ModifyLineStructure(0, line_structure);

  // modifies first line cable to use new attachment
  LineCable line_cable = t_.line_cables()->front();
  LineCableConnection connection = line_cable.connections()->front();
  connection.index_attachment = 1;
  line_cable.ModifyConnection(0, connection);

  // adds to transmission line and tests if it was sorted correctly
  const int index = t_.ModifyLineCable(0, line_cable);
  EXPECT_EQ(0, index);
}

TEST_F(TransmissionLineTest, ModifyLineStructure) {
  LineStructure line_structure = *factory::BuildLineStructure();
  bool status = false;

  // modifies the second structure
  line_structure.set_station(1500);
  status = t_.ModifyLineStructure(1, line_structure);

  EXPECT_TRUE(status);
  auto iter = std::next(t_.line_structures()->cbegin(), 1);
  line_structure = *iter;
  EXPECT_EQ(1500, line_structure.station());

  // attempts to modify last structure station outside alignment
  line_structure.set_station(5000);
  status = t_.ModifyLineStructure(4, line_structure);

  EXPECT_FALSE(status);

  // attempts to modify a structure to an existing station
  line_structure.set_station(3000);
  status = t_.ModifyLineStructure(4, line_structure);

  EXPECT_FALSE(status);
}

TEST_F(TransmissionLineTest, PointsXyzAlignment) {
  const std::vector<Point3d>* points_xyz = t_.PointsXyzAlignment();
  Point3d point;

  // checks all xyz alignment points
  point = points_xyz->at(0);
  EXPECT_EQ(0, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = points_xyz->at(1);
  EXPECT_EQ(1000, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = points_xyz->at(2);
  EXPECT_EQ(1000, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = points_xyz->at(3);
  EXPECT_EQ(2000, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(100, helper::Round(point.z, 2));

  point = points_xyz->at(4);
  EXPECT_EQ(3000, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));
}

TEST_F(TransmissionLineTest, PointsXyzLineStructures) {
  const std::vector<Point3d>* points_xyz = t_.PointsXyzLineStructures();
  Point3d point;

  // checks all xyz alignment points
  point = points_xyz->at(0);
  EXPECT_EQ(0, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = points_xyz->at(1);
  EXPECT_EQ(1000, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = points_xyz->at(2);
  EXPECT_EQ(1000, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = points_xyz->at(3);
  EXPECT_EQ(2000, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(100, helper::Round(point.z, 2));

  point = points_xyz->at(4);
  EXPECT_EQ(3000, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));
}

TEST_F(TransmissionLineTest, PointXyzAlignment) {
  Point3d point;

  // tests points in the middle of an alignment
  point = t_.PointXyzAlignment(500);
  EXPECT_EQ(500, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = t_.PointXyzAlignment(1500);
  EXPECT_EQ(1000, helper::Round(point.x, 2));
  EXPECT_EQ(500, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = t_.PointXyzAlignment(2500);
  EXPECT_EQ(1500, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(50, helper::Round(point.z, 2));

  point = t_.PointXyzAlignment(3500);
  EXPECT_EQ(2500, helper::Round(point.x, 2));
  EXPECT_EQ(1000, helper::Round(point.y, 2));
  EXPECT_EQ(50, helper::Round(point.z, 2));
}

TEST_F(TransmissionLineTest, PointXyzLineStructure) {
  Point3d point;

  // checks some points, most are checked in another test
  point = t_.PointXyzLineStructure(0);
  EXPECT_EQ(0, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));

  point = t_.PointXyzLineStructure(1);
  EXPECT_EQ(1000, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(0, helper::Round(point.z, 2));
}

TEST_F(TransmissionLineTest, PointXyzLineStructureAttachment) {
  Point3d point;
  LineStructure line_structure;

  // builds a structure with a complicated attachment
  Structure* structure = factory::BuildStructure();
  StructureAttachment& attachment = structure->attachments.at(0);
  attachment.offset_longitudinal = 10;
  attachment.offset_transverse = 10;
  attachment.offset_vertical_top = 10;

  // modifies first structure with new base structure and checks coordinates
  // this structure is at the origin and is not rotated
  line_structure = *std::next(t_.line_structures()->cbegin(), 0);
  line_structure.set_structure(structure);
  t_.ModifyLineStructure(0, line_structure);

  point = t_.PointXyzLineStructureAttachment(0, 0);
  EXPECT_EQ(10, helper::Round(point.x, 2));
  EXPECT_EQ(-10, helper::Round(point.y, 2));
  EXPECT_EQ(110, helper::Round(point.z, 2));

  /// modifes first structure to also have an offset
  line_structure.set_offset(10);
  line_structure.set_structure(structure);
  t_.ModifyLineStructure(0, line_structure);

  point = t_.PointXyzLineStructureAttachment(0, 0);
  EXPECT_EQ(10, helper::Round(point.x, 2));
  EXPECT_EQ(-20, helper::Round(point.y, 2));
  EXPECT_EQ(110, helper::Round(point.z, 2));

  // modifies first structure to also have a rotation
  line_structure.set_rotation(90);
  line_structure.set_structure(structure);
  t_.ModifyLineStructure(0, line_structure);

  point = t_.PointXyzLineStructureAttachment(0, 0);
  EXPECT_EQ(10, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(110, helper::Round(point.z, 2));

  // modifies second structure and checks coordinates
  // this structure is on an angled alignment point, and should auto rotate
  line_structure = *std::next(t_.line_structures()->cbegin(), 1);
  line_structure.set_structure(structure);
  t_.ModifyLineStructure(1, line_structure);

  point = t_.PointXyzLineStructureAttachment(1, 0);
  EXPECT_EQ(1014.14, helper::Round(point.x, 2));
  EXPECT_EQ(0, helper::Round(point.y, 2));
  EXPECT_EQ(110, helper::Round(point.z, 2));
}

TEST_F(TransmissionLineTest, Validate) {
  EXPECT_TRUE(t_.Validate(true, nullptr));
}
