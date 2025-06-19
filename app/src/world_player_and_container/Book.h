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
#include <memory>

using BookID = std::size_t;

class Book {
  static std::atomic<BookID> globalIdCounter;
  BookID id{globalIdCounter++};
  std::string name;
  size_t lvl;
  int32_t damage_per_hit{};
  int32_t extra_time{};
  BoundedValue durability;
  std::unique_ptr<TextSource> textSource_;  // Владение здесь
  TextHolder textHolder;
  bool infiniteDurability = false;

 public:
  Book(std::string name,
       int lvl,
       int32_t damage_per_hit,
       int32_t extra_time,
       BoundedValue durability,
       std::unique_ptr<TextSource> textSource,
       size_t bufferSize,
       bool cleanAutomatically,
       bool infiniteDurability = false)
      : name(std::move(name)),
        lvl(lvl),
        damage_per_hit(damage_per_hit),
        extra_time(extra_time),
        durability(durability),
        textSource_(std::move(textSource)),  // Сохраняем владение
        textHolder(bufferSize, "", 0, *textSource_, cleanAutomatically),
        infiniteDurability(infiniteDurability) {}

  [[nodiscard]] BookID getId() const { return id; }
  TextHolder &getTextHolder() { return textHolder; }
  const std::string &getName() const { return name; }

  [[nodiscard]] size_t getDurability() const {
    if (infiniteDurability) return durability.get_max_value();
    return durability.get_current_value();
  }
  bool hasInfiniteDurability() const { return infiniteDurability; }

  [[nodiscard]] size_t getLevel() const { return lvl; }
  [[nodiscard]] int32_t getDamagePerHit() const { return damage_per_hit; }
  [[nodiscard]] int32_t getExtraTime() const { return extra_time; }
};

inline std::atomic<BookID> Book::globalIdCounter{0};