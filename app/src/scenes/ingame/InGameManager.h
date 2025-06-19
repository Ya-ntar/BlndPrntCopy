#pragma once

#include "../SceneType.h"
#include "../Scene.h"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "../../GameContext.h"
#include "inrun/InRunManager.h"
#include "HomebaseScene.h"

class MainManager;

class InGameManager : public SceneManager<State::InGame> {
 private:
  MainManager *parent;
  static constexpr int kWinMoney = 10;
  static constexpr int kDeathPenalty = 3;
  struct StableInfo {
    int money = 0;
    int getMoney() const { return money; }
    void addMoney(int amount) { money += amount; }
    void setMoney(int amount) { money = amount; }
  } stableInfo;

  std::unique_ptr<Scene> createScene(State::InGame scene) override {
    switch (scene) {
      using
      enum State::InGame;
      case Base:return std::make_unique<HomeBaseScene>(this, context);
      case InRun:return std::make_unique<InRunManager>(this, context);
      case Death:return std::make_unique<DeathScene>(this, context);
      default:return nullptr;
    }
  }

 public:
  InGameManager(MainManager *parent, GameContext &context);

  void saveReturnToMenu();
  void load() override {
    changeScene(State::InGame::Base);

  }
  void returnToBase(RunResult runResult);

  [[nodiscard]] int getStableMoney() const { return stableInfo.getMoney(); }
  void takeStableMoney(int amount) { stableInfo.addMoney(-amount); }
  void addStableMoney(int amount) { stableInfo.addMoney(amount); }
  void setStableMoney(int amount) { stableInfo.setMoney(amount); }
  [[nodiscard]] int getWinMoney() const { return kWinMoney; }
  [[nodiscard]] bool isWin() const { return stableInfo.getMoney() >= kWinMoney; }
  int getDeathPenalty() const { return kDeathPenalty; }
  MainManager* getParent() const { return parent; }
};

