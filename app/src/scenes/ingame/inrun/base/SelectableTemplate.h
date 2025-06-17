#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <set>
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/HorizontalLayout.hpp>

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