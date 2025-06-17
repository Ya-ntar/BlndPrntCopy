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

  virtual ~textProcessDisplay() = default;
};

class ColorLineDisplay : public textProcessDisplay, public SmartSubscriber {
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

class RunningLineDisplay : public textProcessDisplay, public SmartSubscriber {
  RunInfo &runInfo;
  std::shared_ptr<tgui::RichTextLabel> mainString;
 public:

  RunningLineDisplay(std::shared_ptr<tgui::RichTextLabel> active_char,
                     std::shared_ptr<tgui::RichTextLabel> main_string, RunInfo &runInfo,
                     QueuedMessageBus &bus) :
      SmartSubscriber(bus),
      runInfo(runInfo),
      mainString(std::move(main_string)) {

    subscribeSmartly<AttackLogicStateChange>(
        [this](const AttackLogicStateChange &msg) {
          this->updateView(msg.state);
        }
    );

  };

  void update() override {

    mainString->setTextSize(24);
    mainString->setText("<color=white>" + runInfo.getActiveChar()
    + "</color><color=#808080>" + runInfo.getMainString() + "</color>");


  }

  void errorReaction() override {
    mainString->setText("<color=red>" + runInfo.getActiveChar()
                            + "</color><color=white>" + runInfo.getMainString() + "</color>");
  }

  void set(tgui::RichTextLabel::Ptr newMainString) {
    this->mainString = std::move(newMainString);
  }
};

}  // namespace textProcessing


