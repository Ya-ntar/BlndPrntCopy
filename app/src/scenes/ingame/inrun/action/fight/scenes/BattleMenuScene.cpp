//
// Created by user on 11.05.2025.
//
#include "BattleMenuScene.h"
#include "../InFightManager.h"

void BattleMenuScene::loadGraphics() {
  base.load();
  if (auto backButton = base.getGui().get<tgui::Button>("back")) {
    backButton->setText("Run away");
    backButton->onPress([&] {
      parent->runAway();
    });
  }

  if (auto info = base.getGui().get<tgui::TextArea>("menu_info")){
    info->setText("Battle Menu");
  }
  tgui::Gui tempGui;
  try { tempGui.loadWidgetsFromFile(path); }
  catch (const tgui::Exception &e) {
    std::cerr << "Failed to load Battle Menu widgets: " << e.what() << std::endl;
    throw e;
  }

  if (auto act = tempGui.get<tgui::Button>("act")) {
    act->setText(parent->getCurrentTurn());
    act->onPress([&] {
      parent->startTurn();
    });
  }
  if (auto inventory = tempGui.get<tgui::Button>("inventory")) {
    inventory->onPress([&] {
      parent->showInventory();
    });
  }

  {
    auto layout = tempGui.get<tgui::Group>("grp");
    tempGui.remove(layout);
    base.getMainFreeSpace()->add(layout);
    layout->setPosition({"1%", "0%"});
    layout->setSize({"50%", "50%"});
  }
}

