#pragma once

#include "main_game_screen/MenuScene.h"
#include "SceneType.h"
#include "../GameContext.h"
#include "ingame/InGameManager.h"

using Main = State::Menu;

class MainManager : public SceneManager<Main> {

  std::unique_ptr<Scene> createScene(Main scene) override {
    switch (scene) {
      case Main::MainMenu:return std::make_unique<MenuScene>(this, context);
      case Main::StartGame:return std::make_unique<InGameManager>(this, context);
      default:return nullptr;
    }
  }

 public:
  using SceneManager::SceneManager;;

  void load() override {
    changeScene(Main::MainMenu);
  };

};