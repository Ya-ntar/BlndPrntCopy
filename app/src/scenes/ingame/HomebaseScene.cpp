//
// Created by user on 11.05.2025.
//
#include "HomebaseScene.h"
#include "InGameManager.h"
#include "../../utils/ErrorHandler.h"
#include "../../utils/WidgetConfigurator.h"

void HomeBaseScene::loadGraphics() {
  tgui::Gui &gui = context.gui;
  try { gui.loadWidgetsFromFile(path); }
  catch (const tgui::Exception &e) {
    throw error::ResourceLoadError("HomeBaseScene", e.what());
  }

  int current = parent->getStableMoney();
  int goal = parent->getWinMoney();
  bool win = parent->isWin();

  auto capitalLabel = tgui::Label::create("Money obtained: " + std::to_string(current) + " / " + std::to_string(goal));
  capitalLabel->setTextSize(20);
  capitalLabel->setPosition({"3.75%", "10%"});
  WidgetConfigurator::configureLabel(capitalLabel);
  gui.add(capitalLabel, "capital_label");

  // Отображение штрафа за смерть
  auto penaltyLabel = tgui::Label::create("Death penalty: -" + std::to_string(parent->getDeathPenalty()));
  penaltyLabel->setTextSize(16);
  penaltyLabel->setPosition({"3.75%", "17%"});
  WidgetConfigurator::configureLabel(penaltyLabel);
  gui.add(penaltyLabel, "penalty_label");

  if (win) {
    auto winLabel = tgui::Label::create("Вы победили!");
    winLabel->setTextSize(28);
    winLabel->setPosition({"3.75%", "25%"});
    gui.add(winLabel, "win_label");

    auto okButton = tgui::Button::create("Ок");
    okButton->setPosition({"3.75%", "40%"});
    okButton->setSize({200, 60});
    WidgetConfigurator::configureButton(okButton);
    okButton->onPress([&] {
      parent->saveReturnToMenu();
    });
    gui.add(okButton, "ok_button");
    return;
  }

  if (auto startRun = gui.get<tgui::Button>("start_run")) {
    startRun->onPress([&] {
      parent->changeScene(State::InGame::InRun);
    });
  }
}

