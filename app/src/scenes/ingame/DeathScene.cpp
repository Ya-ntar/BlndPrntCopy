//
// Created by user on 10.05.2025.
//
#include "DeathScene.h"
#include "InGameManager.h"
#include "../../utils/ErrorHandler.h"

void DeathScene::loadGraphics() {
  tgui::Gui &gui = context.gui;
  try { gui.loadWidgetsFromFile(path); }
  catch (const tgui::Exception &e) {
    throw error::ResourceLoadError("DeathScene", e.what());
  }

  if (auto return_btn = gui.get<tgui::Button>("return")) {
    return_btn->onPress([&] {
      parent->changeScene(State::InGame::Base);
    });
  }
}

