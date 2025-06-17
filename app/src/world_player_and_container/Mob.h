#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <fstream>
#include <atomic>
#include <iostream>
#include "nlohmann/json.hpp"
#include "../utils/Utils.h"
#include "../utils/ErrorHandler.h"


using ItemID = std::string;
using json = nlohmann::json;

class MobType;

class Mob : public SmartSubscriber {
 private:
  const MobType &type;
  int currentHp;
  const MobID uniqueId;
  static std::atomic<std::size_t> globalIdCounter;

 public:
  explicit Mob(QueuedMessageBus &bus, const MobType &typeRef);

  Mob(Mob &&other) noexcept
      : SmartSubscriber(other.getBus()), //too lazy to make move constr
        type(other.type),
        currentHp(other.currentHp),
        uniqueId(other.uniqueId) {}

  bool operator<(const Mob &other) const {
    return uniqueId < other.uniqueId;
  }

  void beAffectedByItem(const ItemID &item);

  void takeDamage(int amount) {
    currentHp = std::max(0, currentHp - amount);
    if (!isAlive()){
      getBus().add(mob::MobChangedMsg{.val = mob::State::DIED, .target = uniqueId});
    } else {
      getBus().add(mob::MobChangedMsg({.val = mob::State::HP_LESSENED, .target =uniqueId}));
    }

  }
  void subscribeToEverything() {
    // it's okay if not subscribed to anything
  }
  [[nodiscard]] const MobType &getType() const { return type; }

  [[nodiscard]] int getCurrentHp() const { return currentHp; }

  [[nodiscard]] bool isAlive() const { return currentHp > 0; }

  [[nodiscard]] MobID getUniqueId() const { return uniqueId; }

};

class MobType {
 private:
  std::string name;
  std::string imagePath;
  std::string image;
  int maxHp{};
  std::string mainAttackPattern;
  std::string beatingPattern;
  float attackDuration{};
  int attackPower{};

  std::unordered_map<ItemID, std::function<void(Mob &)>> itemReactions;

 public:
  explicit MobType(const json &data) {
    parseJson(data);
    image = getStringFromFile(imagePath);
  }

  void parseJson(const json &data) {
    try {
      name = data.value("name", "Unknown");
      imagePath = data.value("image_path", "");
      maxHp = data.value("hp", 0);
      mainAttackPattern = data.value("main_attack_pattern", "");
      beatingPattern = data.value("beating_pattern", "");
      attackDuration = data.value("attack_duration", 0.0F);
      attackPower = data.value("attack_power", 0);
      if (data.contains("item_reactions") && data["item_reactions"].is_array()) {
        for (const auto &reaction : data["item_reactions"]) {
          parseAndApply(reaction);
        }
      }

    } catch (const json::exception &e) {
      throw std::runtime_error("Invalid MobType JSON format: " + std::string(e.what()));
    }
  }
  void parseAndApply(const nlohmann::basic_json<>& reaction) {
    try {
      std::string event = reaction.value("event", "");
      std::string action = reaction.value("action", "");
      int value = reaction.value("value", 0);

      if (!event.empty() && !action.empty() && action == "lose_hp") {
        itemReactions[event] = [value](Mob &mob) { mob.takeDamage(value); };
      }
    } catch (const json::exception &e) {
      throw error::ValidationError("item_reaction", e.what());
    }
  }

  const std::function<void(Mob &)> &getReactionFor(const ItemID &item) const {
    return itemReactions.at(item);
  }

  const std::string &getName() const { return name; }

  const std::string &getImagePath() const { return imagePath; }

  const std::string &getImage() const { return image; }

  int getMaxHp() const { return maxHp; }

  bool canBeAffected(const ItemID &item) const {
    return itemReactions.contains(item);
  }

  const std::string &getMainAttackPattern() const { return mainAttackPattern; }

  const std::string &getBeatingPattern() const { return beatingPattern; }

  float getAttackDuration() const { return attackDuration; }

  int getAttackPower() const { return attackPower; }

  bool hasReactionFor(const ItemID &item) const {
    return static_cast<unsigned int>(itemReactions.contains(item)) != 0U;
  }
};

class MobDatabase {
 private:
  std::unordered_map<ItemID, MobType> mobTypes;

 public:
  void loadFromJson(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Cannot open file: " + filename);
    }

    try {
      json data = json::parse(file);

      ItemID name = data.value("name", "Unknown");
      mobTypes.try_emplace(name, data);

    } catch (const json::exception &e) {
      throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
  }

  const std::unordered_map<ItemID, MobType> &getMobTypes() const {
    return mobTypes;
  }

  const MobType &getMobType(const ItemID &mobId) const {
    if (!mobTypes.contains(mobId)) {
      throw std::invalid_argument("Unknown mob type: " + mobId);
    }
    return mobTypes.at(mobId);
  }

  Mob createMob(QueuedMessageBus &bus, const ItemID &mobId) const {
    return Mob(bus, getMobType(mobId));

  }
};