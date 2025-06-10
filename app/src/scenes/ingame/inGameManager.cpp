
#include "InGameManager.h"
#include "../MainManager.h"

InGameManager::InGameManager(MainManager *parent, GameContext &context) :
    SceneManager<State::InGame>(context),
    parent(parent) {

}

void InGameManager::returnToBase(const RunResult runResult) {
  if (runResult.reason == RunResult::Reason::Death) {
    changeScene(State::InGame::Death);
  } else {
    changeScene(State::InGame::Base);
  }
}

