#pragma once

#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "../../../GameContext.h"
#include "InRunManager.h"


class BookMenu : public Scene {
  RunInfo &runInfo;
  std::string path = Assets::FORM_INVENTORY;
  InRunManager &parent;
  std::shared_ptr<tgui::Panel> mainPanel;
  std::shared_ptr<tgui::ScrollablePanel> bookPanel;
  std::shared_ptr<tgui::Button> backButton;

  void reloadBooks() {
    auto vLayout = bookPanel->get<tgui::GrowVerticalLayout>("vLayout");
    if (!vLayout) {
      vLayout = tgui::GrowVerticalLayout::create();
      bookPanel->add(vLayout, "vLayout");
    }
    vLayout->removeAllWidgets();

    for (const auto &pair : runInfo.getBooks()) {
      const auto &book = pair.second;
      auto hLayout = tgui::GrowHorizontalLayout::create();

      std::string btnName = book.getName();
      auto button = tgui::Button::create(btnName);
      button->setSize("150", "30");
      button->getRenderer()->setBorderColor(tgui::Color::White);
      button->getRenderer()->setBackgroundColor(tgui::Color::Black);
      button->getRenderer()->setTextColor(tgui::Color::White);
      button->onPress([this, id = book.getId()] {
        runInfo.setCurrentBook(id);
      });

      auto description = tgui::RichTextLabel::create();
      description->setText(book.getName() + ", " + std::to_string(book.getDurability()));
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
    //context.gui.removeAllWidgets();
    //BookLayoutUtils::createBase(context.gui);
    //BookLayoutUtils::addRunInfo(context.gui, runInfo);

    // Create main panel
    mainPanel = tgui::Panel::create();
    mainPanel->setSize("100%", "100%");
    mainPanel->getRenderer()->setBackgroundColor(tgui::Color::Black);

    auto middle = context.gui.get<tgui::Panel>("main_free_space");
    middle->add(mainPanel);

    // Create book panel
    bookPanel = tgui::ScrollablePanel::create();
    bookPanel->setSize("80%", "80%");
    bookPanel->setPosition("10%", "10%");
    bookPanel->getRenderer()->setBackgroundColor(tgui::Color(30, 30, 30));
    mainPanel->add(bookPanel);

    // Create back button
    backButton = context.gui.get<tgui::Button>("back");
    backButton->onPress.disconnectAll();
    backButton->setText("back");
    backButton->onPress([this] {
      parent.hideBookMenu();
    });

    reloadBooks();
  }

  void handleEvent(const std::optional<sf::Event> &_event) override {};

  BookMenu(GameContext &context, RunInfo &runInfo, InRunManager &parent)
      : Scene(context), runInfo(runInfo), parent(parent) {};

  void clear() override {
    if (mainPanel) {
      context.gui.remove(mainPanel);
      mainPanel.reset();
      bookPanel.reset();
      backButton.reset();
    }
  }
}; 