//
// Created by user on 16.04.2025.
//

#pragma once

#include <utility>

#include "FillableDisplayer.h"

#include "TGUI/Backend/SFML-Graphics.hpp"
#include "../../../../world_player_and_container/Player.h"

namespace player {
class PlayerParametersFront : SmartSubscriber {
  FillableDisplayer hp;
  FillableDisplayer errors;

  void init() {
    update_all();
    subscribeSmartly<PlayerChangedMsg>(
        [this](const PlayerChangedMsg &msg) {
          switch (msg.val) {
            case State::HP_LESSENED:hp.damage();
              break;
            case State::HP_GREW:hp.heal();
              break;
            case State::ERRORS_LESSENED:errors.damage();
              break;
            case State::ERRORS_GREW:errors.heal();
              break;
            case State::STABILISE:hp.neutral();
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

  PlayerParametersFront(player::Player &player,
                        const std::shared_ptr<tgui::TextArea> &curr_hp,
                        const std::shared_ptr<tgui::TextArea> &max_hp,
                        const std::shared_ptr<tgui::TextArea> &curr_err,
                        const std::shared_ptr<tgui::TextArea> &max_err) :
      SmartSubscriber(player.getBus()),
      hp(curr_hp, max_hp, player.getHp()),
      errors(curr_err, max_err, player.getErrors()) {
    init();
  }

};
}  // namespace player