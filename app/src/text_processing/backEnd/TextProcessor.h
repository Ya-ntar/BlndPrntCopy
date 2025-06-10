//
// Created by user on 13.04.2025.
//

#pragma once

#include "tools/TextHolder.h"

namespace textProcessing {
class TextProcessor {
  TextHolder &textHolder;
  bool isActive = true;

 private:

 public:
  explicit TextProcessor(
      TextHolder &textHolder) : textHolder(textHolder) {};

  TextHolder &getTextHolder() {
    return textHolder;
  }

  void turnOff() {
    isActive = false;
  }

  void turnOn() {
    isActive = true;
  }

  void next() {
    textHolder.next();
  }

  [[nodiscard]] bool isEmpty() const {
    return textHolder.isEmpty();

  };

  CharCheckState checkKey(const string &key) {
    if (textHolder.isEmpty() || !isActive) {
      return CharCheckState::NOTHING;
    }
    string waitedChar = textHolder.getFirstChar();
    if (key == waitedChar) {
      return CharCheckState::CORRECT_INPUT;
    }
    return CharCheckState::INCORRECT_INPUT;

  }

};
}  // namespace textProcessing

