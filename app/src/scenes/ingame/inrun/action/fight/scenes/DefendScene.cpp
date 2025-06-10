#include "DefendScene.h"
#include "../InFightManager.h"

#include <thread>
#include <chrono>

void DefendScene::loadGraphics() { //toDo: переделать...
  base.load();
  for (auto &mob : fightInfo.mobs) {

    context.queueBus.
        add(mob::MobAttacking{.damage = mob.getType().getAttackPower(), .target = mob.getUniqueId()});
    //runInfo.player.damageHP(mob.getType().getAttackPower());
  };
  parent->changeScene(State::InFight::BattleMenu);
}
