//
// Created by user on 20.04.2025.
//
#pragma once

#include <memory>
#include <utility>
#include "TGUI/Widgets/TextArea.hpp"

class HitsTime : SmartSubscriber {
  std::string timerName;
  std::shared_ptr<tgui::RichTextLabel> hitArea;
  std::shared_ptr<tgui::RichTextLabel> timeArea;

  void sub() {
    subscribeSmartly<TimerMsg>(
        [this](const TimerMsg &msg) {
          if (msg.timerType != timerName) { return; };
          timeArea->setText("Time:" + std::to_string(msg.timeLeft));
        });

    subscribeSmartly<textProcessing::AttackLogicStateChange>(
        [this](const textProcessing::AttackLogicStateChange &msg) {
          hitArea->setText("Hits:" + std::to_string(msg.hits));
        });
  }

 public:
  HitsTime(std::string timerName, std::shared_ptr<tgui::RichTextLabel> hitArea,
           std::shared_ptr<tgui::RichTextLabel> timeArea, QueuedMessageBus &bus) : SmartSubscriber(bus),
                                                                                   timerName(std::move(timerName)),
                                                                                   hitArea(std::move(hitArea)),
                                                                                   timeArea(std::move(timeArea)) {
    sub();

  }
};