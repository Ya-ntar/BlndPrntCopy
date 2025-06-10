// Code from project [PubBus] (https://github.com/eXpl0it3r/PubBus/tree/faa3f4e4818be5fd86424986bd3bf9b0bd6a876e/include/PubBus)
// Author: [eXpl0it3r] license: Public Domain.
#pragma once

#include <cstddef>
#include "Message.hpp"

namespace pub {
class SubscriberHandle {
 public:
  SubscriberHandle(Message::Id id, std::size_t index);

  [[nodiscard]] Message::Id id() const;

  [[nodiscard]] std::size_t index() const;

 private:
  Message::Id m_id;
  std::size_t m_index;
};

inline SubscriberHandle::SubscriberHandle(const Message::Id id, const std::size_t index)
    : m_id(id), m_index(index) {

}

inline Message::Id SubscriberHandle::id() const {
  return m_id;
}

inline std::size_t SubscriberHandle::index() const {
  return m_index;
}
}  // namespace pub
