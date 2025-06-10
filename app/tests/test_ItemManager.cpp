#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <map>
#include "../src/world_player_and_container/ItemManager.h"
#include "../src/world_player_and_container/Item.h"

class TestItem {
  std::string identity;
  std::string name;
 public:
  TestItem(std::string id, std::string n) : identity(std::move(id)), name(std::move(n)) {}
  bool operator==(const TestItem &other) const { return identity == other.identity && name == other.name; }
  bool operator!=(const TestItem &other) const { return !(*this == other); }
  bool operator<(const TestItem &other) const {
    return (identity < other.identity) || (identity == other.identity && name < other.name);
  }
  [[nodiscard]] std::string getIdentity() const { return identity; }
  [[nodiscard]] std::string getName() const { return name; }
};

class DummyItemAffected {
 public:
  mutable bool used = false;
  bool useItem(const TestItem &item) const {
    used = true;
    return true;
  }
};

class DummyAffected : public ItemAffected {
 public:
  mutable bool used = false;
  DummyAffected() {
    addReaction("test_id", [this](const Item &) { used = true; }, "desc");
  }
};

class TestItemManager {
  std::map<TestItem, size_t> items;
 public:
  [[nodiscard]] const std::map<TestItem, size_t> &getItemMap() const { return items; }
  void addItem(const TestItem &item) {
    if (static_cast<unsigned int>(items.contains(item)) != 0U) { items[item]++; }
    else {
      items[item] = 1;
    }
  }
  void useItem(DummyItemAffected &context, const TestItem &whatToUse) {
    if (static_cast<unsigned int>(items.contains(whatToUse)) == 0U) {
      throw std::runtime_error("No item");
    }
    if (context.useItem(whatToUse)) {
      items[whatToUse]--;
      if (items[whatToUse] == 0) {
        items.erase(whatToUse);
      }
    }
  }
};

TEST_CASE("TestItemManager basic add and getItemMap", "[ItemManager]") {
  TestItemManager manager;
  TestItem item1{"id1", "Sword"};
  REQUIRE(manager.getItemMap().count(item1) == 0);
  manager.addItem(item1);
  REQUIRE(manager.getItemMap().count(item1) == 1);
}

TEST_CASE("TestItemManager addItem increments count", "[ItemManager]") {
  TestItemManager manager;
  TestItem item1{"id1", "Sword"};
  manager.addItem(item1);
  manager.addItem(item1);
  REQUIRE(manager.getItemMap().at(item1) == 2);
}

TEST_CASE("TestItemManager takeItem via useItem", "[ItemManager]") {
  TestItemManager manager;
  TestItem item1{"id1", "Sword"};
  DummyItemAffected context;
  manager.addItem(item1);
  manager.addItem(item1);
  manager.useItem(context, item1);
  REQUIRE(manager.getItemMap().at(item1) == 1);
  manager.useItem(context, item1);
  REQUIRE(manager.getItemMap().count(item1) == 0);
}

TEST_CASE("TestItemManager removeAllItems via addItem and getItemMap", "[ItemManager]") {
  TestItemManager manager;
  TestItem item1{"id1", "Sword"};
  manager.addItem(item1);
  manager.addItem(item1);
  DummyItemAffected context;
  manager.useItem(context, item1);
  manager.useItem(context, item1);
  REQUIRE(manager.getItemMap().count(item1) == 0);
}

TEST_CASE("TestItemManager useItem calls context and removes item", "[ItemManager]") {
  TestItemManager manager;
  TestItem item1{"id1", "Sword"};
  DummyItemAffected context;
  manager.addItem(item1);
  REQUIRE_NOTHROW(manager.useItem(context, item1));
  REQUIRE(context.used);
  REQUIRE(manager.getItemMap().count(item1) == 0);
}

TEST_CASE("TestItemManager useItem throws if item not present", "[ItemManager]") {
  TestItemManager manager;
  TestItem item1{"id1", "Sword"};
  DummyItemAffected context;
  REQUIRE_THROWS(manager.useItem(context, item1));
}

TEST_CASE("ItemManager contains and count", "[ItemManager]") {
  ItemManager manager;
  Item item{"test_id", "TestName"};
  REQUIRE_FALSE(manager.contains(item));
  manager.addItem(item);
  REQUIRE(manager.contains(item));
  REQUIRE(manager.getItemMap().at(item) == 1);
  // getItemCount возвращает count, а не количество
  REQUIRE(manager.count(item) == 1);
}

TEST_CASE("ItemManager removeAllItems", "[ItemManager]") {
  ItemManager manager;
  Item item{"test_id", "TestName"};
  manager.addItem(item);
  manager.addItem(item);
  REQUIRE(manager.getItemMap().at(item) == 2);
  manager.removeAllItems(item);
  REQUIRE(manager.getItemMap().count(item) == 0);
}

TEST_CASE("ItemManager takeItem", "[ItemManager]") {
  ItemManager manager;
  Item item{"test_id", "TestName"};
  manager.addItem(item);
  manager.addItem(item);
  REQUIRE(manager.takeItem(item));
  REQUIRE(manager.getItemMap().at(item) == 1);
  REQUIRE(manager.takeItem(item));
  REQUIRE(manager.getItemMap().count(item) == 0);
  REQUIRE_FALSE(manager.takeItem(item));
}

TEST_CASE("ItemManager multiple items", "[ItemManager]") {
  ItemManager manager;
  Item item1{"id1", "Sword"};
  Item item2{"id2", "Shield"};
  manager.addItem(item1);
  manager.addItem(item2);
  manager.addItem(item1);
  REQUIRE(manager.getItemMap().at(item1) == 2);
  REQUIRE(manager.getItemMap().at(item2) == 1);
  manager.removeAllItems(item1);
  REQUIRE(manager.getItemMap().count(item1) == 0);
  REQUIRE(manager.getItemMap().at(item2) == 1);
}

TEST_CASE("ItemManager useItem with reaction", "[ItemManager]") {
  ItemManager manager;
  Item item{"test_id", "TestName"};
  DummyAffected context;
  manager.addItem(item);
  REQUIRE_NOTHROW(manager.useItem(context, item));
  REQUIRE(context.used);
  REQUIRE(manager.getItemMap().count(item) == 0);
}

TEST_CASE("ItemManager useItem throws if item not present (real)", "[ItemManager]") {
  ItemManager manager;
  Item item{"test_id", "TestName"};
  DummyAffected context;
  REQUIRE_THROWS(manager.useItem(context, item));
}
