//
// Created by user on 11.05.2025.
//
#include "MenuScene.h"
#include "../MainManager.h"
#include "../../utils/StringUtils.h"
#include "../../utils/ErrorHandler.h"

void MenuScene::load() {
  loadGraphics();
}

void MenuScene::loadGraphics() {
  try {
    gui.removeAllWidgets();
    gui.loadWidgetsFromFile(path);

    if (auto startButton = gui.get<tgui::Button>("Start")) {
      startButton->onPress([this] {
        parent->changeScene(State::Menu::StartGame);
      });
    }

    std::string asciiArt = getStringFromFile(std::string(Assets::kAsciiBlind));

    auto asciiLabel = gui.get<tgui::RichTextLabel>("ascii");
    asciiLabel->setText(asciiArt);
    auto size_font = fmin(gui.getWindow()->getSize().x, gui.getWindow()->getSize().y) / PARAM;
    asciiLabel->setTextSize(static_cast<int>(size_font));

  }
  catch (const tgui::Exception &e) {
    throw error::ResourceLoadError("MainMenu", e.what());
  }
}



