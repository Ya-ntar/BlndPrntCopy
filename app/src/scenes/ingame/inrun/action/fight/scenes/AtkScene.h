
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
      : bus(bus), textProcessor(th), time(time) {
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
      case textProcessing::CharCheckState::CORRECT_INPUT:textProcessor.next();
        hits++;
        break;
      case textProcessing::CharCheckState::INCORRECT_INPUT:break;
      case textProcessing::CharCheckState::NOTHING:break;
    }
    bus.add(textProcessing::TextProcessStateMsg{.val = val});
    currentState = val;
    bus.add(textProcessing::AttackLogicStateChange{.state = currentState,
        .currentTextHolder= textProcessor.getTextHolder(), .hits = hits});
  }

};

class InputProcessor {

 public:
  explicit InputProcessor() = default;

  virtual void handleEvent(const std::optional<sf::Event> &_event) = 0;
};

class AtkInput : InputProcessor {
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
  std::optional<textProcessing::RunningLineDisplay> runningLine;
  std::optional<HitsTime> hitsTime;

  Layout &menuBase;

 public:
  explicit AttackMenu(Layout &menuBase, std::string path = "base.txt") :
      menuBase(menuBase), path(std::move(path)) {
  }

  [[nodiscard]] bool wasCreated() const {
    return menuBase.hasBeenLoaded() && runningLine.has_value();
  }

  void load() {
    menuBase.load();
    //mobBase.load();
    tgui::Gui tempGui;
    try { tempGui.loadWidgetsFromFile(path); }
    catch (const tgui::Exception &e) {
      std::cerr << "Failed to load Attack widgets: " << e.what() << std::endl;
      throw e;
    }

    auto active_char = tempGui.get<tgui::TextArea>("current_char");
    auto other_string = tempGui.get<tgui::TextArea>("other_string");
    auto running_line = tempGui.get<tgui::Group>("running_line");

    auto gp = tempGui.get<tgui::Group>("time_hits_group");
    auto hitArea = tempGui.get<tgui::TextArea>("hit_area");
    auto timeArea = tempGui.get<tgui::TextArea>("time_area");

    tempGui.remove(running_line);
    tempGui.remove(gp);

    {
      menuBase.getMainFreeSpace()->add(running_line);
      running_line->setPosition({"5%", "0%"});
      running_line->setSize({"90%", "50%"});
      other_string->setCaretPosition(0);
    }
    {
      menuBase.getTopFreeSpace()->add(gp);
      gp->setPosition({"0%", "0%"});
      gp->setSize({"100%", "100%"});
      timeArea->setText("");
      hitArea->setText("0");
    }

    hitsTime.emplace(ATK_TIMER_NAME, hitArea, timeArea,
                     menuBase.getContext().queueBus); //toDo: подумать о проблеме типов таймера
    runningLine.emplace(active_char, other_string, menuBase.getPlayer(), menuBase.getContext().queueBus);
    runningLine.value().update();

  }

  void clear() {
    menuBase.clear();
    runningLine.reset();

  }

  tgui::Gui &getGui() {
    return menuBase.getGui();
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
  AtkScene(InFightManager *parent, RunInfo &runInfo, Layout &base,
           std::string pathToAtk = "forms/attack_widgets.txt");

  void handleEvent(const std::optional<sf::Event> &_event) override {
    atkInput.handleEvent(_event);
  };

  void load() override;

  void loadGraphics() override;

  int32_t endAtk() {
    menu.clear();
    return logic.end();
  }

  void clear() override;

  void subscribeToAll();
};