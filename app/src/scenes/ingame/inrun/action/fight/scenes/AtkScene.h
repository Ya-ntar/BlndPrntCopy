#pragma once

#include "TGUI/Backend/SFML-Graphics.hpp"
#include <utility>
#include "../../../../../Scene.h"
#include "../../../../../SceneType.h"

#include "../../../../../../text_processing/backEnd/TextProcessor.h"

#include "../tools/RunningLineDisplay.h"
#include "../tools/HitsTime.h"
#include "../../../base/Layout.h"
#include "../../../../../../utils/SmartTimer.h"
#include "../../../../../../utils/WidgetConfigurator.h"

static constexpr std::string ATK_TIMER_NAME = "AtkTimer";

class AttackLogic {
 private:
  QueuedMessageBus &bus;
  textProcessing::CharCheckState currentState = textProcessing::CharCheckState::NOTHING;
  const float &time;

  SmartTimer timer{bus, ATK_TIMER_NAME, time};
  textProcessing::TextProcessor textProcessor;
  int32_t hits = 0;

 public:

  AttackLogic(QueuedMessageBus &bus, TextHolder &th, const float &time)
      : bus(bus), time(time), textProcessor(th) {
  }

  void start() {
    hits = 0;
    timer.startTimer();
  }

  int32_t end() {
    timer.end();
    return hits;

  }

  void processSymbol(const string &symbol) {
    auto val = textProcessor.checkKey(symbol);
    switch (val) {
      using
      enum textProcessing::CharCheckState;
      case CORRECT_INPUT:textProcessor.next();
        hits++;
        break;
      case INCORRECT_INPUT:break;
      case NOTHING:break;
    }
    bus.add(textProcessing::TextProcessStateMsg{.val = val});
    currentState = val;
    bus.add(textProcessing::AttackLogicStateChange{.state = currentState, .hits = hits});
  }

};

class InputProcessor {

 public:
  explicit InputProcessor() = default;
  virtual ~InputProcessor() = default;
  virtual void handleEvent(const std::optional<sf::Event> &_event) = 0;
};

class AtkInput : public InputProcessor {
 private:
  AttackLogic &logic;

 public:
  explicit AtkInput(AttackLogic &logic) : logic(logic) {};

  void handleEvent(const std::optional<sf::Event> &_event) override {
    if (const auto *textEntered = _event->getIf<sf::Event::TextEntered>()) {
      string st = textEntered->unicode;
      if (st == U'\r') { //toDo: FIX!!!
        st = U'\n';
      }
      logic.processSymbol(st);
      std::cout << "ASCII character typed: " << static_cast<char>(textEntered->unicode) << std::endl;
    }

  }
};

class AttackMenu {
  std::string path;
  GameContext &context;
  RunInfo &run_info_;
  std::shared_ptr<tgui::Panel> mainPanel;
  std::shared_ptr<tgui::Group> runningLineGroup;
  std::shared_ptr<tgui::Group> timeHitsGroup;
  std::shared_ptr<tgui::RichTextLabel> textDisplay;
  std::shared_ptr<tgui::RichTextLabel> hitArea;
  std::shared_ptr<tgui::RichTextLabel> timeArea;
  std::optional<HitsTime> hitsTime;
  textProcessing::RunningLineDisplay runningLine;

  void updateText(const tgui::String &currentChar, const tgui::String &otherText) {
    if (currentChar.empty()) {
      textDisplay->setText("");
      return;
    }
    textDisplay->setText("<color=red>" + currentChar + "</color><color=white>" + otherText + "</color>");
  }

 public:
  explicit AttackMenu(GameContext &context, RunInfo &run_info_, std::string path) :
      path(std::move(path)),
      context(context),
      run_info_(run_info_),
      runningLine(nullptr, nullptr, run_info_, context.queueBus) {
  }

  void load() {
    createWidgets();
    hitsTime.emplace(ATK_TIMER_NAME, hitArea, timeArea, context.queueBus);
    runningLine.update();
  }

  void clearVisual() {
    context.gui.remove(mainPanel);
  }
  void clear() {
    hitsTime.reset();

    if (mainPanel) {
      context.gui.remove(mainPanel);
      mainPanel.reset();
      runningLineGroup.reset();
      timeHitsGroup.reset();
      textDisplay.reset();
      hitArea.reset();
      timeArea.reset();
    }
  }

  tgui::Gui &getGui() {
    return context.gui;
  }
  void createWidgets() {
    mainPanel = tgui::Panel::create();
    WidgetConfigurator::configurePanel(mainPanel);

    auto middle = context.gui.get<tgui::Panel>("main_free_space");
    middle->add(mainPanel);

    runningLineGroup = tgui::Group::create();
    runningLineGroup->setPosition("5%", "0%");
    runningLineGroup->setSize("90%", "30%");
    mainPanel->add(runningLineGroup);

    textDisplay = tgui::RichTextLabel::create();
    WidgetConfigurator::configureRichTextLabel(textDisplay, tgui::Color::Black, tgui::Color::White,
                                               tgui::Color::White, tgui::VerticalAlignment::Center,
                                               tgui::HorizontalAlignment::Left);
    runningLineGroup->add(textDisplay);

    timeHitsGroup = tgui::Group::create();
    timeHitsGroup->setPosition("0%", "0%");
    timeHitsGroup->setSize("100%", "100%");

    auto top = context.gui.get<tgui::Panel>("top_free_space");
    top->add(timeHitsGroup);

    auto statsLayout = tgui::HorizontalLayout::create();
    statsLayout->setSize("100%", "100%");
    timeHitsGroup->add(statsLayout);

    // Create hit area
    hitArea = tgui::RichTextLabel::create();
    WidgetConfigurator::configureRichTextLabel(hitArea, tgui::Color::Black, tgui::Color::Black,
                                               tgui::Color::White, tgui::VerticalAlignment::Center,
                                               tgui::HorizontalAlignment::Center, 17);
    hitArea->setText("<color=white>Hits: 0</color>");
    statsLayout->add(hitArea);

    timeArea = tgui::RichTextLabel::create();
    WidgetConfigurator::configureRichTextLabel(timeArea, tgui::Color::Black, tgui::Color::Black,
                                               tgui::Color::White, tgui::VerticalAlignment::Center,
                                               tgui::HorizontalAlignment::Center, 17);
    timeArea->setText("<color=white>Time: </color>");
    statsLayout->add(timeArea);

    runningLine.set(textDisplay);
  }
};

class InFightManager;

class AtkScene : public Scene {
 private:
  InFightManager *parent;
  AttackLogic logic; //toDo: подумать а не ссылка ли
  AtkInput atkInput; // Дополнительный input, отличный от tgui
  AttackMenu menu;

 public:
  AtkScene(InFightManager *parent, RunInfo &runInfo, GameContext &base,
           std::string pathToAtk = "forms/attack_widgets.txt");

  void handleEvent(const std::optional<sf::Event> &_event) override {
    atkInput.handleEvent(_event);
  };

  void load() override;

  void loadGraphics() override;

  int32_t endAtk() {
    return logic.end();
  }

  void clear() override;

  void subscribeToAll();
};