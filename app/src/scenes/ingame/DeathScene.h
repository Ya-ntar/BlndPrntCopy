#pragma once

#include "TGUI/Widgets/Button.hpp"
#include "../../GameContext.h"
#include "../Scene.h"

class InGameManager;
class DeathScene : public Scene {
  GameContext &context;
  std::string path;
  InGameManager *parent;

 public:
  explicit DeathScene(InGameManager *parent, GameContext &context,
                      std::string path = "forms/death.txt") :
      Scene(context), parent(parent), context(context), path(std::move(path)) {
  };

  void loadGraphics() override;
  void clear() override {
  }

  void handleEvent(const std::optional<sf::Event> &_event) override {};
};
