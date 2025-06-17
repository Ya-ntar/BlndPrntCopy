//
// Created by user on 08.05.2025.
//

#pragma once

#include <utility>
#include <iostream>


#include "../../../../../SceneType.h"
#include "../../../base/Layout.h"
#include "../../../../../../scenes/Scene.h"

class InFightManager;
class BattleMenuScene : public Scene {
  Layout &menuBase;
  InFightManager *parent;
 public:
  explicit BattleMenuScene(InFightManager *parent, Layout &menuBase) :
      Scene(menuBase.getContext()),
      menuBase(menuBase), parent(parent) {
  };

  void loadGraphics() override;
  void clear() override {

  }

  void handleEvent(const std::optional<sf::Event> &_event) override {};
};

