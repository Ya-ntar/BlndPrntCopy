#pragma once

#include <optional>
#include <SFML/Window/Event.hpp>
#include <utility>
#include "../GameContext.h"
#include "TGUI/Widgets/Group.hpp"

class Loadable {
 public:
  virtual void load() = 0;
  virtual void prepare() {};
  virtual ~Loadable() = default;
};

class Settable : public Loadable {
  std::shared_ptr<tgui::Group> parent;
  std::shared_ptr<tgui::Widget> mainField;

 public:
  void setMain(std::shared_ptr<tgui::Widget> newMain) {
    this->mainField = std::move(newMain);
  }

  void setParent(std::shared_ptr<tgui::Group> newParent) {
    this->parent.reset();
    this->parent = std::move(newParent);
  }

  void connect() const { // единоличное владение?
    if (parent && mainField) {
      parent->removeAllWidgets();
      parent->add(mainField);
    }
  }
};

template<typename PARENT_ID>
class SceneManager;

class Scene : public SmartSubscriber, public Loadable {

 public: //ToDo: fix it

  GameContext &context;

  explicit Scene(GameContext &context) : SmartSubscriber(context.queueBus), context(context) {};

  //logic
  virtual void handleEvent(const std::optional<sf::Event> &_event) = 0;

  virtual void loadGraphics() = 0;

  virtual void clear() = 0;

  virtual void processDeferred() {};

  // output
  void load() override {
    prepare();
    loadGraphics();
  };

  ~Scene() override = default;
};

template<typename CHILD_ID>
class SceneManager : public Scene {
 protected:
  std::optional<CHILD_ID> chgToScene; //toDo: поменять на список из номеров 'на удаление'.
  bool closeLastScene = false;

  void closeAndChange(std::unique_ptr<Scene> newScene) {
    deleteLastScene();
    chgToScene.reset();
    if (newScene) {
      newScene->load();
      activeScenes.push_back(std::move(newScene));
    }
  }

  void deleteLastScene() {
    if (!activeScenes.empty()) {
      activeScenes.back()->clear();
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
      throw std::runtime_error("Scene change already in progress!");
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
      throw std::runtime_error("Scene change already in progress!");
    }
    closeLastScene = true;
  }

  void processDeferred() override {
    while (chgToScene.has_value()) {
      closeAndChange(createScene(chgToScene.value()));
    }
    while (closeLastScene) {
      closeLastScene = false;
      deleteLastScene();
      loadGraphics();
    }

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
    prepare();
    activeScenes.back()->loadGraphics();
  }

  std::vector<std::unique_ptr<Scene>> activeScenes;
};

