#include "InRunManager.h"
#include "../InGameManager.h"
#include "InventoryMenu.h"
#include "CrossroadScene.h"
#include "BookMenu.h"
#include "VictoryScene.h"

InRunManager::InRunManager(InGameManager *parent, GameContext &context) :
    SceneManager<State::InRun>(context),
    parent(parent), run_info_(context), layout(context, run_info_),
    gen(this, context, run_info_, layout) {
  SceneInstruction::setToZero(); // toDo: подумать а тут ли ему место?

}

std::unique_ptr<Scene> InRunManager::createScene(State::InRun scene) {
  layout.load();
  switch (scene) {
    case State::InRun::SCENE:return (gen.generateScene(nextScene));
    case State::InRun::CROSSROAD:return std::make_unique<CrossroadScene>(run_info_, layout, this, gen);
    case State::InRun::WON_BATTLE:return std::make_unique<VictoryScene>(run_info_, layout, this);
    default:return nullptr;

  }
}

void InRunManager::playerDeath(const player::PlayerDeath &death) {
  parent->returnToBase({RunResult::Reason::Death});
}

void InRunManager::ranAwayHome() {
  parent->returnToBase({RunResult::Reason::RanAway, run_info_.getCurrentCoins()});
}

void InRunManager::subscribeAll() {

  subscribeSmartly<player::PlayerDeath>(
      [this](const player::PlayerDeath &death) {
        playerDeath(death);
      });

}

void InRunManager::ranAwayFromAFight() {
  changeScene(State::InRun::CROSSROAD);
}

void InRunManager::showInventory(ItemAffected &mainScene) {
  layout.load();
  addScene(std::make_unique<InventoryMenu>(context, run_info_, mainScene, *this));
  activeScenes[activeScenes.size() - 1]->loadGraphics();
}

void InRunManager::showChangeArmour(ItemAffected &mainScene) {
  addScene(std::make_unique<InventoryMenu>(context, run_info_, mainScene, *this));
  activeScenes[activeScenes.size() - 1]->loadGraphics();
}

void InRunManager::showBookMenu() {
  addScene(std::make_unique<BookMenu>(context, run_info_, *this));
  activeScenes[activeScenes.size() - 1]->loadGraphics();
}
void InRunManager::winAFight(const FightInfo &fight_info) {
  std::cout << "+" + std::to_string(fight_info.totalCoins) << '\n';
  run_info_.addCoins(fight_info.totalCoins);
  run_info_.setLog(std::to_string(fight_info.totalCoins) + " coins \n");
  layout.clear();
  changeScene(State::InRun::WON_BATTLE);

}


