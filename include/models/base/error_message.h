// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_BASE_ERRORMESSAGE_H_
#define OTLS_MODELS_BASE_ERRORMESSAGE_H_

#include <string>

/// \par OVERVIEW
///
/// This struct contains information for an error message.
struct ErrorMessage {
 public:
  /// \var description
  ///   The description.
  std::string description;

  /// \var title
  ///   The title.
  std::string title;
};

#endif  // OTLS_MODELS_BASE_ERRORMESSAGE_H_