#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../src/scenes/Scene.h"
#include "../src/scenes/DummyScene.h"
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../src/GameContext.h"
#include <optional>
#include "../src/scenes/ingame/inrun/InRunManager.h"
#include "../src/scenes/SceneType.h"

// Dummy ID для SceneManager
enum class DummyID { First, Second, Third };

static sf::RenderWindow dummyWindow;

// DummyManager для простых тестов
class DummyManager : public SceneManager<DummyID> {
 public:
  using SceneManager::SceneManager;
  std::unique_ptr<Scene> createScene(DummyID) override {
    return std::make_unique<DummyScene>(context);
  }
};

// DummyScene с подсчётом вызовов
class CountingDummyScene : public Scene {
 public:
  int handleEventCount = 0;
  int clearCount = 0;
  int loadGraphicsCount = 0;

  using Scene::Scene;

  void handleEvent(const std::optional<sf::Event> &) override { handleEventCount++; }
  void loadGraphics() override { loadGraphicsCount++; }
  void clear() override {
    clearCount++;
  }
};


class CountingDummyManager : public SceneManager<DummyID> {
 public:
  std::vector<CountingDummyScene *> createdScenes;
  using SceneManager::SceneManager;
  std::unique_ptr<Scene> createScene(DummyID) override {
    auto ptr = std::make_unique<CountingDummyScene>(context);
    createdScenes.push_back(ptr.get());
    return ptr;
  }

};

TEST_CASE("SceneManager basic add and delete", "[SceneManager]") {
  GameContext ctx(dummyWindow);
  DummyManager manager(ctx);
  REQUIRE(manager.activeScenes.empty());
  manager.addScene(DummyID::First);
  REQUIRE(manager.activeScenes.size() == 1);
  manager.addScene(DummyID::Second);
  REQUIRE(manager.activeScenes.size() == 2);
  manager.deleteScene();
  manager.processDeferred();
  REQUIRE(manager.activeScenes.size() == 1);
}

TEST_CASE("SceneManager changeScene replaces scene", "[SceneManager]") {
  using enum DummyID;
  GameContext ctx(dummyWindow);
  DummyManager manager(ctx);
  manager.addScene(First);
  manager.addScene(Second);
  REQUIRE(manager.activeScenes.size() == 2);
  manager.changeScene(Third);
  manager.processDeferred();
  REQUIRE(manager.activeScenes.size() == 1);
}

TEST_CASE("SceneManager multiple add/delete/change", "[SceneManager]") {
  using enum DummyID;
  GameContext ctx(dummyWindow);
  DummyManager manager(ctx);
  manager.addScene(First);
  manager.addScene(Second);
  manager.deleteScene();
  manager.processDeferred();
  REQUIRE(manager.activeScenes.size() == 1);
  manager.changeScene(Third);
  manager.processDeferred();
  REQUIRE(manager.activeScenes.size() == 1);
  manager.deleteScene();
  manager.processDeferred();
  REQUIRE(manager.activeScenes.size() == 0);
}

TEST_CASE("SceneManager cycle add/delete/change", "[SceneManager]") {
  GameContext ctx(dummyWindow);
  CountingDummyManager manager(ctx);

  manager.addScene(DummyID::First);
  manager.processDeferred();
  REQUIRE(manager.activeScenes.size() == 1);
  for (auto *scene : manager.createdScenes) {
    scene->handleEvent(std::nullopt);
    scene->clear();
    scene->loadGraphics();
  }
  manager.addScene(DummyID::Second);
  manager.processDeferred();
  REQUIRE(manager.activeScenes.size() == 2);

  manager.changeScene(DummyID::Third);
  manager.processDeferred();
  REQUIRE(manager.activeScenes.size() == 2);

  for (auto const &scene : manager.activeScenes) {
    scene->handleEvent(std::nullopt);
    scene->clear();
    scene->loadGraphics();
  }

  manager.deleteScene();
  manager.processDeferred();
  REQUIRE(manager.activeScenes.empty());
  manager.processDeferred();
  manager.processDeferred();
  manager.processDeferred();
}

TEST_CASE("InRunManager add, delete, and change scenes", "[InRunManager]") {
    GameContext ctx(dummyWindow);
    InRunManager manager(nullptr, ctx);

    REQUIRE(manager.activeScenes.empty());

    manager.addScene(State::InRun::Crossroad);
    REQUIRE(manager.activeScenes.size() == 1);

    manager.addScene(State::InRun::Inventory);
    REQUIRE(manager.activeScenes.size() == 2);


    manager.deleteScene();
    manager.processDeferred();
    REQUIRE(manager.activeScenes.size() == 1);

    // Меняем сцену на Scene
    manager.changeScene(State::InRun::Scene);
    manager.processDeferred();
    REQUIRE(manager.activeScenes.size() == 1);

    // Удаляем последнюю сцену
    manager.deleteScene();
    manager.processDeferred();
    REQUIRE(manager.activeScenes.empty());
}