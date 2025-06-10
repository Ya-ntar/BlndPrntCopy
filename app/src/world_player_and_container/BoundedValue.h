#pragma once

#include <cstddef>
#include <algorithm>

class BoundedValue {
  int32_t max_value;
  int32_t current_value;
 public:
  BoundedValue(size_t max_value,
               size_t current_value) : max_value(max_value), current_value(current_value) {};

  void set_max_value(size_t v) {
    max_value = v;
  }

  void set_current_value(size_t v) {
    current_value = v;
  }

  void restore_all() {
    current_value = max_value;
  }

  void damage(int32_t how_much) {
    current_value = std::max(static_cast<int32_t>(0), current_value - how_much);
  }

  void heal(int32_t how_much) {
    current_value = std::min(max_value, current_value + how_much);
  }

  [[nodiscard]] size_t get_max_value() const {
    return max_value;
  }

  [[nodiscard]] size_t get_current_value() const {
    return current_value;
  }

};