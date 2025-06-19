#pragma once

#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "../../../GameContext.h"
#include "InRunManager.h"

class BookMenu : public Scene {
  RunInfo &runInfo;
  std::string path = Assets::kFormInventory;
  InRunManager &parent;
  std::shared_ptr<tgui::Panel> main_panel_;
  std::shared_ptr<tgui::ScrollablePanel> panel_;
  std::shared_ptr<tgui::Button> back_button_;

  void reloadBooks() {
    auto vLayout = panel_->get<tgui::GrowVerticalLayout>("vLayout");
    if (!vLayout) {
      vLayout = tgui::GrowVerticalLayout::create();
      panel_->add(vLayout, "vLayout");
    }
    vLayout->removeAllWidgets();

    for (const auto &pair : runInfo.getBooks()) {
      const auto &book = pair.second;
      auto hLayout = tgui::GrowHorizontalLayout::create();

      std::string btnName = book.getName();
      auto button = tgui::Button::create(btnName);
      button->setSize("150", "35");
      WidgetConfigurator::configureButton(button);
      button->onPress([this, id = book.getId()] {
        runInfo.setCurrentBook(id);
        parent.loadGraphics();
      });

      auto description = tgui::RichTextLabel::create();
      description->setText(book.getName() +
          " (Lvl: " + std::to_string(book.getLevel()) +
          ", DMG: " + std::to_string(book.getDamagePerHit()) +
          ", Time: +" + std::to_string(book.getExtraTime()) +
          ", Durability: " + std::to_string(book.getDurability()) + ")");
      WidgetConfigurator::configureRichTextLabel(description, tgui::Color::Black, tgui::Color::White,
                                                 tgui::Color::White, tgui::VerticalAlignment::Center,
                                                 tgui::HorizontalAlignment::Center, 0, tgui::TextStyle::Regular);

      auto line = tgui::SeparatorLine::create(button->getSizeLayout());
      description->setSize("300", "35");
      hLayout->setSize(button->getSize().x + description->getSize().x, button->getSizeLayout().y);
      hLayout->add(button);
      hLayout->add(line);
      hLayout->add(description);
      vLayout->add(hLayout);
    }
    vLayout->setSize("100%", "100%");
  }

 public:
  void loadGraphics() override {

    main_panel_ = tgui::Panel::create();
    WidgetConfigurator::configurePanel(main_panel_);

    auto middle = context.gui.get<tgui::Panel>("main_free_space");
    middle->add(main_panel_);

    panel_ = tgui::ScrollablePanel::create();
    WidgetConfigurator::configureScrollablePanel(panel_);
    main_panel_->add(panel_);

    back_button_ = context.gui.get<tgui::Button>("back");
    WidgetConfigurator::configureBackButton(back_button_);
    back_button_->onPress([this] {
      parent.hideBookMenu();
    });

    if (auto info = context.gui.get<tgui::TextArea>("menu_info")) {
      info->setText("Book Menu");
    }

    reloadBooks();
  }

  void handleEvent(const std::optional<sf::Event> &_event) override {};

  BookMenu(GameContext &context, RunInfo &runInfo, InRunManager &parent)
      : Scene(context), runInfo(runInfo), parent(parent) {};

  void clear() override {
    if (main_panel_) {
      context.gui.remove(main_panel_);
      main_panel_.reset();
      panel_.reset();
      back_button_.reset();
    }
  }
}; 