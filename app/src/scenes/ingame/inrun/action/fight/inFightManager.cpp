#include <iostream>
#include "InFightManager.h"
#include "../../InRunManager.h"
#include "scenes/DefendScene.h"
#include "../../InventoryMenu.h"

std::unique_ptr<Scene> InFightManager::createScene(State::InFight scene) {
  layout.load();
  switch (scene) {
    using
    enum State::InFight;
    case Atk:return std::make_unique<AtkScene>(this, run_info_, context);
    case BattleMenu :return std::make_unique<BattleMenuScene>(this, context);
    case Inactive:return nullptr;
    case Defend:return std::make_unique<DefendScene>(this, context, run_info_, info);
  }
  return nullptr;
}

InFightManager::InFightManager(InRunManager *parent,
                               GameContext &context,
                               RunInfo &runInfo,  // хочу проглотить fightInfo
                               FightInfo &fightInfo,
                               Layout &layout) :
    SceneManager<State::InFight>(context),
    info(std::move(fightInfo)),
    ItemReactionConfigurable(runInfo, info),
    run_info_(runInfo),
    layout(layout),
    parent(parent),
    mob_selectable_(std::make_shared<mob::MobSelectable>(info)) {

  subscribeToAll();
  loadFMobReactionsFromConfig(kItemConfig);
  // sub to items

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
        if (msg.val == mob::State::DIED) {
          info.removeMob(msg.target);
          if (info.mobs.empty()) {
            parent->winAFight(info);
          }
        }
        mob_selectable_->load();
      }
  );
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
  for (auto *mob : info.currently_under_attack) {
    mob->takeDamage(msg.hits * run_info_.player.getPwr());
  }
}


