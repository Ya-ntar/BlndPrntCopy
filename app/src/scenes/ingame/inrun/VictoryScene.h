#pragma once
#include "../../../GameContext.h"
#include "base/Layout.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <memory>
#include <string>

class InRunManager;

class VictoryScene : public Scene {
  RunInfo &run_info_;
  Layout &layout_;
  InRunManager *parent;
  std::shared_ptr<tgui::Panel> main_panel_;
 public:
  VictoryScene(RunInfo &runInfo, Layout &layout, InRunManager *parent)
      : Scene(layout.getContext()), layout_(layout), run_info_(runInfo), parent(parent) {}

  void loadGraphics() override;
  void clear() override;
  void prepare() override;
  void handleEvent(const std::optional<sf::Event> &_event) override {};
}; 