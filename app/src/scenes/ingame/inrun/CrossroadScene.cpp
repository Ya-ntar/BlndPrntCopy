//
// Created by user on 28.05.2025.
//
#include "CrossroadScene.h"
#include "../InGameManager.h"
#include "base/InventoryButtonUtil.h"

void CrossroadScene::makeMenuButtons() {
  if (auto backButton = menuBase.getGui().get<tgui::Button>("back")) {
    backButton->setText("Run away");
    backButton->onPress([this] {
      parent->ranAwayHome();
    });
  }

  auto layout = tgui::HorizontalLayout::create();
  layout->setWidgetName("layout");
  auto act = InventoryButtonUtil::createStyledButton("Go");
  act->onPress([this] {
    SceneInstruction const *chosen;
    for (auto const *p_instruction : selected) {
      chosen = p_instruction;
      break;
    }
    parent->chooseScene(*chosen);
  });
  layout->add(act);
  InventoryButtonUtil::addInventoryButton(layout, parent, this);
  BookButtonUtil::addBookButton(layout, parent);
  {
    menuBase.getMainFreeSpace()->add(layout);
    WidgetConfigurator::configureMenuLayout(layout);
  }
}

