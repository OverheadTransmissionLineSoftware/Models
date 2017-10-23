// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGGING_CABLESAGPOSITIONCORRECTOR_H_
#define OTLS_MODELS_SAGGING_CABLESAGPOSITIONCORRECTOR_H_

#include <list>
#include <vector>

#include "models/base/point.h"
#include "models/transmissionline/catenary.h"
#include "models/transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This class solves for the corrections needed to model a cable being sagged
/// in pulleys.
///
/// \par CLIPPED VS PULLEY MODELS
///
/// In design, a cable is typically modeled as clipped at suspension points.
/// This indicates that it has been attached to a clamp that does not permit
/// slack transfer between spans. This model assumes that the HORIZONTAL tension
/// between the back and ahead spans are equal, and that the suspension
/// string(s) will be plumb.
///
/// While stringing, a cable is always in pulleys at suspension points to allow
/// the tension of the cable to be adjusted. This model assumes that the SUPPORT
/// tension between the back and ahead spans are equal. This may or may not
/// cause the suspension string(s) to be plumb.
///
/// While these models will have varying cable sags/lengths in each span, they
/// both will have the same total cable length for all spans.
///
/// \par SAG CORRECTIONS
///
/// Sag corrections translate the sag from the clipped model to the pulley
/// model. These corrections are needed while the cable is being sagged. The
/// corrections are specified for every span, and are calculated by taking the
/// sag difference between the clipped and pulley model. Negative numbers will
/// reduce the clipped model (design) sag, while positive numbers will increase
/// it.
///
/// \par CLIPPING OFFSETS
///
/// Clipping offsets translate the pulley model to the clipped model by
/// adjusting the slack in each span. These offsets are needed after the cable
/// has been sagged. A clipping offset is calculated for every suspension
/// structure. Negative numbers indicate that the clipping offset is
/// back-on-line, while positive numbers indicate that the offset is
/// ahead-on-line.
///
/// \par ASSUMPTIONS
///
/// This class assumes that the cable attachment points do not change between
/// the clipped and pulley models. Modeling the true behavior of the cables in
/// pulleys requires modeling the longitudinal swing of the cable. This requires
/// a finite element method, which this class does not currently implement.
class CableSagPositionCorrector {
 public:
  /// \brief Default constructor.
  CableSagPositionCorrector();

  /// \brief Destructor.
  ~CableSagPositionCorrector();

  /// \brief Gets the clipping offsets.
  /// \return If the clipping offsets.
  std::vector<double> ClippingOffsets() const;

  /// \brief Gets the sag corrections.
  /// \return If the sag corrections.
  std::vector<double> SagCorrections() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the line cable.
  /// \return The line cable.
  const LineCable* linecable() const;

  /// \brief Gets the attachment points.
  /// \return The attachment points.
  const std::vector<Point3d<double>>* points_attachment() const;

  /// \brief Sets the line cable.
  /// \param[in] linecable
  ///   The line cable.
  void set_linecable(const LineCable* linecable);

  /// \brief Sets the attachment points.
  /// \param[in] points_attachment
  ///   The attachment points.
  void set_points_attachment(
      const std::vector<Point3d<double>>* points_attachment);

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets the length of the catenaries.
  /// \param[in] catenaries
  ///   The catenaries.
  /// \return The length of the catenaries.
  double LengthCatenaries(const std::vector<Catenary3d>& catenaries) const;

  /// \brief Solves the catenary tensions for the pulley state.
  /// \param[in] tension_horizontal_start
  ///   The horizontal tension for the starting catenary.
  /// \return If the pulley state tensions were successfully solved for.
  bool SolvePulleyStateTensions(const double& tension_horizontal_start) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///    update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the clipped state.
  /// \return If the clipped state was successfully updated.
  bool UpdateClippedState() const;

  /// \brief Updates the pulley state.
  /// \return If the pulley state was successfully updated.
  bool UpdatePulleyState() const;

  /// \var catenaries_clipped_
  ///   The set of catenaries for the clipped analysis.
  mutable std::vector<Catenary3d> catenaries_clipped_;

  /// \var catenaries_pulleyed_
  ///   The set of catenaries for the pulley analysis.
  mutable std::vector<Catenary3d> catenaries_pulleyed_;

  /// \var is_updated_
  ///   An indicator that tells if the class has been updated.
  mutable bool is_updated_;

  /// \var length_clipped_
  ///   The cable length for the clipped analysis.
  mutable double length_clipped_;

  /// \var length_pulleyed_
  ///   The cable length for the pulley analysis.
  mutable double length_pulleyed_;

  /// \var linecable_
  ///   The line cable.
  const LineCable* linecable_;

  /// \var points_attachment_
  ///   The attachment points.
  const std::vector<Point3d<double>>* points_attachment_;
};

#endif  // OTLS_MODELS_SAGGING_CABLESAGPOSITIONCORRECTOR_H_
