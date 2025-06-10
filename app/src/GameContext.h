#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "world_player_and_container/Player.h"
#include "world_player_and_container/Mob.h"

static const int DEFAULT_TIME = 10;
static const int DEFAULT_VIS_LETTERS = 10;
static const int DEFAULT_HP = 10;
static const int DEFAULT_ERRORS = 10;

static const char *const DEFAULT_TXT = "assets/texts/test_ru.txt";

static const int DEFAULT_LENGTH = 300;
struct GameContext {
  tgui::Gui gui;
  QueuedMessageBus &queueBus = QueuedMessageBus::getInstance();
  MobDatabase mobDatabase;
  sf::RenderWindow &window;

  player::PlayerContext playerContext{DEFAULT_TIME, DEFAULT_VIS_LETTERS,
                                      {DEFAULT_HP, DEFAULT_HP},
                                      {DEFAULT_ERRORS, DEFAULT_ERRORS}};

  explicit GameContext(sf::RenderWindow &window) :
      gui(window),
      window(window) {
    mobDatabase.loadFromJson("assets/gameResources/mobs/cat.json");
    mobDatabase.loadFromJson("assets/gameResources/mobs/frog.json");

  }
};

struct RunInfo {
 private:
  FileTextSource fs{DEFAULT_TXT, DEFAULT_LENGTH};
  GenerativeSource gs{};
 public:
  ItemManager itemManager;
  TextHolder th;
  player::Player player;
  int lvl = 0;
  //std::map <string, TextHolder> holders;

  explicit RunInfo(GameContext &gameContext)
      : th(200, "", 0, fs, true),
        player(gameContext.queueBus, th, gameContext.playerContext) {
    itemManager.addItem({"InkBottle", "Ink Bottle"});
    itemManager.addItem({"Eraser", "Eraser"});
  }
};

struct FightInfo {
  std::vector<Mob> mobs;
  std::set<Mob *> currentlyUnderAttack;

};