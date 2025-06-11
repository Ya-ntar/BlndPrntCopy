#pragma once

#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "../../../GameContext.h"
#include "base/RunInfoFront.h"
#include "base/Layout.h"
#include "InRunManager.h"

class InventoryMenu : public Scene {

  RunInfo &runInfo;
  ItemAffected &mainScene;
  Layout &menuBase;
  std::string path = Assets::FORM_INVENTORY;
  InRunManager &parent;

  void reloadInventory() {
    auto vLayout = menuBase.getGui()
        .get<tgui::GrowVerticalLayout>("vLayout");
    vLayout->removeAllWidgets();

    const auto &inventory(runInfo.itemManager.getItemMap());
    for (const auto &item : inventory) {
      auto hLayout = tgui::GrowHorizontalLayout::create();

      string btnName = item.first.getName();
      if (item.second > 1) {
        btnName += " (" + std::to_string(item.second) + ")";
      }

      auto button = tgui::Button::create(btnName);
      button->setSize("150", "30");
      button->getRenderer()->setBorderColor(tgui::Color::White);
      button->getRenderer()->setBackgroundColor(tgui::Color::Black);
      button->getRenderer()->setTextColor(tgui::Color::White);
      button->onPress([&, item] {
        runInfo.itemManager.useItem(mainScene, item.first);
        reloadInventory();
      });
      auto description = tgui::RichTextLabel::create();
      description->setText(mainScene.getDescription(item.first.getIdentity()));
      description->getRenderer()->setTextColor(tgui::Color::White);
      description->getRenderer()->setBorderColor(tgui::Color::White);
      description->getRenderer()->setTextStyle(tgui::TextStyle::Italic);
      description->setVerticalAlignment(tgui::VerticalAlignment::Center);
      description->setHorizontalAlignment(tgui::HorizontalAlignment::Center);

      auto line = tgui::SeparatorLine::create(button->getSizeLayout());

      hLayout->setSize(button->getSizeLayout().x + description->getSizeLayout().x, button->getSizeLayout().y);
      hLayout->add(button);
      hLayout->add(line);
      hLayout->add(description);
      vLayout->add(hLayout);
    }
    vLayout->setSize("100%", "100%");

  }

 public:

  void loadGraphics() override {
    menuBase.load();
    tgui::Gui tempGui;
    try {
      tempGui.loadWidgetsFromFile(path);
    }
    catch (const tgui::Exception &e) {
      std::cerr << "Failed to load Base for InventoryMenu: " << e.what() << std::endl;
      throw e;
    }
    auto inventoryPanel = tempGui.get<tgui::ScrollablePanel>("InvPanel");

    tempGui.remove(inventoryPanel);
    menuBase.getMainFreeSpace()->add(inventoryPanel);
    reloadInventory();

    if (auto backButton = menuBase.getGui().get<tgui::Button>("back")) {
      backButton->onPress([&] {
        parent.hideInventory();
      });
    }
  }

  void handleEvent(const std::optional<sf::Event> &_event) override {};

  InventoryMenu(GameContext &context, RunInfo &runInfo, ItemAffected &mainScene, Layout &base, InRunManager &parent)
      : Scene(context), runInfo(runInfo), mainScene(mainScene), menuBase(base), parent(parent) {};

  void clear() override {
    context.gui.removeAllWidgets();
  }
};


