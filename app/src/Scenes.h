#pragma once

// --- Внешние библиотеки ---
// #include <SFML/Window.hpp> // удалено, чтобы не было ошибки
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/RichTextLabel.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/SeparatorLine.hpp>
#include <TGUI/Widgets/ScrollablePanel.hpp>
#include <TGUI/Widgets/GrowVerticalLayout.hpp>
#include <TGUI/Widgets/GrowHorizontalLayout.hpp>
#include <optional>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <map>

// --- Внутренние зависимости ---
#include "GameContext.h"
#include "utils/SmartSubscriber.h"
#include "utils/QueuedMessageBus.h"
#include "utils/SmartTimer.h"
#include "world_player_and_container/Player.h"
#include "world_player_and_container/Mob.h"
#include "world_player_and_container/ItemManager.h"
#include "text_processing/backEnd/TextProcessor.h"
#include "text_processing/backEnd/tools/TextHolder.h"
#include "text_processing/backEnd/tools/TextSource.h"

// --- State (SceneType) ---
namespace State {
enum class Menu { MainMenu, StartGame, Credits };
enum class InGame { Inactive, Base, InRun, Death };
enum class InRun { Inactive, Scene, Crossroad, Inventory };
enum class InFight { Inactive, BattleMenu, Atk, Defend };
}

// --- Forward declarations ---
class MainManager;
class InGameManager;
class InRunManager;
class InFightManager;
class MenuScene;
class HomeBaseScene;
class DeathScene;
class InventoryMenu;
class BattleMenuScene;
class AtkScene;
class DefendMenu;
class Layout;
class BasicMobMenu;

class Scene : public SmartSubscriber {

 public: //ToDo: fix it

  GameContext &context;

  explicit Scene(GameContext &context) : SmartSubscriber(context.queueBus), context(context) {};

  virtual void handleEvent(const std::optional<sf::Event> &_event) = 0;

  virtual void processDeferred() {};

  virtual void load() { loadGraphics(); };

  virtual void loadGraphics() = 0;

  virtual void clear() = 0;

  ~Scene() override = default;
};

template<typename CHILD_ID>
class SceneManager : public Scene {
 protected:
  std::vector<std::unique_ptr<Scene>> activeScenes;
  std::optional<CHILD_ID> chgToScene;
  bool closeLastScene = false;

  void closeAndChange(std::unique_ptr<Scene> newScene) {
    close();
    if (newScene) {
      newScene->load();
      activeScenes.push_back(std::move(newScene));
    }
  }

  void close() {
    if (!activeScenes.empty()) {
      activeScenes[activeScenes.size() - 1]->clear();
      activeScenes.pop_back();
    }
  }

  virtual std::unique_ptr<Scene> createScene(CHILD_ID) = 0;

 public:
  ~SceneManager() override = default;

  explicit SceneManager(GameContext &context) :
      Scene(context) {};

  void changeScene(CHILD_ID scene) {
    if (chgToScene.has_value() || closeLastScene) {
      throw std::invalid_argument("Scene change already in progress!");
    }
    chgToScene.emplace(scene);
  }

  void addScene(CHILD_ID scene) {
    activeScenes.push_back(std::move(createScene(scene)));
  }

  void addScene(std::unique_ptr<Scene> scene) {
    activeScenes.push_back(std::move(scene));
  }

  void deleteScene() {
    if (chgToScene.has_value() || closeLastScene) {
      throw std::invalid_argument("Scene change already in progress!");
    }
    closeLastScene = true;
  }

  void processDeferred() override {
    if (chgToScene.has_value()) {
      closeAndChange(createScene(chgToScene.value()));
    }
    if (closeLastScene) {
      close();
      loadGraphics();
    }
    closeLastScene = false;
    chgToScene.reset();
    for (auto const &layer : activeScenes) {
      layer->processDeferred();
    }
  }

  void clear() override {
    for (auto const &layer : activeScenes) {
      layer->clear();
    }
  }

  void handleEvent(const std::optional<sf::Event> &_event) override {
    for (auto const &layer : activeScenes) {
      layer->handleEvent(_event);
    }
  };

  void loadGraphics() override {
    for (auto const &layer : activeScenes) {
      layer->loadGraphics();
    }
  }
};
