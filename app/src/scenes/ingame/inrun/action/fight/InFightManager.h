#pragma once

#include "../../../../SceneType.h"
#include "../../base/Layout.h"
#include "scenes/BattleMenuScene.h"
#include "scenes/AtkScene.h"

using turnType = std::string; //toDo: change later

class InRunManager;

class InFightManager : public SceneManager<State::InFight>, public ItemAffected {
  RunInfo &runInfo;
  FightInfo fightInfo;
  Layout &menuBase;
  InRunManager *parent;
  turnType turn = "Attack";
  std::unique_ptr<Scene> createScene(State::InFight /*unused*/) override;

 public:
  InFightManager(InRunManager *parent, GameContext &context, RunInfo &runInfo, FightInfo &fightInfo, Layout &base);

  void load() override {
    menuBase.setMiddle(std::make_unique<mob::MobSelectable>(fightInfo));
    changeScene(State::InFight::BattleMenu);
  };

  void clear() override {
    menuBase.clear();
  };

  [[nodiscard]] turnType getCurrentTurn() const {
    return turn;
  }
  void startTurn();
  void showInventory();

  void runAway();

  void damageMobs(fight::AtkEnded msg);
  void endDefense() {
    changeScene(State::InFight::BattleMenu);
  }
  void subscribeToAll();
};