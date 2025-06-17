#pragma once
#include "Scene.h"

class DummyScene : public Scene {
 public:
  explicit DummyScene(GameContext &context) : Scene(context) {}
  void handleEvent(const std::optional<sf::Event> &_event) override {}
  void loadGraphics() override {}
  void clear() override {}
}; 