#pragma once
#include <thread>
#include <format>
#include <chrono>
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <optional>
#include <memory>
#include <string>
#include <iostream>
#include "../../../../GameContext.h"
#include "RunInfoFront.h"
#include "../../../../scenes/Scene.h"
#include "SelectableTemplate.h"
#include "InRunPanel.h"
#include "MobView.h"

class Layout {
  GameContext &context;
  tgui::Gui &gui = context.gui;
  std::shared_ptr<tgui::Group> up;
  std::shared_ptr<tgui::Group> middle;
  std::shared_ptr<tgui::Group> down;
  InRunPanel player_menu_panel_;
  std::shared_ptr<Settable> middleView;
 public:
  Layout(GameContext &context, RunInfo &runInfo, std::string path = std::string(Assets::kFormBase))
      : context(context), player_menu_panel_(runInfo, std::move(path)), runInfo(runInfo) {
    init();
  }

  tgui::Gui &getGui() { return context.gui; }

  [[nodiscard]] std::shared_ptr<tgui::Panel> getMainFreeSpace() const { return player_menu_panel_.getMainFreeSpace(); }

  [[nodiscard]] std::shared_ptr<tgui::Panel> getTopFreeSpace() const { return player_menu_panel_.getTopFreeSpace(); }

  GameContext &getContext() { return context; }

  void setMiddle(std::shared_ptr<Settable> view) {
    middleView = std::move(view);
    middleView->setParent(middle);
  }

  player::Player &getPlayer() { return runInfo.player; }

  [[nodiscard]] bool hasBeenLoaded() const { return player_menu_panel_.hasBeenLoaded(); }

  std::shared_ptr<tgui::Panel> get() { return player_menu_panel_.get(); }

  void load() {
    gui.removeAllWidgets();
    init();
    if (middleView) {
      middleView->setParent(middle);
      middleView->load();
    }
  }

  void init() {
    up = tgui::Group::create();
    middle = tgui::Group::create();
    down = tgui::Group::create();

    up->setWidgetName("up");
    middle->setWidgetName("middle");
    down->setWidgetName("down");

    gui.add(up);
    gui.add(middle);
    gui.add(down);
    up->setSize("100%", "10%");
    up->setPosition({"0%", "0%"});
    middle->setSize("100%", "60%");
    middle->setPosition({"0%", "10%"});
    down->setSize("100%", "30%");
    down->setPosition({"0%", "70%"});
    player_menu_panel_.setRunMenu(down);
  }

  void clear() {
    gui.removeAllWidgets();
    up.reset();
    down.reset();
    middle.reset();
    player_menu_panel_.clear();
    middleView.reset();
  }

  void updatePlayerPanel() { player_menu_panel_.updatePlayerPanel(); }

  RunInfo &runInfo;
};


