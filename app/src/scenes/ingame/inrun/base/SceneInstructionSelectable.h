#pragma once

#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <list>
#include "../../../../scenes/Scene.h"
#include "../SceneGenerator.h"
#include "SceneInstructions.h"

class SceneInstructionSelectable;

class SceneInstructionView : public TemplateFront {
  SceneInstruction &instruction;
  SceneInstructionSelectable *parent = nullptr;
 public:
  SceneInstructionView(const std::shared_ptr<tgui::HorizontalLayout> &layout,
                       SceneInstruction &instruction,
                       SceneInstructionSelectable *parent)
      : instruction(instruction), parent(parent) {
    loadTemplateWidgets("forms/standard/fighting/MobBase.txt", "Tmpl" +
        std::to_string(instruction.uniqueId));
    update_all();
    pic->onClick([this] {
      setSelected(true);
    });
  }

  [[nodiscard]] bool isSelected() const;

  void setSelected(bool selected);

  void update_all();
};

class SceneInstructionSelectable : public Settable {
  std::shared_ptr<tgui::HorizontalLayout> instructionPlace;
  std::vector<SceneInstruction> &instructions;
  std::set<SceneInstruction *> &selectedInstructions;
  std::list<SceneInstructionView> fronts;
 public:
  explicit SceneInstructionSelectable(std::vector<SceneInstruction> &instructions,
                                      std::set<SceneInstruction *> &selectedInstructions)
      : instructions(instructions), selectedInstructions(selectedInstructions) {}

  void load() override {
    instructionPlace = tgui::HorizontalLayout::create();
    instructionPlace->setSize({"100%", "50%"});
    instructionPlace->setPosition("0%", "10%");
    for (auto &instr : instructions) {
      fronts.emplace_back(instructionPlace, instr, this);
      instructionPlace->add(fronts.back().getTmpl());
    }
    setMain(instructionPlace);
    connect();
  }

  std::set<SceneInstruction *> &getSelector() { return selectedInstructions; }

  void updateAllFronts() {
    for (auto &front : fronts) {
      front.update_all();
    }
  }

  [[nodiscard]] const std::vector<SceneInstruction> &getInstructions() const {
    return instructions;
  }
};

inline bool SceneInstructionView::isSelected() const {
  return (parent != nullptr) && parent->getSelector().contains(&instruction);
}

inline void SceneInstructionView::setSelected(bool selected) {
  if (parent == nullptr) {
    return;
  }
  auto &selector = parent->getSelector();
  if (selected) {
    selector.clear();
    selector.insert(&instruction);
  } else {
    selector.erase(&instruction);
  }
  parent->updateAllFronts();
}

inline void SceneInstructionView::update_all() {
  if (instruction.type == SceneInstruction::Type::FIGHT) {
    name->setText("Type: Fight");
  }

  pic->setText("Where: " + instruction.where);
  std::string mobsStr = "Mobs: ";
  for (const auto &mob : instruction.mobs) {
    mobsStr += mob + ", ";
  }
  if (!instruction.mobs.empty()) {
    mobsStr.pop_back(), mobsStr.pop_back();
  }
  info->setText(mobsStr);
  if (pointer) {
    pointer->setVisible(isSelected());
  }
} 