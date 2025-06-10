//
// Created by user on 10.05.2025.
//
#include "DeathScene.h"
#include "InGameManager.h"

void DeathScene::loadGraphics() {
  tgui::Gui &gui = context.gui;
  try { gui.loadWidgetsFromFile(path); }
  catch (const tgui::Exception &e) {
    std::cerr << "Failed to load HomeBaseScene " << e.what() << std::endl;
    throw e;
  }

  if (auto return_btn = gui.get<tgui::Button>("return")) {
    return_btn->onPress([&] {
      parent->changeScene(State::InGame::Base);
    });
  }
}

