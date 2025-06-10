// Code from project [PubBus] (https://github.com/eXpl0it3r/PubBus/tree/faa3f4e4818be5fd86424986bd3bf9b0bd6a876e/include/PubBus)
// Author: [eXpl0it3r] license: Public Domain.

#pragma once

#include "MessageContainerBase.hpp"
#include <functional>
#include <map>

namespace pub {
template<typename M>
class MessageContainer final : public MessageContainerBase {
 public:
  MessageContainer() = default;

  MessageContainer(MessageContainer &&) = default;

  MessageContainer(const MessageContainer &) = default;

  MessageContainer &operator=(MessageContainer &&) = default;

  MessageContainer &operator=(const MessageContainer &) = default;

  ~MessageContainer() override = default;

  [[nodiscard]] std::size_t size() const final;

  void remove(std::size_t index) final;

  [[nodiscard]] bool validate(std::size_t index) const final;

  std::size_t add(std::function<void(const M &)> subscriber);

  void publish(const M &message);

 private:
  std::size_t m_index = 0;
  std::map<std::size_t, std::function<void(const M &)>> m_subscribers;
};

template<typename M>
std::size_t MessageContainer<M>::size() const {
  return m_subscribers.size();
}

template<typename M>
void MessageContainer<M>::remove(std::size_t index) {
  auto iterator = m_subscribers.find(index);

  if (iterator != m_subscribers.end()) {
    m_subscribers.erase(iterator);
  }
}

template<typename M>
bool MessageContainer<M>::validate(std::size_t index) const {
  return m_subscribers.find(index) != m_subscribers.end();
}

template<typename M>
std::size_t MessageContainer<M>::add(std::function<void(const M &)> subscriber) {
  m_subscribers.insert({m_index, subscriber});
  ++m_index;
  return m_index - 1U;
}

template<typename M>
void MessageContainer<M>::publish(const M &message) {
  for (auto &subscriber : m_subscribers) {
    subscriber.second(message);
  }
}
}  // namespace pub
