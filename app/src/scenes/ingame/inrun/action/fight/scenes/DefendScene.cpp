#include "DefendScene.h"
#include "../InFightManager.h"

#include <thread>
#include <chrono>

void DefendScene::loadGraphics() { //toDo: переделать...
  base.load();
  for (auto const &mob : fightInfo.mobs) {

    context.queueBus.
        add(mob::MobAttacking{.damage = mob.second.getType().getAttackPower(), .target = mob.first});
    //runInfo.player.damageHP(mob.getType().getAttackPower());
  };
  parent->changeScene(State::InFight::BattleMenu);
}
