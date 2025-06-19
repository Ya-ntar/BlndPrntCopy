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
  int money;
};

class InRunManager : public SceneManager<State::InRun> {
 private:
  SceneInstruction nextScene;
  InGameManager *parent;
  RunInfo run_info_;
  Layout layout;

  SceneGenerator gen;

  void subscribeAll();

 public:
  InRunManager(InGameManager *parent, GameContext &context);

  void showInventory(ItemAffected &mainScene);

  void showChangeArmour(ItemAffected &mainScene);
  void showBookMenu();
  void hideChangeArmour() {//toDo: добавить эксепшн
    deleteScene();
  }
  void hideInventory() { //toDo: добавить эксепшн
    deleteScene();
  }
  void hideBookMenu() { deleteScene(); }

  void load() override {
    subscribeAll();
    changeScene(State::InRun::CROSSROAD);
  };

  void ranAwayFromAFight();

  void playerDeath(const player::PlayerDeath &death);
  void prepare() override {
    layout.load();
  }
  std::unique_ptr<Scene> createScene(State::InRun scene) override;
  void chooseScene(const SceneInstruction &scene) {
    nextScene = scene;
    changeScene(State::InRun::SCENE);
  }
  void ranAwayHome();
  void winAFight(const FightInfo &info_1);
};