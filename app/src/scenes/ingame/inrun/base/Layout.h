#pragma once
#include <thread>
#include <format>
#include <chrono>
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <optional>
#include <memory>
#include <string>
#include <iostream>
#include "../../../../GameContext.h"
#include "RunInfoFront.h"
#include "../../../../scenes/Scene.h"

// technical

class SelectableTemplate {
 protected:
  std::shared_ptr<tgui::Group> tmpl;
  std::shared_ptr<tgui::RichTextLabel> pointer;
  std::shared_ptr<tgui::RichTextLabel> name;
  std::shared_ptr<tgui::RichTextLabel> pic;
  std::shared_ptr<tgui::RichTextLabel> info;

  void loadTemplateWidgets(const std::string &formPath, const std::string &widgetName) {
    tgui::Gui tempGui;
    tempGui.loadWidgetsFromFile(formPath);
    tmpl = tempGui.get<tgui::Group>("Template");
    pointer = tmpl->get<tgui::RichTextLabel>("Pointer");
    name = tmpl->get<tgui::RichTextLabel>("Name");
    pic = tmpl->get<tgui::RichTextLabel>("Picture");
    info = tmpl->get<tgui::RichTextLabel>("Info");
    if (!widgetName.empty()) {
      tmpl->setWidgetName(widgetName);
      std::cout << widgetName << std::endl;
    }

    tempGui.removeAllWidgets();
  }

  SelectableTemplate() = default;

 public:
  std::shared_ptr<tgui::Group> getTmpl() { return tmpl; }
};

template<typename Target, typename Parent>
class SelectableFront : public SelectableTemplate {
 protected:
  std::shared_ptr<tgui::HorizontalLayout> layout;
  Target *target = nullptr;
  Parent *parent = nullptr;
 public:
  SelectableFront(std::shared_ptr<tgui::HorizontalLayout> layout, Target *target, Parent *parent)
      : SelectableTemplate(), layout(std::move(layout)), target(target), parent(parent) {}

  [[nodiscard]] bool isSelected() const {
    return parent && parent->getSelector().contains(target);
  }

  void setSelected(bool selected) {
    if (parent == nullptr) {
      return;
    }
    auto &selector = parent->getSelector();
    if (selected) {
      selector.clear();
      selector.insert(target);
    } else {
      selector.erase(target);
    }
    parent->updateAllFronts();
  }

  void updateSelectionVisual() {
    if (!pointer) {
      return;
    }
    pointer->setVisible(isSelected());
    if (pointer->isVisible()) {
      pic->getRenderer()->setTextStyle(tgui::TextStyle::Italic);
    } else {
      pic->getRenderer()->setTextStyle(tgui::TextStyle::Regular);
    }
  }
};

//player
class InRunPanel {
  RunInfo &runInfo;
  std::string path;
  std::optional<RunInfoFront> playerParametersFront;
  std::shared_ptr<tgui::Panel> mainPanel;
  std::shared_ptr<tgui::Panel> mainFreeSpace;
  std::shared_ptr<tgui::Panel> topFreeSpace;

  void reloadPlayerParametersFront(std::shared_ptr<tgui::Group> const &where) {
    auto curr_hp = where->get<tgui::TextArea>("current_hp");
    auto max_hp = where->get<tgui::TextArea>("max_hp");
    auto curr_err = where->get<tgui::TextArea>("curr_err");
    auto max_err = where->get<tgui::TextArea>("max_err");
    auto coins = where->get<tgui::TextArea>("coins_info_value");
    auto deep = where->get<tgui::TextArea>("deep_info_value");
    auto armour = where->get<tgui::TextArea>("armour_info_value");

    playerParametersFront.emplace(runInfo, curr_hp, max_hp, curr_err, max_err);
  }

 public:
  explicit InRunPanel(RunInfo &runInfo, std::string path = std::string(Assets::FORM_BASE))
      : runInfo(runInfo), path(std::move(path)) {}

  void setRunMenu(std::shared_ptr<tgui::Group> const &where) {
    tgui::Gui tempGui;
    try {
      tempGui.loadWidgetsFromFile(path);
    }
    catch (const tgui::Exception &e) {
      std::cerr << "Failed to load Base for RunMenu: " << e.what() << std::endl;
      throw e;
    }
    mainPanel = tempGui.get<tgui::Panel>("main_panel");
    mainPanel->getRenderer()->setBorderColor(tgui::Color::White);
    mainFreeSpace = tempGui.get<tgui::Panel>("main_free_space");
    topFreeSpace = tempGui.get<tgui::Panel>("top_free_space");
    tempGui.removeAllWidgets();
    mainPanel->setSize("100%", "100%"); //toDo: написать в поддержку tgui, потому что сетсайз забагован
    where->add(mainPanel);

    reloadPlayerParametersFront(where);
  }

  [[nodiscard]] std::shared_ptr<tgui::Panel> getMainFreeSpace() const { return mainFreeSpace; }

  [[nodiscard]] std::shared_ptr<tgui::Panel> getTopFreeSpace() const { return topFreeSpace; }

  std::shared_ptr<tgui::Panel> get() const { return mainPanel; }

  [[nodiscard]] bool hasBeenLoaded() const { return playerParametersFront.has_value(); }

  void clear() {
    playerParametersFront.reset();
    mainFreeSpace = nullptr;
    topFreeSpace = nullptr;
  }
};
//mob
namespace mob {
class MobSelectable;

class MobView : public SelectableFront<Mob, MobSelectable>, SmartSubscriber {
  Mob &mob;
  std::string HPColoring;
  std::string DMGColoring;

  void init();
  void animateAtk() {
    pic->showWithEffect(tgui::ShowEffectType::Scale, 800);
  };
 public:
  MobView(std::shared_ptr<tgui::HorizontalLayout> mobPlace,
          Mob &mob,
          MobSelectable &parent)
      : SelectableFront<Mob, MobSelectable>(std::move(mobPlace), &mob, &parent),
        SmartSubscriber(mob.getBus()),
        mob(mob) {
    init();
  }

  void update_all();
};

class MobSelectable : public Settable {
  std::shared_ptr<tgui::HorizontalLayout> mobPlace;
  std::shared_ptr<tgui::Container> parent;

  FightInfo &fightInfo; //toDo: подумать о константности
  std::list<MobView> fronts;
 public:
  explicit MobSelectable(FightInfo &fightInfo) : fightInfo(fightInfo) {
  }

  void load() override {
    mobPlace = tgui::HorizontalLayout::create();
    mobPlace->setSize({"100%", "50%"});
    mobPlace->setPosition("0%", "10%");
    for (auto &m : fightInfo.mobs) {
      fronts.emplace_back(mobPlace, m, *this);
      mobPlace->add(fronts.back().getTmpl());
    }
    setMain(mobPlace);
    connect();
  }

  std::set<Mob *> &getSelector() { return fightInfo.currentlyUnderAttack; }

  void updateAllFronts() {
    for (auto &front : fronts) {
      front.update_all();
    }
  }

  [[nodiscard]] const FightInfo &getFightInfo() const {
    return fightInfo;
  }
};

inline void MobView::init() {
  loadTemplateWidgets(Assets::FORM_MOB_BASE, std::format("mob{}", mob.getUniqueId()));
  update_all();
  pic->onClick([this] {
    setSelected(true);
  });
  subscribeSmartly<mob::MobAttacking>(
      [this](const mob::MobAttacking &msg) {
        if (mob.getUniqueId() == msg.target) {
          animateAtk();
        }
      }
  );
  subscribeSmartly<mob::MobChangedMsg>([this](const mob::MobChangedMsg &msg) {
    if (msg.target == mob.getUniqueId()) {
      switch (msg.val) {
        case State::HP_LESSENED:HPColoring = "RED";
          break;
        case State::HP_GREW:HPColoring = "GREEN";
          break;
      }
      update_all();
    }
  });
}

inline void MobView::update_all() {
  name->setText(mob.getType().getName());
  pic->setText(mob.getType().getImage());
  std::string HPColor = HPColoring.empty() ? "white" : HPColoring;
  std::string DMGColor = DMGColoring.empty() ? "white" : DMGColoring;
  tgui::String dsc;
  dsc = std::format("<color={}>{}/{}</color><color={}>DMG: {}</color>",
                    HPColor, mob.getCurrentHp(), mob.getType().getMaxHp(),
                    DMGColor, mob.getType().getAttackPower());
  info->setText(dsc);
  updateSelectionVisual();
}
}  // namespace mob

class Layout {
  GameContext &context;
  tgui::Gui &gui = context.gui;
  std::shared_ptr<tgui::Group> up;
  std::shared_ptr<tgui::Group> middle;
  std::shared_ptr<tgui::Group> down;
  InRunPanel player_menu_panel_;
  std::unique_ptr<Settable> middleView;
 public:
  Layout(GameContext &context, RunInfo &runInfo, std::string path = std::string(Assets::FORM_BASE))
      : context(context), player_menu_panel_(runInfo, std::move(path)), runInfo(runInfo) {
    init();
  }

  tgui::Gui &getGui() { return context.gui; }

  std::shared_ptr<tgui::Panel> getMainFreeSpace() { return player_menu_panel_.getMainFreeSpace(); }

  std::shared_ptr<tgui::Panel> getTopFreeSpace() { return player_menu_panel_.getTopFreeSpace(); }

  GameContext &getContext() { return context; }

  void setMiddle(std::unique_ptr<Settable> view) {
    middleView = std::move(view);
    middleView->setParent(middle);
  }

  player::Player &getPlayer() { return runInfo.player; }

  [[nodiscard]] bool hasBeenLoaded() const { return player_menu_panel_.hasBeenLoaded(); }

  std::shared_ptr<tgui::Panel> get() { return player_menu_panel_.get(); }

  void load() {
    gui.removeAllWidgets();
    init();
    if (middleView) {
      middleView->setParent(middle);
      middleView->load();
    }
  }

  void init() {
    up = tgui::Group::create();
    middle = tgui::Group::create();
    down = tgui::Group::create();
    gui.add(up);
    gui.add(middle);
    gui.add(down);
    up->setSize("100%", "10%");
    up->setPosition({"0%", "0%"});
    middle->setSize("100%", "60%");
    middle->setPosition({"0%", "10%"});
    down->setSize("100%", "30%");
    down->setPosition({"0%", "70%"});
    player_menu_panel_.setRunMenu(down);
  }

  void clear() {
    gui.removeAllWidgets();
    up.reset();
    down.reset();
    middle.reset();
    player_menu_panel_.clear();
  }
  RunInfo &runInfo;
};


