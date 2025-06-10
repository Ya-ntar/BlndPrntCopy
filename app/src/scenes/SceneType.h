
#pragma once

namespace State {
enum class Menu {
  MainMenu,
  StartGame,
  Credits,
};

enum class InGame {
  Inactive,
  Base,
  InRun,
  Death
};

enum class InRun {
  Inactive,
  Scene,
  Crossroad,
  Inventory,
};
enum class InFight {
  Inactive,
  BattleMenu,
  Atk,
  Defend
};
}  // namespace State
