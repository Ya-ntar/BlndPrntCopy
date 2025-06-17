//
// Created by user on 16.04.2025.
//

#pragma once

#include <utility>

#include "TGUI/Backend/SFML-Graphics.hpp"
#include "../../../../world_player_and_container/Player.h"

template<typename T>
class ValueDisplayer {
  std::shared_ptr<tgui::TextArea> place;
  T value;
 public:
  ValueDisplayer(std::shared_ptr<tgui::TextArea> current_value, const T &val)
      : place(std::move(current_value)), value(val) {}

  void update() {
    place->setText(value);
  }
  void update(T new_value) {
    value = new_value;
    update();
  }
};

template<typename T> requires std::integral<T> || std::floating_point<T>
class ValueDisplayer<T> {
  std::shared_ptr<tgui::TextArea> place;
  T value;

 public:
  ValueDisplayer(std::shared_ptr<tgui::TextArea> current_value, const T &val)
      : place(std::move(current_value)), value(val) {}

  void update() {
    place->setText(std::to_string(value));
  }
  void update(T new_value) {
    value = new_value;
    update();
  }
};

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
  const RunInfo &run_info_;
  BoundedValueDisplayer hp;
  BoundedValueDisplayer errors;
  ValueDisplayer<int> coins;
  ValueDisplayer<tgui::String> armor;
  ValueDisplayer<int> level;

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

 public:
  void update_all() {
    hp.update();
    errors.update();
    coins.update(run_info_.getCurrentCoins());
    armor.update(run_info_.getCurrentBook().getName());
    level.update(run_info_.getCurrentLvl());
  }
  RunInfoFront(RunInfo &runInfo,
               const std::shared_ptr<tgui::TextArea> &curr_hp,
               const std::shared_ptr<tgui::TextArea> &max_hp,
               const std::shared_ptr<tgui::TextArea> &curr_err,
               const std::shared_ptr<tgui::TextArea> &max_err,
               const std::shared_ptr<tgui::TextArea> &coins,
               const std::shared_ptr<tgui::TextArea> &armor,
               const std::shared_ptr<tgui::TextArea> &level
  ) :
      SmartSubscriber(runInfo.player.getBus()),
      run_info_(runInfo),
      hp(curr_hp, max_hp, runInfo.player.getHp()),
      errors(curr_err, max_err, runInfo.player.getErrors()),
      coins(coins, runInfo.getCurrentCoins()),
      armor(armor, runInfo.getCurrentBook().getName()),
      level(level, runInfo.getCurrentLvl()) {
    init();
  }

};
