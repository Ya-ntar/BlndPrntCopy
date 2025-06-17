#include "InRunManager.h"
#include "../InGameManager.h"
#include "InventoryMenu.h"
#include "CrossroadScene.h"
#include "BookMenu.h"

InRunManager::InRunManager(InGameManager *parent, GameContext &context) :
    SceneManager<State::InRun>(context),
    parent(parent),  info(context), menuBase(context, info),
    gen(this, context, info, menuBase) {
  SceneInstruction::setToZero(); // toDo: подумать а тут ли ему место?

}

std::unique_ptr<Scene> InRunManager::createScene(State::InRun scene) {
  switch (scene) {
    case State::InRun::Scene:return (gen.generateScene(nextScene));
    case State::InRun::Crossroad:return std::make_unique<CrossroadScene>(info, menuBase, this, gen);
    default:return nullptr;

  }
}

void InRunManager::playerDeath(const player::PlayerDeath &death) {
  parent->returnToBase({RunResult::Reason::Death});
}

void InRunManager::ranAwayHome() {
  parent->returnToBase({RunResult::Reason::RanAway});
}

void InRunManager::subscribeAll() {

  subscribeSmartly<player::PlayerDeath>(
      [this](const player::PlayerDeath &death) {
        playerDeath(death);
      });

}

void InRunManager::ranAwayFromAFight() {
  changeScene(State::InRun::Crossroad);
}

void InRunManager::showInventory(ItemAffected &mainScene) {
  addScene(std::make_unique<InventoryMenu>(context, info, mainScene,  *this));
  activeScenes[activeScenes.size() - 1]->loadGraphics();
}
void InRunManager::showChangeArmour(ItemAffected &mainScene) {
  addScene(std::make_unique<InventoryMenu>(context, info, mainScene,  *this));
  activeScenes[activeScenes.size() - 1]->loadGraphics();
}

void InRunManager::showBookMenu() {
  addScene(std::make_unique<BookMenu>(context, info, *this));
  activeScenes[activeScenes.size() - 1]->loadGraphics();
}


