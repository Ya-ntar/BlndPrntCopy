#pragma once
#include <optional>
#include <memory>
#include <string>
#include <iostream>
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include "../../../../GameContext.h"
#include "RunInfoFront.h"

class InRunPanel {
  RunInfo &runInfo;
  std::string path;
  std::optional<RunInfoFront> playerParametersFront;
  std::shared_ptr<tgui::Panel> mainPanel;
  std::shared_ptr<tgui::Panel> mainFreeSpace;
  std::shared_ptr<tgui::Panel> topFreeSpace;

  void reloadPlayerParametersFront(std::shared_ptr<tgui::Group> const &where) {
    auto curr_hp = where->get<tgui::TextArea>("current_hp");
    auto max_hp = where->get<tgui::TextArea>("max_hp");
    auto curr_err = where->get<tgui::TextArea>("curr_err");
    auto max_err = where->get<tgui::TextArea>("max_err");
    auto coins = where->get<tgui::TextArea>("coins_info_value");
    auto level = where->get<tgui::TextArea>("deep_info_value");
    auto armour = where->get<tgui::TextArea>("armour_info_value");

    playerParametersFront.emplace(runInfo, curr_hp, max_hp, curr_err, max_err, coins, armour, level);
  }

 public:
  explicit InRunPanel(RunInfo &runInfo, std::string path = std::string(Assets::kFormBase))
      : runInfo(runInfo), path(std::move(path)) {}

  void setRunMenu(std::shared_ptr<tgui::Group> const &where) {
    tgui::Gui tempGui;
    try {
      tempGui.loadWidgetsFromFile(path);
    }
    catch (const tgui::Exception &e) {
      std::cerr << "Failed to load Base for RunMenu: " << e.what() << std::endl;
      throw e;
    }
    mainPanel = tempGui.get<tgui::Panel>("main_panel");
    mainPanel->getRenderer()->setBorderColor(tgui::Color::White);
    mainPanel->getRenderer()->setBackgroundColor(tgui::Color::Black);

    auto newMainFreeSpace = tempGui.get<tgui::Panel>("main_free_space");
    mainFreeSpace = newMainFreeSpace;
    auto newTopFreeSpace = tempGui.get<tgui::Panel>("top_free_space");
    topFreeSpace = newTopFreeSpace;
    tempGui.removeAllWidgets();
    mainPanel->setSize("100%", "100%"); //toDo: написать в поддержку tgui, потому что сетсайз забагован?
    where->add(mainPanel);

    reloadPlayerParametersFront(where);
  }

  [[nodiscard]] std::shared_ptr<tgui::Panel> getMainFreeSpace() const { return mainFreeSpace; }

  [[nodiscard]] std::shared_ptr<tgui::Panel> getTopFreeSpace() const { return topFreeSpace; }

  std::shared_ptr<tgui::Panel> get() const { return mainPanel; }

  [[nodiscard]] bool hasBeenLoaded() const { return playerParametersFront.has_value(); }

  void clear() {
    playerParametersFront.reset();
    mainFreeSpace = nullptr;
    topFreeSpace = nullptr;
  }

  void updatePlayerPanel() {
    if (playerParametersFront) playerParametersFront->update_all();
  }
}; 