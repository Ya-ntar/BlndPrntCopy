//
// Created by user on 16.04.2025.
//

#pragma once

#include <utility>

#include "TGUI/Backend/SFML-Graphics.hpp"
#include "../../../../world_player_and_container/Player.h"

class BoundedValueDisplayer {
  std::shared_ptr<tgui::TextArea> current_value;
  std::shared_ptr<tgui::TextArea> max_value;
  const BoundedValue &bounded_value_;

 public:
  BoundedValueDisplayer(std::shared_ptr<tgui::TextArea> current_value, std::shared_ptr<tgui::TextArea> max_value,
                        const BoundedValue &fillable)
      : current_value(std::move(current_value)), max_value(std::move(max_value)), bounded_value_(fillable) {

  }

  void update() {
    current_value->setText(std::to_wstring(bounded_value_.get_current_value()));
    max_value->setText(std::to_wstring(bounded_value_.get_max_value()));
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

class RunInfoFront : public SmartSubscriber {
  BoundedValueDisplayer hp;
  BoundedValueDisplayer errors;

  void init() {
    update_all();
    subscribeSmartly<player::PlayerChangedMsg>(
        [this](const player::PlayerChangedMsg &msg) {
          switch (msg.val) {
            case player::State::HP_LESSENED:hp.damage();
              break;
            case player::State::HP_GREW:hp.heal();
              break;
            case player::State::ERRORS_LESSENED:errors.damage();
              break;
            case player::State::ERRORS_GREW:errors.heal();
              break;
            case player::State::STABILISE:hp.neutral();
              errors.neutral();
          }
          update_all();
        }
    );

  }

  void update_all() {
    hp.update();
    errors.update();
  }

 public:

  RunInfoFront(RunInfo &runInfo,
               const std::shared_ptr<tgui::TextArea> &curr_hp,
               const std::shared_ptr<tgui::TextArea> &max_hp,
               const std::shared_ptr<tgui::TextArea> &curr_err,
               const std::shared_ptr<tgui::TextArea> &max_err


               ) :
      SmartSubscriber(runInfo.player.getBus()),
      hp(curr_hp, max_hp, runInfo.player.getHp()),
      errors(curr_err, max_err, runInfo.player.getErrors()) {
    init();
  }

};
