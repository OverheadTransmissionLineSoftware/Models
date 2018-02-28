// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_BASE_ERROR_MESSAGE_H_
#define MODELS_BASE_ERROR_MESSAGE_H_

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

#endif  // MODELS_BASE_ERROR_MESSAGE_H_
