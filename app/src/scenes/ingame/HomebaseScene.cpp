//
// Created by user on 11.05.2025.
//
#include "HomebaseScene.h"
#include "InGameManager.h"
#include "../../utils/ErrorHandler.h"

void HomeBaseScene::loadGraphics() {
  tgui::Gui &gui = context.gui;
  try { gui.loadWidgetsFromFile(path); }
  catch (const tgui::Exception &e) {
    throw error::ResourceLoadError("HomeBaseScene", e.what());
  }

  if (auto startRun = gui.get<tgui::Button>("start_run")) {
    startRun->onPress([&] {
      parent->changeScene(State::InGame::InRun);
    });
  }
}

