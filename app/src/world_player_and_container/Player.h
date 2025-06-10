//
// Created by user on 13.04.2025.
//

#pragma once

#include "../text_processing/backEnd/tools/TextHolder.h"

#include "BoundedValue.h"
#include "../scenes/ingame/inrun/base/FillableDisplayer.h"
#include "../GameContext.h"
#include "ItemManager.h"
#include "../utils/Utils.h"

namespace player {
struct PlayerContext {
  float time;
  size_t how_many_visible_letters_;
  BoundedValue hp;
  BoundedValue errors;
  int pwr;

  PlayerContext(float time, size_t howManyVisibleLetters, BoundedValue hp, BoundedValue errors, int pwr = 1) :
      time(time), how_many_visible_letters_(howManyVisibleLetters), hp(hp), errors(errors), pwr(pwr) {};
};

class Player : public SmartSubscriber {
  QueuedMessageBus &bus;
  TextHolder &current_text_;
  PlayerContext context;

 private:
  void incorrectInput() {
    if (context.errors.get_current_value() > 0) {
      damageErr(1);
    } else {
      damageHP(1);
    }
  }

  void subscribeToEverything() {
    subscribeSmartly<textProcessing::TextProcessStateMsg>(
        [this](const textProcessing::TextProcessStateMsg &msg) {
          if (msg.val == textProcessing::CharCheckState::INCORRECT_INPUT) {
            incorrectInput();
          }
        }
    );
    subscribeSmartly<mob::MobAttacking>(
        [this](const mob::MobAttacking &msg) {
          damageHP(msg.damage);
        }
    );

  }

 public:
  Player(QueuedMessageBus &bus, TextHolder &currentText, PlayerContext startingPlayerContext) :
      SmartSubscriber(bus), bus(bus), current_text_(currentText),
      context(startingPlayerContext) {
    subscribeToEverything();

  }

  TextHolder &getTextHolder() {
    return current_text_;
  }

  [[nodiscard]] size_t getHowManyVisibleLetters() const {
    return context.how_many_visible_letters_;
  }

  [[nodiscard]] tgui::String getActiveChar() const {
    if (current_text_.isEmpty()) {
      return "";
    }
    return current_text_.getFirstChar();
  }

  [[nodiscard]] tgui::String getMainString() const {

    assert(context.how_many_visible_letters_ <= current_text_.maxSize() &&
        "Visible length should be smaller, or it becomes = holder.maxsize()");
    auto txt(current_text_.currentText(context.how_many_visible_letters_));
    if (txt.size() <= 1) {
      return "";
    }
    return txt.substr(1);
  }

  [[nodiscard]] const BoundedValue &getHp() const {
    return context.hp;
  };
  void healHP(int hm) {
    context.hp.heal(hm);
    bus.add(PlayerChangedMsg{.val = State::HP_GREW});
  }
  void healErr(int hm) {
    context.errors.heal(hm);
    bus.add(PlayerChangedMsg{.val = State::ERRORS_GREW});
  }
  void damageHP(int hm) {
    context.hp.damage(hm);
    if (context.hp.get_current_value() == 0) {
      bus.add(PlayerDeath{});
    }
    bus.add(PlayerChangedMsg{.val = State::HP_LESSENED});
  }
  void damageErr(int hm) {
    context.errors.damage(hm);
    bus.add(PlayerChangedMsg{.val = State::ERRORS_LESSENED});
  }
  [[nodiscard]] const BoundedValue &getErrors() const {
    return context.errors;
  }
  [[nodiscard]] const float &getTime() const {
    return context.time;
  }

  [[nodiscard]] int32_t getPwr() const {
    return context.pwr;
  }
};

}  // namespace player

