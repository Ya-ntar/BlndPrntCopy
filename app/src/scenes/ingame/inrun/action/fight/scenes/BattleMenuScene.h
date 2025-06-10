//
// Created by user on 08.05.2025.
//

#pragma once

#include <utility>
#include <iostream>

#include "../../../../../SceneType.h"
#include "../../../../../Scene.h"
#include "../../../base/Layout.h"

class InFightManager;
class BattleMenuScene : public Scene {
  std::string path;
  Layout &base;
  InFightManager *parent;
 public:
  explicit BattleMenuScene(InFightManager *parent, Layout &base,
                           std::string path = "forms/fighting_menu_widgets.txt") :
      parent(parent), Scene(base.getContext()), base(base), path(std::move(path)) {
  };

  void loadGraphics() override;
  void clear() override {

  }

  void handleEvent(const std::optional<sf::Event> &_event) override {};
};

