#pragma once
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Container.hpp>
#include <type_traits>
#include "../../../../utils/WidgetConfigurator.h"

struct InventoryButtonUtil {
  template<typename ParentType, typename AffectedType = void>
  static void addInventoryButton(
      const std::shared_ptr<tgui::Container> &layout,
      ParentType *parent,
      AffectedType *affected = nullptr
  ) {
    auto inventory = createStyledButton("Inventory");
    inventory->onPress([parent, affected] {
      if constexpr (std::is_same_v<AffectedType, void>) {
        parent->showInventory();
      } else {
        parent->showInventory(*affected);
      }
    });
    layout->add(inventory);
  }

  static std::shared_ptr<tgui::Button> createStyledButton(const std::string &text) {
    auto button = tgui::Button::create(text);
    button->setSize("150", "30");
    WidgetConfigurator::configureButton(button);
    return button;
  }
};

struct BookButtonUtil {
  template<typename ParentType, typename AffectedType = void>
  static void addBookButton(
      const std::shared_ptr<tgui::Container> &layout,
      ParentType *parent) {
    auto bookButton = InventoryButtonUtil::createStyledButton("Book");
    bookButton->onPress([parent] {
      parent->showBookMenu();
    });
    layout->add(bookButton);
  }
}; 