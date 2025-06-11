//
// Created by user on 13.04.2025.
//

#pragma once

#include <memory>
#include <utility>

#include "TGUI/Widgets/TextArea.hpp"
#include "../../../../../../text_processing/backEnd/tools/TextHolder.h"
#include "../../../../../../world_player_and_container/Player.h"
#include "TGUI/Widgets/RichTextLabel.hpp"

namespace textProcessing {
class textProcessDisplay {

 public:
  virtual void update() = 0;

  virtual void errorReaction() = 0;

  virtual void updateView(const CharCheckState &state) {
    if (state == CharCheckState::INCORRECT_INPUT) {
      errorReaction();
    }
    if (state == CharCheckState::CORRECT_INPUT) {
      update();
    }
  };

};

class ColorLineDisplay : textProcessDisplay, SmartSubscriber {
  std::shared_ptr<tgui::RichTextLabel> text;

 public:
  tgui::Color userColor = "#AFEEEE";
  tgui::Color mobColor = "#B22222";
  tgui::Color sharedColor = "#EE82EE";

  void update() override {};

  ColorLineDisplay(const std::shared_ptr<tgui::RichTextLabel> &text,
                   QueuedMessageBus &bus) :
      SmartSubscriber(bus) {

    subscribeSmartly<AttackLogicStateChange>(
        [this](const AttackLogicStateChange &msg) {
          this->updateView(msg.state);
        }
    );

  };

};

class RunningLineDisplay : textProcessDisplay, SmartSubscriber {
  RunInfo &runInfo;
  std::shared_ptr<tgui::TextArea> activeChar;
  std::shared_ptr<tgui::TextArea> mainString;
 public:

  RunningLineDisplay(std::shared_ptr<tgui::TextArea> active_char,
                     std::shared_ptr<tgui::TextArea> main_string, RunInfo &runInfo,
                     QueuedMessageBus &bus) :
      SmartSubscriber(bus),
      activeChar(std::move(active_char)),
      mainString(std::move(main_string)), runInfo(runInfo) {

    subscribeSmartly<AttackLogicStateChange>(
        [this](const AttackLogicStateChange &msg) {
          this->updateView(msg.state);
        }
    );

  };

  void update() override {
    activeChar->getRenderer()->setBackgroundColor("#141359");
    activeChar->getRenderer()->setTextColor(tgui::Color::White);
    mainString->getRenderer()->setTextColor("#808080");

    activeChar->setText(runInfo.getActiveChar());
    mainString->setText(runInfo.getMainString());
    if (runInfo.getActiveChar() == U'\n') { //toDo: плохо пахнет, подумать как правильно
      mainString->setText(runInfo.getActiveChar() + runInfo.getMainString());
    }
    mainString->setMaximumCharacters(runInfo.getHowManyVisibleLetters());
    mainString->setCaretPosition(0);
    mainString->getHorizontalScrollbar()->setValue(0);
    mainString->getVerticalScrollbar()->setValue(0);
  }

  void errorReaction() override {
    activeChar->getRenderer()->setBackgroundColor(tgui::Color::Red);
  }

  void set(tgui::TextArea::Ptr newActiveChar, tgui::TextArea::Ptr newMainString) {
    this->activeChar = std::move(newActiveChar);
    this->mainString = std::move(newMainString);
  }
};

}  // namespace textProcessing


