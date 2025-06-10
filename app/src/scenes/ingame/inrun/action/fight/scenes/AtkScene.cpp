
#include "AtkScene.h"
#include "../InFightManager.h"

void AtkScene::load() {
  menu.load();
  loadGraphics();
  logic.start();
}

AtkScene::AtkScene(InFightManager *parent, RunInfo &runInfo, Layout &base, std::string pathToAtk) :
    parent(parent),
    Scene(base.getContext()),
    logic(base.getContext().queueBus, runInfo.player.getTextHolder(), runInfo.player.getTime()),
    atkInput(logic),
    menu(base, std::move(pathToAtk)) {

  subscribeToAll();

}

void AtkScene::subscribeToAll() {
  subscribeSmartly<TimerMsg>(
      [this](const TimerMsg &msg) {
        if (msg.timerType == ATK_TIMER_NAME && msg.is_finished) {
          parent->changeScene(State::InFight::BattleMenu);;
          return;
        };

      });
}

void AtkScene::loadGraphics() {
  auto &gui = menu.getGui();
  if (auto backButton = gui.get<tgui::Button>(BACK_BUTTON)) {
    backButton->onPress([&] {
      parent->changeScene(State::InFight::BattleMenu);;
    });
  }
}

void AtkScene::clear() {
  parent->damageMobs(fight::AtkEnded{.hits = endAtk()});
}
