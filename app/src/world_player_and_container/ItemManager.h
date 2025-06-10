//
// Created by user on 16.05.2025.
//

#pragma once

#include <set>
#include "Item.h"
#include <stdexcept>

class ItemManager {
  std::map<Item, size_t> items;

 public:
  bool takeItem(const Item &item) {
    if (static_cast<unsigned int>(items.contains(item)) != 0U) {
      items[item]--;
      if (items[item] == 0) {
        items.erase(item);
      }
      return true;
    }
    return false;
  }

  void removeAllItems(const Item &item) {
    items.erase(item);
  }

  [[nodiscard]] size_t count(const Item &item) const {
    return items.count(item);
  }

  [[nodiscard]] bool contains(const Item &item) const {
    return items.contains(item);
  }

  [[nodiscard]] const std::map<Item, size_t> &getItemMap() const {
    return items;
  }

  void addItem(const Item &item) {
    if (static_cast<unsigned int>(items.contains(item)) != 0U) {
      items[item]++;
    } else {
      items.try_emplace(item, 1);
    }

  }

  void useItem(ItemAffected const &context, const Item &whatToUse) {
    if (!contains(whatToUse)) {
      throw std::invalid_argument("ItemManager: item not present");
    }
    if (context.useItem(whatToUse) && !takeItem(whatToUse)) {
      throw std::invalid_argument("ItemManager: failed to take item after use");
    }
  }
};
