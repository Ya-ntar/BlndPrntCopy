#pragma once

#include <utility>

#include "../DeathScene.h"
#include "SceneGenerator.h"

class InGameManager;

struct RunResult {
  enum class Reason {
    Death,
    BossDefeated,
    RanAway
  } reason;

};

class InRunManager : public SceneManager<State::InRun> {
 private:
  SceneInstruction nextScene;
  InGameManager *parent;
  RunInfo info;
  Layout menuBase;

  SceneGenerator gen;

  void subscribeAll();

 public:
  InRunManager(InGameManager *parent, GameContext &context);

  void showInventory(ItemAffected &mainScene);

  void showChangeArmour(ItemAffected &mainScene);
  void showBookMenu();
  void hideChangeArmour(){//toDo: добавить эксепшн
    deleteScene();
  }
  void hideInventory() { //toDo: добавить эксепшн
    deleteScene();
  }
  void hideBookMenu() { deleteScene(); }

  void load() override {
    subscribeAll();
    changeScene(State::InRun::Crossroad);
  };

  void ranAwayFromAFight();

  void playerDeath(const player::PlayerDeath &death);

  std::unique_ptr<Scene> createScene(State::InRun scene) override;
  void chooseScene(const SceneInstruction &scene) {
    nextScene = scene;
    changeScene(State::InRun::Scene);
  }
  void ranAwayHome();
};