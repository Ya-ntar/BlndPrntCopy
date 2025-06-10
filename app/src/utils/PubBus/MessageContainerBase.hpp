// Code from project [PubBus] (https://github.com/eXpl0it3r/PubBus/tree/faa3f4e4818be5fd86424986bd3bf9b0bd6a876e/include/PubBus)
// Author: [eXpl0it3r] license: Public Domain.

#pragma once

#include <cstddef>

namespace pub {
struct MessageContainerBase {
  MessageContainerBase() = default;

  MessageContainerBase(MessageContainerBase &&) = default;

  MessageContainerBase(const MessageContainerBase &) = default;

  MessageContainerBase &operator=(MessageContainerBase &&) = default;

  MessageContainerBase &operator=(const MessageContainerBase &) = default;

  virtual ~MessageContainerBase() = default;

  [[nodiscard]] virtual std::size_t size() const = 0;

  virtual void remove(std::size_t index) = 0;

  [[nodiscard]] virtual bool validate(std::size_t index) const = 0;
};
}  // namespace pub
