//
// Created by user on 11.05.2025.
//
#include "HomebaseScene.h"
#include "InGameManager.h"

void HomeBaseScene::loadGraphics() {
  tgui::Gui &gui = context.gui;
  try { gui.loadWidgetsFromFile(path); }
  catch (const tgui::Exception &e) {
    std::cerr << "Failed to load HomeBaseScene " << e.what() << std::endl;
    throw e;
  }

  if (auto startRun = gui.get<tgui::Button>("start_run")) {
    startRun->onPress([&] {
      parent->changeScene(State::InGame::InRun);
    });
  }
}

