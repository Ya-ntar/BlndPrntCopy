#pragma once

#include <string>
#include <map>
#include <functional>
#include <memory>
#include <utility>
#include "TGUI/Backend/SFML-Graphics.hpp"

using id = std::string;
using string = tgui::String;

class Item {
 private:
  id identity;
  std::string name;
 public:
  bool operator==(const Item &other) const {
    return (this->identity == other.identity) && (this->name == other.name);
  }
  bool operator!=(const Item &other) const {
    return !(*this == other);
  }
  bool operator<(const Item &other) const {
    return (identity < other.identity) || (identity == other.identity && name < other.name);
  }

  Item(id item_id, std::string item_name)
      : identity(std::move(item_id)), name(std::move(item_name)) {}

  [[nodiscard]] id getIdentity() const {
    return identity;
  }

  [[nodiscard]] std::string getName() const {
    return name;
  }
};

class ItemAffected {
 private:
  std::map<id, std::function<void(const Item &)>> itemReactions; //toDo: объединить в одну структуру
  std::map<id, string> itemDescription;
 public:

  void addReaction(const id &itemId, std::function<void(const Item &)> reaction,
                   const string &description = "Unknown") {
    itemReactions[itemId] = std::move(reaction);
    itemDescription.insert({itemId, description});
  }

  void removeReaction(const id &itemId) {
    itemReactions.erase(itemId);
    itemDescription.erase(itemId);
  }

  string getDescription(const id &itemId) {
    if (itemDescription.contains(itemId)) {
      return itemDescription[itemId];
    }
    return "Unusable";
  }

  [[nodiscard]] bool useItem(const Item &item) const { //toDo: возможно позднее переделать логику траты предметов
    auto it = itemReactions.find(item.getIdentity());
    if (it != itemReactions.end()) {
      it->second(item);
      return true;
    }
    return false;
  }
};