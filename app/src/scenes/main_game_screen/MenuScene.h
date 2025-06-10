#pragma once

#include <iostream>
#include <cmath>
#include <fstream>
#include <utility>

#include "../Scene.h"
#include "../SceneType.h"

#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/RichTextLabel.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

static constexpr const int PARAM = 110; // для высчитывания размеров шрифта (экспериментально)
class MainManager;

class MenuScene : public Scene {
 private:
  MainManager *parent;
  std::string path;
  tgui::Gui &gui;

 public:
  explicit MenuScene(MainManager *parent, GameContext &context,
                     std::string  path = std::string(Assets::FORM_MENU)) :
      Scene(context),
      parent(parent),
      path(std::move(path)),
      gui(context.gui) {};

  void clear() override {
    // usually doesn't need anything special
    };
  void loadGraphics() override;
  void load() override;

  void handleEvent(const std::optional<sf::Event> &_event) override {
    //most scenes don't have special functionality
  };

};