// Code from project [PubBus] (https://github.com/eXpl0it3r/PubBus/tree/faa3f4e4818be5fd86424986bd3bf9b0bd6a876e/include/PubBus)
// Author: [eXpl0it3r] license: Public Domain.
#pragma once

#include <typeindex>

namespace pub {
struct Message {
  using Id = std::type_index;

  template<typename T>
  static Id id();
};

template<typename T>
Message::Id Message::id() {
  return std::type_index{typeid(T)};
}
}  // namespace pub
