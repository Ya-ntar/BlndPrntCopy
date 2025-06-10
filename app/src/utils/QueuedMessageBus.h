//
// Created by user on 15.04.2025.
//
#pragma once

#include <queue>
#include <functional>
#include <mutex>
#include "PubBus/MessageBus.hpp"

class QueuedMessageBus {

 private:
  pub::MessageBus m_inner_bus;
  std::queue<std::function<void()>> m_message_queue;

  QueuedMessageBus() = default;

 public:
  QueuedMessageBus(const QueuedMessageBus &) = delete;

  QueuedMessageBus &operator=(const QueuedMessageBus &) = delete;

  static QueuedMessageBus &getInstance() {
    static QueuedMessageBus instance;
    return instance;
  }

  template<typename M>
  void add(const M &message) {
    m_message_queue.push([this, message]() {
      m_inner_bus.publish(message);
    });
  }

  template<typename M>
  void processImmediately(const M &message) {
    m_inner_bus.publish(message);
  }

  void processLast() {
    if (m_message_queue.empty()) {
      return;
    }
    m_message_queue.front()();
    m_message_queue.pop();
  }

  void process_all() {
    std::queue<std::function<void()>> local_queue;

    {
      std::swap(local_queue, m_message_queue);
    }

    while (!local_queue.empty()) {
      local_queue.front()();
      local_queue.pop();
    }
  }

 private:

  template<typename M>
  auto subscribe(std::function<void(const M &)> subscriber) {
    return m_inner_bus.subscribe<M>(subscriber);
  }

  void unsubscribe(pub::SubscriberHandle &handle) {
    m_inner_bus.unsubscribe(handle);
  }

  friend class SmartSubscriber;
};