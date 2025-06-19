
#include "InGameManager.h"
#include "../MainManager.h"

InGameManager::InGameManager(MainManager *parent, GameContext &context) :
    SceneManager<State::InGame>(context),
    parent(parent) {

}

void InGameManager::returnToBase(const RunResult runResult) {
  if (runResult.reason == RunResult::Reason::Death) {
    takeStableMoney(getDeathPenalty());
    changeScene(State::InGame::Death);
  } else {
    addStableMoney(runResult.money);
    changeScene(State::InGame::Base);
  }
}
void InGameManager::saveReturnToMenu() {
  parent->changeScene(State::Menu::MainMenu);

}

