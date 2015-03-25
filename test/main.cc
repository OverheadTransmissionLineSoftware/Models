
#include "base/point.h"
#include "sagtension/catenary_cable_loader.h"
#include "transmissionline/catenary.h"

// When static libraries are built (TLS-Libraries) they are not linked.
// This file contains basic declarations and test routines so that when
// the libraries are built, they are compiled AND linked. This helps remove
// some errors that would be present when linked against an executable.

int main() {

  // base library
  Point2d point;

  // transmission line library
  Catenary3d catenary;

  // sagtension library
  CatenaryCableLoader loader;

  return 0;
}
