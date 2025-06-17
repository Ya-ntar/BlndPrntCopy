#pragma once
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "../../../GameContext.h"
#include "InRunManager.h"

struct BookLayoutUtils {

  static void createBase(tgui::Gui &gui) {
    auto up = tgui::Group::create();
    up->setWidgetName("up");
    auto middle = tgui::Group::create();
    middle->setWidgetName("middle");
    auto down = tgui::Group::create();
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
  }
  static void addRunInfo(tgui::Gui &gui, RunInfo &runInfo, std::string path = std::string(Assets::FORM_BASE)) {
    tgui::Gui tempGui;
    try {
      tempGui.loadWidgetsFromFile(path);
    }
    catch (const tgui::Exception &e) {
      std::cerr << "Failed to load Base for RunMenu: " << e.what() << std::endl;
      throw e;
    }
    auto mainPanel = tempGui.get<tgui::Panel>("main_panel");
    mainPanel->getRenderer()->setBorderColor(tgui::Color::White);

    auto newMainFreeSpace = tempGui.get<tgui::Panel>("main_free_space");
    auto mainFreeSpace = newMainFreeSpace;
    auto newTopFreeSpace = tempGui.get<tgui::Panel>("top_free_space");
    auto topFreeSpace = newTopFreeSpace;
    tempGui.removeAllWidgets();
    mainPanel->setSize("100%", "100%");
    auto down = gui.get<tgui::Group>("down");
    down->removeAllWidgets();
    down->add(mainPanel);
  }
  static void updateRunInfo(tgui::Gui &gui, RunInfo &runInfo) {
    auto curr_hp = gui.get<tgui::TextArea>("current_hp");
    auto max_hp = gui.get<tgui::TextArea>("max_hp");
    auto curr_err = gui.get<tgui::TextArea>("curr_err");
    auto max_err = gui.get<tgui::TextArea>("max_err");
    auto coins = gui.get<tgui::TextArea>("coins_info_value");
    auto level = gui.get<tgui::TextArea>("deep_info_value");
    auto armour = gui.get<tgui::TextArea>("armour_info_value");

    curr_hp->setText(std::to_wstring(runInfo.player.getHp().get_current_value()));
    max_hp->setText(std::to_wstring(runInfo.player.getHp().get_max_value()));

    curr_err->setText(std::to_wstring(runInfo.player.getErrors().get_current_value()));
    max_err->setText(std::to_wstring(runInfo.player.getErrors().get_max_value()));

    coins->setText(std::to_wstring(runInfo.getCurrentCoins()));
    level->setText(std::to_wstring(runInfo.getCurrentLvl()));
    armour->setText(runInfo.getCurrentBook().getName());
  }
};