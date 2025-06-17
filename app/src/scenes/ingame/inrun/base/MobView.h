#pragma once
#include <memory>
#include <string>
#include <map>
#include <set>
#include <format>
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/HorizontalLayout.hpp>

#include "SelectableTemplate.h"
#include "../../../../scenes/Scene.h"

namespace mob {
class MobSelectable;

class MobView : public SelectableFront<Mob, MobSelectable>, SmartSubscriber {
  Mob &mob;
  std::string HPColoring;
  std::string DMGColoring;

  void init();
  void animateAtk() {
    pic->showWithEffect(tgui::ShowEffectType::Scale, 5000);
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

  FightInfo &fightInfo;
  std::map<MobID, MobView> fronts;
 public:
  explicit MobSelectable(FightInfo &fightInfo) : fightInfo(fightInfo) {
  }

  void load() override {
    mobPlace = tgui::HorizontalLayout::create();
    mobPlace->setSize({"100%", "50%"});
    mobPlace->setPosition("0%", "10%");
    for (auto &m : fightInfo.mobs) {
      MobID mobId = m.second.getUniqueId();
      fronts.try_emplace(mobId, mobPlace, m.second, *this);
      mobPlace->add(fronts.at(mobId).getTmpl());
    }
    setMain(mobPlace);
    connect();
  }

  std::set<Mob *> &getSelector() { return fightInfo.currentlyUnderAttack; }

  void updateAllFronts() {
    for (auto &front : fronts) {
      front.second.update_all();
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