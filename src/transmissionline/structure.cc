// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/structure.h"

StructureAttachment::StructureAttachment() {
  offset_longitudinal = -999999;
  offset_transverse = -999999;
  offset_vertical_top = -999999;
}

StructureAttachment::~StructureAttachment() {
}

bool StructureAttachment::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "STRUCTURE ATTACHMENT";

  // validates offset-vertical-top
  if (offset_vertical_top < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical offset from top of structure";
      messages->push_back(message);
    }
  }

  return is_valid;
}


Structure::Structure() {
  height = -999999;
  name = "";
}

Structure::~Structure() {
}

bool Structure::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "STRUCTURE";

  // validates attachments
  if (attachments.size() == 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "No attachments defined";
      messages->push_back(message);
    }
  } else {
    for (auto iter = attachments.cbegin(); iter != attachments.cend(); iter++) {
      const StructureAttachment& attachment = *iter;
      attachment.Validate(is_included_warnings, messages);
    }
  }

  // validates height
  if (height <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid height";
      messages->push_back(message);
    }
  }

  if (attachments.size() != 0) {
    for (auto iter = attachments.cbegin(); iter != attachments.cend(); iter++) {
      const StructureAttachment& attachment = *iter;
      if (height < attachment.offset_vertical_top) {
        is_valid = false;
        if (messages != nullptr) {
          message.description = "Invalid attachment vertical offset. Exceeds "
                                "structure height";
          messages->push_back(message);
        }
      }
    }
  }

  return is_valid;
}
