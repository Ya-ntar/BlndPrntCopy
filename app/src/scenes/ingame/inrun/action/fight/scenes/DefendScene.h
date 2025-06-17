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

class DefendScene : public Scene {
  std::string path;
  Layout &base;
  RunInfo &runInfo;
  FightInfo &fightInfo;
  InFightManager *parent;
 public:
  explicit DefendScene(InFightManager *parent, Layout &base, RunInfo &runInfo, FightInfo &fightInfo,
                       std::string path = "forms/fighting_menu_widgets.txt") :
      Scene(base.getContext()), path(std::move(path)), base(base), runInfo(runInfo),
      fightInfo(fightInfo), parent(parent) {
  };

  void loadGraphics() override;

  void clear() override {

  }

  void handleEvent(const std::optional<sf::Event> &_event) override {};
};

