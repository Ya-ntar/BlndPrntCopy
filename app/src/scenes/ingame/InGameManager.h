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
  class StableInfo {
    int money;
  };

  std::unique_ptr<Scene> createScene(State::InGame scene) override {
    switch (scene) {
      using enum State::InGame;
      case Base:return std::make_unique<HomeBaseScene>(this, context);
      case InRun:return std::make_unique<InRunManager>(this, context);
      case Death:return std::make_unique<DeathScene>(this, context);
      default:return nullptr;
    }
  }

 public:
  InGameManager(MainManager *parent, GameContext &context);

  void load() override {
    changeScene(State::InGame::Base);

  }
  void returnToBase(RunResult runResult);
};

