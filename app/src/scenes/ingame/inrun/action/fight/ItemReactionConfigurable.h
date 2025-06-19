#pragma once
#include "../../../../../world_player_and_container/Item.h"
#include "InFightManager.h"
#include <string>
struct SceneItemReaction {
  std::string item;
  std::string action;
  int value;
  std::string description;
};

struct SceneTypeConfig {
  std::string scene_type;
  std::vector<SceneItemReaction> item_reactions;
};

class ItemReactionConfigurable : public ItemAffected {

 private:
  RunInfo &runInfo;
  FightInfo &fightInfo;
  static SceneTypeConfig loadSceneTypeConfig(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Cannot open scene config file: " + filename);
    }
    nlohmann::json data = nlohmann::json::parse(file);
    SceneTypeConfig config;
    config.scene_type = data.value("scene_type", "");
    if (data.contains("item_reactions") && data["item_reactions"].is_array()) {
      for (const auto &reaction : data["item_reactions"]) {
        SceneItemReaction r;
        r.item = reaction.value("item", "");
        r.action = reaction.value("action", "");
        r.value = reaction.value("value", 0);
        r.description = reaction.value("description", "");
        config.item_reactions.push_back(r);
      }
    }
    return config;
  }

 public:
  ItemReactionConfigurable(RunInfo &runInfo, FightInfo &fightInfo) : runInfo(runInfo), fightInfo(fightInfo) {};
  void loadFMobReactionsFromConfig(const std::string &filename) {
    SceneTypeConfig config = loadSceneTypeConfig(filename);
    for (const auto &reaction : config.item_reactions) {
      if (reaction.action == "heal_player") {
        int healValue = reaction.value;
        addReaction(reaction.item, [this, healValue](const Item &item) {
          runInfo.player.healHP(healValue);
        }, reaction.description);
      } else if (reaction.action == "damage_enemy") {
        int damageValue = reaction.value;
        addReaction(reaction.item, [this, damageValue](const Item &item) {
          for (auto &mob : fightInfo.currently_under_attack) {
            mob->takeDamage(damageValue);
          }
        }, reaction.description);
      }
    }
  }
}; 