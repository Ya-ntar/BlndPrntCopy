#pragma once

#include "base/SceneInstructionSelectable.h"
// #include "base/InventoryButtonScene.h" // больше не нужно

class InRunManager;

class CrossroadScene : public Scene, public ItemAffected {
  RunInfo &runInfo;
  Layout &menuBase;
  InRunManager *parent;
  SceneGenerator &generator;
  std::vector<SceneInstruction> choices;
  std::set<SceneInstruction *> selected;

 public:
  CrossroadScene(RunInfo &runInfo, Layout &menuBase, InRunManager *parent, SceneGenerator &generator) :
      Scene(menuBase.getContext()),
      runInfo(runInfo), menuBase(menuBase), parent(parent), generator(generator) {};

  void clear() override {
    menuBase.clear();
  };

  void handleEvent(const std::optional<sf::Event> &_event) override {
    // in progress
  };

  void load() override {
    choices = generator.generateInstructions();
    selected.clear();
    selected.insert(choices.data());
    loadGraphics();
  }
  void loadGraphics() override {

    menuBase.setMiddle(std::make_shared<SceneInstructionSelectable>(choices, selected));
    menuBase.load();
    if (auto info = context.gui.get<tgui::TextArea>("menu_info")) {
      info->setText("Crossroad");
    }
    makeMenuButtons();
  }

  void makeMenuButtons();

};
