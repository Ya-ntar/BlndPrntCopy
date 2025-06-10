//
// Created by user on 16.04.2025.
//

#pragma once

#include <vector>
#include "PubBus/SubscriberHandle.hpp"
#include "QueuedMessageBus.h"

class SmartSubscriber {
  std::vector<pub::SubscriberHandle> subscribes;
  QueuedMessageBus &bus;;

 private:

  void freeMe() {
    for (auto s : subscribes) {
      try {
        bus.unsubscribe(s);
      } catch (...) {
        // toDo: обмазать exceptions
      }
    }
    subscribes.clear();
  }

 public:
  template<typename M>
  void subscribeSmartly(std::function<void(const M &)> subscriber) {
    subscribes.push_back(bus.subscribe(subscriber));
  }

  QueuedMessageBus &getBus() {
    return bus;
  }

  explicit SmartSubscriber(QueuedMessageBus &bus) : bus(bus) {}

  SmartSubscriber(const SmartSubscriber &) = delete;

  SmartSubscriber &operator=(const SmartSubscriber &) = delete;

  virtual ~SmartSubscriber() {
    freeMe();
  }

};