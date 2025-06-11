#pragma once

#include "../SceneType.h"
#include "../Scene.h"
#include "../../GameContext.h"
#include "TGUI/Widgets/Button.hpp"

class InGameManager;
class HomeBaseScene : public Scene {
  std::string path;
  InGameManager *parent;
 public:
  explicit HomeBaseScene(InGameManager *parent, GameContext &context,
                         std::string path = "forms/homebase.txt") :
      Scene(context),
      path(std::move(path)), parent(parent) {
  };

  void clear() override {
  }

  void loadGraphics() override;
  void handleEvent(const std::optional<sf::Event> &_event) override {};
};
