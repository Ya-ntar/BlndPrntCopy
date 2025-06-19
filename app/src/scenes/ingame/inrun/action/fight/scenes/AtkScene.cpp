#include "AtkScene.h"
#include "../InFightManager.h"
#include "../../../../../../utils/WidgetConfigurator.h"

void AtkScene::load() {

  menu.load();
  loadGraphics();
  logic.start();
}

AtkScene::AtkScene(InFightManager *parent, RunInfo &runInfo, GameContext &context, std::string pathToAtk)
    : Scene(context),
      parent(parent),
      logic(context.queueBus, runInfo.getTextHolder(), runInfo.player.getTime()),
      atkInput(logic),
      menu(context, runInfo, std::move(pathToAtk)) {
  subscribeToAll();
}

void AtkScene::subscribeToAll() {
  subscribeSmartly<TimerMsg>(
      [this](const TimerMsg &msg) {
        if (msg.timerType == ATK_TIMER_NAME && msg.is_finished) {
          parent->changeScene(State::InFight::BattleMenu);
          return;
        }

      });
}

void AtkScene::loadGraphics() {
  menu.clearVisual();
  menu.createWidgets();
  menu.load();
  if (auto info = context.gui.get<tgui::TextArea>("menu_info")) {
    info->setText("Attack");
  }

  auto const &gui = menu.getGui();
  if (auto backButton = gui.get<tgui::Button>("back")) {
    WidgetConfigurator::configureBackButton(backButton, "End Attack");
    backButton->onPress([this] {
      parent->changeScene(State::InFight::BattleMenu);
    });
  }
}

void AtkScene::clear() {
  parent->damageMobs(fight::AtkEnded{.hits = endAtk()});
}

