//
// Created by user on 20.04.2025.
//

#pragma once

#include <cdonts.h>

#include <utility>
#include "TGUI/Timer.hpp"
#include "QueuedMessageBus.h"
#include "Messages.h"

class SmartTimer {
  std::string name;
  float stopTime;
  float tickInterval;
  float totalTime;

  QueuedMessageBus &bus;
  std::shared_ptr<tgui::Timer> timer;
 public:

  explicit SmartTimer(QueuedMessageBus &bus, std::string name,
                      float stopTime = 5.0F,
                      float tickInterval = 0.5F, float totalTime = 0.0F) : name(std::move(name)), stopTime(stopTime),
                                                                           tickInterval(tickInterval),
                                                                           totalTime(totalTime),
                                                                           bus(bus) {

  }

  ~SmartTimer() {
    end();
  }

  void end() {
    if (timer != nullptr) {
      timer->setEnabled(false);
      timer.reset();
    }
    totalTime = 0;
  }
  void startTimer(float newStopTime) {
    stopTime = newStopTime;
    startTimer();

  }
  void startTimer() {
    timer = tgui::Timer::create(
        [this]() {
          totalTime += tickInterval;
          bus.add(TimerMsg{.timerType = name, .timeLeft = stopTime -
              totalTime, .is_finished= false});
          if (totalTime >= stopTime) {
            bus.add(TimerMsg{.timerType = name, .timeLeft = 0, .is_finished= true});
            timer->setEnabled(false);
          }
        },
        std::chrono::milliseconds(static_cast<int>(tickInterval * 1000)),  // 500 мс
        true
    );
  }

};