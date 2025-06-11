//
// Created by user on 16.04.2025.
//

#pragma once

#include <cstdint>
#include <utility>
#include "BoundedValue.h"
#include "../text_processing/backEnd/tools/TextSource.h"
#include "../text_processing/backEnd/tools/TextHolder.h"
#include <atomic>
#include <map>
#include <string>

using BookID = std::size_t;

class Book {
  static std::atomic<BookID> globalIdCounter;
  BookID id{globalIdCounter++};
  std::string name;
  size_t lvl;
  int32_t damage_per_hit{};
  int32_t extra_time{};
  BoundedValue durability;
  TextHolder textHolder;
  bool infiniteDurability = false;

 public:
  Book(std::string name,
       size_t lvl,
       int32_t damage_per_hit,
       int32_t extra_time,
       BoundedValue durability,
       TextSource &textSource,
       size_t bufferSize,
       bool cleanAutomatically,
       bool infiniteDurability = false)
      :
      name(std::move(name)), lvl(lvl),
      damage_per_hit(damage_per_hit), extra_time(extra_time),
      durability(durability),
      textHolder(bufferSize, "", 0, textSource, cleanAutomatically),
      infiniteDurability(infiniteDurability) {}

  [[nodiscard]] BookID getId() const { return id; }
  TextHolder &getTextHolder() { return textHolder; }
  const std::string &getName() const { return name; }

  [[nodiscard]] size_t getDurability() const {
    if (infiniteDurability) return durability.get_max_value();
    return durability.get_current_value();
  }
  bool hasInfiniteDurability() const { return infiniteDurability; }
};

inline std::atomic<BookID> Book::globalIdCounter{0};