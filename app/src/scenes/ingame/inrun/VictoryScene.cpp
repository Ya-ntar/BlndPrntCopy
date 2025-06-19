#include "VictoryScene.h"
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include "../../../utils/WidgetConfigurator.h"
#include "InRunManager.h"

void VictoryScene::loadGraphics() {
  main_panel_ = tgui::Panel::create();
  WidgetConfigurator::configurePanel(main_panel_);
  auto main = context.gui.get<tgui::Panel>("main_free_space");
  main->add(main_panel_);

  auto label = tgui::Label::create(" Congratulations! You've got: " + run_info_.getLog());

  main_panel_->add(label);
  label->setTextSize(28);
  label->setHorizontalAlignment(tgui::HorizontalAlignment::Center);

  auto back_button = context.gui.get<tgui::Button>("back");
  if (back_button) {
    back_button->setText("Continue");
    back_button->onPress([this] {
      parent->changeScene(State::InRun::CROSSROAD);
    });
  }
}

void VictoryScene::clear() {
  if (main_panel_) {
    auto main = context.gui.get<tgui::Panel>("main_free_space");
    main->remove(main_panel_);
    main_panel_.reset();
  }
}
void VictoryScene::prepare() {
  layout_.load();
}
