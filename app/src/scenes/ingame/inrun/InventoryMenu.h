#pragma once

#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "../../../GameContext.h"
#include "InRunManager.h"

class InventoryMenu : public Scene {
  RunInfo &runInfo;
  ItemAffected &mainScene;
  InRunManager &parent;
  std::shared_ptr<tgui::Panel> mainPanel;
  std::shared_ptr<tgui::ScrollablePanel> inventoryPanel;
  std::shared_ptr<tgui::Button> backButton;

  void reloadInventory() {
    auto vLayout = inventoryPanel->get<tgui::GrowVerticalLayout>("vLayout");
    if (!vLayout) {
      vLayout = tgui::GrowVerticalLayout::create();
      inventoryPanel->add(vLayout, "vLayout");
    }
    vLayout->removeAllWidgets();

    const auto &inventory(runInfo.getManager().getItemMap());
    for (const auto &[item, count] : inventory) {
      auto hLayout = tgui::GrowHorizontalLayout::create();

      string btnName = item.getName();
      if (count > 1) {
        btnName += " (" + std::to_string(count) + ")";
      }

      auto button = tgui::Button::create(btnName);
      button->setSize("150", "30");
      WidgetConfigurator::configureButton(button);
      button->onPress([this, item] {
        runInfo.getManager().useItem(mainScene, item);
        reloadInventory();
      });

      auto description = tgui::RichTextLabel::create();
      description->setText(mainScene.getDescription(item.getIdentity()));
      WidgetConfigurator::configureRichTextLabel
          (description, tgui::Color::Black, tgui::Color::White,
           tgui::Color::White, tgui::VerticalAlignment::Center,
           tgui::HorizontalAlignment::Center, 13, tgui::TextStyle::Regular);

      auto line = tgui::SeparatorLine::create(button->getSizeLayout());
      description->setSize("300", "30");
      hLayout->setSize((button->getSize().x + description->getSize().x) * 2, button->getSize().y);
      hLayout->add(button);

      hLayout->add(description);
      vLayout->add(hLayout);
    }
    vLayout->setSize("100%", "100%");
  }

 public:
  void loadGraphics() override {

    if (auto info = context.gui.get<tgui::TextArea>("menu_info")) {
      info->setText("Inventory");
    }

    mainPanel = tgui::Panel::create();
    WidgetConfigurator::configurePanel(mainPanel);

    auto middle = context.gui.get<tgui::Panel>("main_free_space");
    middle->getRenderer()->setBackgroundColor(tgui::Color::Green);
    middle->add(mainPanel);

    inventoryPanel = tgui::ScrollablePanel::create();
    WidgetConfigurator::configureScrollablePanel(inventoryPanel);
    mainPanel->add(inventoryPanel);

    backButton = context.gui.get<tgui::Button>("back");
    WidgetConfigurator::configureBackButton(backButton);
    backButton->onPress([this] {
      parent.hideInventory();
    });

    reloadInventory();
  }

  void handleEvent(const std::optional<sf::Event> &_event) override {};

  InventoryMenu(GameContext &context, RunInfo &runInfo, ItemAffected &mainScene, InRunManager &parent)
      : Scene(context), runInfo(runInfo), mainScene(mainScene), parent(parent) {};

  void clear() override {
    if (mainPanel) {
      context.gui.remove(mainPanel);
      mainPanel.reset();
      inventoryPanel.reset();
      backButton.reset();
    }
  }
};


