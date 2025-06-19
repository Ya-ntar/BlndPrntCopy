#pragma once

#include "../../../../SceneType.h"
#include "../../base/Layout.h"
#include "scenes/BattleMenuScene.h"
#include "scenes/AtkScene.h"
#include <vector>
#include <string>
#include "ItemReactionConfigurable.h"

static const std::string kItemConfig = "assets/gameResources/scenes/battle_scene.json";
using turnType = std::string; //toDo: change later

class InRunManager;

class InFightManager : public SceneManager<State::InFight>, public ItemReactionConfigurable {
  RunInfo &run_info_;
  FightInfo info;
  std::shared_ptr<mob::MobSelectable> mob_selectable_;
  Layout &layout;
  InRunManager *parent;
  turnType turn = "Attack";

  std::unique_ptr<Scene> createScene(State::InFight /*unused*/) override;

 public:
  InFightManager(InRunManager *parent, GameContext &context, RunInfo &runInfo, FightInfo &fightInfo, Layout &layout);

  void load() override {
    layout.setMiddle(mob_selectable_);
    changeScene(State::InFight::BattleMenu);
  };

  void updateMobView() {
    mob_selectable_->load();
  }
  void clear() override {
    layout.clear();
  };

  [[nodiscard]] turnType getCurrentTurn() const {
    return turn;
  }

  void startTurn();

  void showInventory();

  void runAway();

  void damageMobs(fight::AtkEnded msg);
  void prepare() override {
    layout.load();
  }

  void subscribeToAll();
};