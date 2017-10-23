// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagging/cable_sag_position_corrector.h"

#include <cmath>

#include "models/transmissionline/catenary_solver.h"

CableSagPositionCorrector::CableSagPositionCorrector() {
  linecable_ = nullptr;
  points_attachment_ = nullptr;

  catenaries_clipped_.clear();
  catenaries_pulleyed_.clear();
  length_clipped_ = -999999;
  length_pulleyed_ = -999999;

  is_updated_ = false;
}

CableSagPositionCorrector::~CableSagPositionCorrector() {
}

std::vector<double> CableSagPositionCorrector::ClippingOffsets() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return std::vector<double>();
    }
  }

  // creates a container
  const int kSize = catenaries_clipped_.size() + 1;
  std::vector<double> offsets(kSize);

  // generates offsets for every structure
  double length_clipped = 0;
  double length_pulleyed = 0;
  for (int i = 0; i < kSize; i++) {
    if ((i == 0) || (i == kSize - 1)) {
      // sets offsets to zero on boundary structures
      offsets[i] = 0;
    } else {
      // calculates clipping offset at suspension structure
      // uses previous span catenaries
      const Catenary3d& catenary_clipped = catenaries_clipped_[i - 1];
      const Catenary3d& catenary_pulleyed = catenaries_pulleyed_[i - 1];

      // calculates catenary length from the start
      length_clipped += catenary_clipped.Length();
      length_pulleyed += catenary_pulleyed.Length();

      // calculates the offset
      offsets[i] = length_clipped - length_pulleyed;
    }
  }

  return offsets;
}

std::vector<double> CableSagPositionCorrector::SagCorrections() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return std::vector<double>();
    }
  }

  // creates a container
  const int kSize = catenaries_clipped_.size();
  std::vector<double> corrections(kSize);

  // calculates a correction for each span
  for (int i = 0; i < kSize; i++) {
    // gets both catenaries
    const Catenary3d& catenary_clipped = catenaries_clipped_[i];
    const Catenary3d& catenary_pulleyed = catenaries_pulleyed_[i];

    // calculates sag for each
    const double sag_clipped = catenary_clipped.Sag();
    const double sag_pulleyed = catenary_pulleyed.Sag();

    // calculates the correction
    corrections[i] = sag_pulleyed - sag_clipped;
  }

  return corrections;
}

bool CableSagPositionCorrector::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE SAG POSITION CORRECTOR";

  // validates linecable
  if (linecable_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid line cable";
      messages->push_back(message);
    }
  } else {
    if (linecable_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates points-attachment
  if (points_attachment_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid line cable";
      messages->push_back(message);
    }
  } else {
    if (points_attachment_->size() <= 1) {
      is_valid = false;
    }
  }

  // validates update process
  if (Update() == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Error updating class";
      messages->push_back(message);
    }

    return is_valid;
  }

  return is_valid;
}

const LineCable* CableSagPositionCorrector::linecable() const {
  return linecable_;
}

const std::vector<Point3d<double>>*
    CableSagPositionCorrector::points_attachment() const {
  return points_attachment_;
}

void CableSagPositionCorrector::set_linecable(const LineCable* linecable) {
  linecable_ = linecable;
  is_updated_ = false;
}

void CableSagPositionCorrector::set_points_attachment(
    const std::vector<Point3d<double>>* points_attachment) {
  points_attachment_ = points_attachment;
  is_updated_ = false;
}

bool CableSagPositionCorrector::IsUpdated() const {
  if (is_updated_ == true) {
    return true;
  } else {
    return false;
  }
}

double CableSagPositionCorrector::LengthCatenaries(
    const std::vector<Catenary3d>& catenaries) const {
  double length = 0;

  // sums the length of all the catenaries
  const int kSize = catenaries.size();
  for (int i = 0; i < kSize; i++) {
    const Catenary3d& catenary = catenaries[i];
    length += catenary.Length();
  }

  return length;
}

bool CableSagPositionCorrector::SolvePulleyStateTensions(
    const double& tension_horizontal_start) const {
  // initializes
  CatenarySolver solver;
  solver.set_position_target(0);
  solver.set_type_target(CatenarySolver::TargetType::kTension);
  solver.set_weight_unit(catenaries_pulleyed_[0].weight_unit());

  // adjusts the horizontal tension for every catenary
  const Catenary3d* catenary_prev = nullptr;
  const int kSize = catenaries_pulleyed_.size();
  for (int i = 0; i < kSize; i++) {
    // gets the catenary for the span of interest
    Catenary3d& catenary = catenaries_pulleyed_[i];

    // selects tension update method
    if (i == 0) {
      // horizontal tension is already specified
      catenary.set_tension_horizontal(tension_horizontal_start);
    } else {
      // solves for the horizontal tension
      // back support tension of the current catenary is set to match the
      // ahead support tension of the previous catenary
      solver.set_spacing_endpoints(catenary.spacing_endpoints());
      solver.set_value_target(catenary_prev->Tension(1));

      if (solver.Validate() == true) {
        catenary.set_tension_horizontal(solver.TensionHorizontal());
      } else {
        return false;
      }
    }

    // increments
    catenary_prev = &catenary;
  }

  return true;
}

bool CableSagPositionCorrector::Update() const {
  // updates the clipped state
  is_updated_ = UpdateClippedState();
  if (is_updated_ == false) {
    return false;
  }

  // updates the pulley state
  is_updated_ = UpdatePulleyState();
  if (is_updated_ == false) {
    return false;
  }

  // if it reaches this point, update was successful
  return true;
}

bool CableSagPositionCorrector::UpdateClippedState() const {
  // initializes
  catenaries_clipped_.clear();
  length_clipped_ = -999999;

  // solves for a catenary for the entire line cable section
  Catenary3d catenary_section = linecable_->CatenaryRulingSpan();
  if (catenary_section.Validate() == false) {
    return false;
  }

  // creates catenaries for each span
  int kSizeSpans = points_attachment_->size() - 1;
  catenaries_clipped_.resize(kSizeSpans);

  const int kSizeCatenaries = catenaries_clipped_.size();
  for (int i = 0; i < kSizeCatenaries; i++) {
    // solves for attachment spacing
    const Point3d<double>& point_back = (*points_attachment_)[i];
    const Point3d<double>& point_ahead = (*points_attachment_)[i + 1];

    Vector3d spacing_endpoints;
    spacing_endpoints.set_x(point_ahead.x - point_back.x);
    spacing_endpoints.set_y(point_ahead.y - point_back.y);
    spacing_endpoints.set_z(point_ahead.z - point_back.z);

    // updates catenary for span
    Catenary3d& catenary = catenaries_clipped_[i];
    catenary.set_direction_transverse(AxisDirectionType::kPositive);
    catenary.set_spacing_endpoints(spacing_endpoints);
    catenary.set_tension_horizontal(catenary_section.tension_horizontal());
    catenary.set_weight_unit(catenary_section.weight_unit());
  }

  // caches the length of the catenaries
  length_clipped_ = LengthCatenaries(catenaries_clipped_);

  return true;
}

bool CableSagPositionCorrector::UpdatePulleyState() const {
  // initializes
  catenaries_pulleyed_ = catenaries_clipped_;
  length_pulleyed_ = -999999;

  // x = start horizontal tension
  // y = summed catenary lengths (pulley model)

  // initializes target
  double target_solution = length_clipped_;

  // declares and initializes left point
  Point2d<double> point_left;
  point_left.x = catenaries_pulleyed_[0].tension_horizontal();
  if (SolvePulleyStateTensions(point_left.x) == false) {
    return false;
  }
  point_left.y = LengthCatenaries(catenaries_pulleyed_);

  // declares and initializes right point
  Point2d<double> point_right;
  point_right.x = point_left.x * 1.10;
  if (SolvePulleyStateTensions(point_right.x) == false) {
    return false;
  }
  point_right.y = LengthCatenaries(catenaries_pulleyed_);

  // declares and initializes current point
  Point2d<double> point_current;

  // adjusts the pulleyed catenaries until the length is within tolerance
  int iter = 0;
  const double kPrecisionX = 0.1;
  const double kPrecisionY = 0.01;
  const int kIterMax = 100;
  while (kPrecisionX < std::abs(point_left.x - point_right.x)
      && (kPrecisionY < std::abs(point_current.y - target_solution))
      && (iter <= kIterMax)) {
    // gets current point x value using left and right points
    // calculates tangent line between points, extrapolates using line
    double slope_line = (point_right.y - point_left.y)
                        / (point_right.x - point_left.x);

    point_current.x = point_left.x
                      + ((target_solution - point_left.y) / slope_line);

    // gets current point y value
    if (SolvePulleyStateTensions(point_current.x) == false) {
      return false;
    }

    point_current.y = LengthCatenaries(catenaries_pulleyed_);

    // updates either left or right point based on current point
    if (point_current.x < point_left.x) {

      point_right.x = point_left.x;
      point_right.y = point_left.y;
      point_left.x = point_current.x;
      point_left.y = point_current.y;

    } else if ((point_left.x < point_current.x)
        && (point_current.x < point_right.x)) {

      if (point_current.y < target_solution) {
        point_right.x = point_current.x;
        point_right.y = point_current.y;
      } else if (target_solution < point_current.y) {
        point_left.x = point_current.x;
        point_left.y = point_current.y;
      }

    } else if (point_right.x < point_current.x) {

      point_left.x = point_right.x;
      point_left.y = point_right.y;
      point_right.x = point_current.x;
      point_right.y = point_current.y;
    }

    iter++;
  }

  // returns success status
  if (iter < kIterMax) {
    length_pulleyed_ = point_current.y;
    return true;
  } else {
    return false;
  }
}
