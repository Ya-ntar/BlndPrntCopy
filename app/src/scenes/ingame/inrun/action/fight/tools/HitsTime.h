//
// Created by user on 20.04.2025.
//
#pragma once

#include <memory>
#include <utility>
#include "TGUI/Widgets/TextArea.hpp"

class HitsTime : SmartSubscriber {
  std::string timerName;
  std::shared_ptr<tgui::TextArea> hitArea;
  std::shared_ptr<tgui::TextArea> timeArea;

  void sub() {
    subscribeSmartly<TimerMsg>(
        [this](const TimerMsg &msg) {
          if (msg.timerType != timerName) { return; };
          timeArea->setText(std::to_string(msg.timeLeft));
        });

    subscribeSmartly<textProcessing::AttackLogicStateChange>(
        [this](const textProcessing::AttackLogicStateChange &msg) {
          hitArea->setText(std::to_string(msg.hits));
        });
  }

 public:
  HitsTime(std::string timerName, std::shared_ptr<tgui::TextArea> hitArea,
           std::shared_ptr<tgui::TextArea> timeArea, QueuedMessageBus &bus) : timerName(std::move(timerName)),
                                                                              SmartSubscriber(bus),
                                                                              hitArea(std::move(hitArea)),
                                                                              timeArea(std::move(timeArea)) {
    sub();

  }
};