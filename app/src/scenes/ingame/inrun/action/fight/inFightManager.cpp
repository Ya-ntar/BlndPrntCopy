#include <iostream>
#include "InFightManager.h"
#include "../../InRunManager.h"
#include "scenes/DefendScene.h"

std::unique_ptr<Scene> InFightManager::createScene(State::InFight scene) {
  switch (scene) {
    case State::InFight::Atk: return std::make_unique<AtkScene>(this, runInfo, context);
    case State::InFight::BattleMenu : return std::make_unique<BattleMenuScene>(this, menuBase);
    case State::InFight::Inactive: return nullptr;
    case State::InFight::Defend: return std::make_unique<DefendScene>(this, menuBase, runInfo, fightInfo);
  }
  return nullptr;
}

InFightManager::InFightManager(InRunManager *parent,
                               GameContext &context,
                               RunInfo &runInfo,  // хочу проглотить fightInfo
                               FightInfo &fightInfo,
                               Layout &base) :
    SceneManager<State::InFight>(context),
    runInfo(runInfo),
    fightInfo(std::move(fightInfo)),
    menuBase(base),
    parent(parent) {

  subscribeToAll();
  addReaction("InkBottle", [&](const Item &item) {
    std::cout << "Reaction to InkBottle! Name: " << item.getName() << "\n";
    runInfo.player.healHP(3);
    std::cout << "Player was healed for three hp" << "\n";
  }, "Лечит три здоровья.");

}

void InFightManager::runAway() {
  parent->ranAwayFromAFight();
}

void InFightManager::showInventory() {
  parent->showInventory(*this);
}

void InFightManager::subscribeToAll() {
  subscribeSmartly<mob::MobChangedMsg>(
      [this](const mob::MobChangedMsg &msg) {
       fightInfo.mobs;
      }
  );
  /*subscribeSmartly<mob::MobAttacking>(
          [this](const mob::MobAttacking &msg) {
              std::this_thread::sleep_for(std::chrono::milliseconds{800});
          }
  );*/
}

void InFightManager::startTurn() {
  if (turn == "Attack") { //toDo: Change!
    changeScene(State::InFight::Atk);
    turn = "Defense";
  } else {
    changeScene(State::InFight::Defend);
    turn = "Attack";
  }

}

void InFightManager::damageMobs(fight::AtkEnded msg) {
  for (auto *mob : fightInfo.currentlyUnderAttack) {
    mob->takeDamage(msg.hits * runInfo.player.getPwr());
  }
}


