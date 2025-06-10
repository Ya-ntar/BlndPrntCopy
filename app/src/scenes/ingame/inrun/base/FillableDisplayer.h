//
// Created by user on 15.04.2025.
//

#pragma once

#include <memory>
#include <utility>

#include "TGUI/Widgets/TextArea.hpp"
#include "../../../../world_player_and_container/BoundedValue.h"

namespace player {
class FillableDisplayer {
  std::shared_ptr<tgui::TextArea> current_value;
  std::shared_ptr<tgui::TextArea> max_value;
  const BoundedValue &fillable;

 public:
  FillableDisplayer(std::shared_ptr<tgui::TextArea> current_value, std::shared_ptr<tgui::TextArea> max_value,
                    const BoundedValue &fillable)
      : current_value(std::move(current_value)), max_value(std::move(max_value)), fillable(fillable) {

  }

  void update() {
    current_value->setText(std::to_wstring(fillable.get_current_value()));
    max_value->setText(std::to_wstring(fillable.get_max_value()));
  }

  void damage() {
    current_value->getRenderer()->setTextColor(tgui::Color::Red);
    update();
  }

  void heal() {
    current_value->getRenderer()->setTextColor(tgui::Color::Green);
  }

  void neutral() {
    current_value->getRenderer()->setTextColor(tgui::Color::White);
    max_value->getRenderer()->setTextColor(tgui::Color::White);
    update();
  }

};
}  // namespace player