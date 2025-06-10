//
// Created by user on 28.05.2025.
//
#include "CrossroadScene.h"
#include "../InGameManager.h"

void CrossroadScene::makeMenuButtons() {
  if (auto backButton = menuBase.getGui().get<tgui::Button>(BACK_BUTTON)) {
    backButton->setText("Run away");
    backButton->onPress([this] {
      parent->ranAwayHome();
    });
  }

  tgui::Gui tempGui;
  try { tempGui.loadWidgetsFromFile("forms/fighting_menu_widgets.txt"); }
  catch (const tgui::Exception &e) {
    std::cerr << "Failed to load Battle Menu widgets: " << e.what() << std::endl;
    throw;
  }

  if (auto act = tempGui.get<tgui::Button>("act")) {
    act->setText("Go");
    act->onPress([this] {
      SceneInstruction const *chosen;
      for (auto const *p_instruction : selected) {
        chosen = p_instruction;
        break;
      }
      parent->chooseScene(*chosen);
    });
  }
  if (auto inventory = tempGui.get<tgui::Button>("inventory")) {
    inventory->onPress([this] {
      parent->showInventory(*this);
    });
  }

  {
    auto layout = tempGui.get<tgui::Group>("grp");
    tempGui.remove(layout);
    menuBase.getMainFreeSpace()->add(layout);
    layout->setPosition({"1%", "0%"});
    layout->setSize({"50%", "50%"});
  }
}

