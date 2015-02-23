// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_CABLELOADCASE_H_
#define TRANSMISSIONLINE_CABLELOADCASE_H_

#include <list>
#include <string>

#include "include/SupportLibrary.h"

/**
 * @par CABLE LOAD CASE OVERVIEW
 *
 * This struct contains information for a cable load case.
 */
struct CableLoadCase {
public:
  /**
   * @brief Default constructor.
   */
  CableLoadCase();

  /**
   * @brief Destructor.
   */
  ~CableLoadCase();

  /**
   * @brief Validates class data.
   * @param is_included_warnings A flag that tightens the acceptable value
   *        range.
   * @param messages_error A list of detailed error messages. If this is
   *        provided, any validation errors will be appended to the list.
   * @return A boolean value indicating status of class data.
   */
  bool Validate(bool is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  // member variables
  Vector2D  load_unit;
  double    temperature_cable;
};

#endif  // TRANSMISSIONLINE_CABLELOADCASE_H_
