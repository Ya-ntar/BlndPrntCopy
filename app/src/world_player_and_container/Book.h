//
// Created by user on 16.04.2025.
//

#pragma once

#include <cstdint>
#include <utility>
#include "BoundedValue.h"
#include "../text_processing/backEnd/tools/TextSource.h"
#include "../text_processing/backEnd/tools/TextHolder.h"

class BookInfo {
  string name;
  size_t lvl;
  int32_t damage_per_hit{};
  int32_t extra_time{};
  BoundedValue durability;
  TextHolder &textHolder; // Владение?

 public:
  BookInfo(string name, size_t lvl, int32_t damage_per_hit, int32_t extra_time,
           BoundedValue durability, TextHolder &textHolder) : name(std::move(name)), lvl(lvl),
                                                              damage_per_hit(damage_per_hit), extra_time(extra_time),
                                                              durability(durability), textHolder(textHolder) {};
};