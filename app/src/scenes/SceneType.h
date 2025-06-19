
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
  WON_BATTLE,
  SCENE,
  CROSSROAD,
  INVENTORY,
};
enum class InFight {
  Inactive,
  BattleMenu,
  Atk,
  Defend
};
}  // namespace State
