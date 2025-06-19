//
// Created by user on 11.05.2025.
//
#include "BattleMenuScene.h"
#include "../InFightManager.h"
#include "../../../base/InventoryButtonUtil.h"
#include "../../../../../../utils/WidgetConfigurator.h"

void BattleMenuScene::loadGraphics() {
  if (auto backButton = context.gui.get<tgui::Button>("back")) {
    backButton->setText("Run away");
    backButton->onPress([this] {
      parent->runAway();
    });
  }
  if (auto info = context.gui.get<tgui::TextArea>("menu_info")) {
    info->setText("Battle Menu");
  }
  auto layout = tgui::HorizontalLayout::create();
  layout->setWidgetName("layout");
  auto act = InventoryButtonUtil::createStyledButton(parent->getCurrentTurn());
  act->onPress([this] {
    parent->startTurn();
  });
  layout->add(act);
  InventoryButtonUtil::addInventoryButton(layout, parent);
  auto main = context.gui.get<tgui::Panel>("main_free_space");
  main->add(layout);
  WidgetConfigurator::configureMenuLayout(layout);
}

void BattleMenuScene::clear() {
  auto main = context.gui.get<tgui::Panel>("main_free_space");
  main->removeAllWidgets();
}

