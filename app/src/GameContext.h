#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "world_player_and_container/Player.h"
#include "world_player_and_container/Mob.h"
#include <map>
#include <utility>
#include "world_player_and_container/Book.h"
#include <functional>

static const int DEFAULT_TIME = 10;
static const int DEFAULT_VIS_LETTERS = 10;
static const int DEFAULT_HP = 10;
static const int DEFAULT_ERRORS = 10;

static const int DEFAULT_LENGTH = 300;
static const int DEFAULT_BUFFER_SIZE = 200;
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
    mobDatabase.loadFromJson(Assets::MOB_CAT_JSON);
    mobDatabase.loadFromJson(Assets::MOB_FROG_JSON);

  }
};

struct RunInfo {
 private:
  FileTextSource fs{std::string(Assets::TEXT_TEST_RU), DEFAULT_LENGTH};
  GenerativeSource gs{};
 public:
  ItemManager itemManager;
  std::map<std::size_t, Book> books;
  std::reference_wrapper<Book> currentBook;
  player::Player player;
  int lvl = 0;

  explicit RunInfo(GameContext &gameContext)
      : currentBook(createBaseBook()),
        player(gameContext.queueBus, gameContext.playerContext) {

    itemManager.addItem({"InkBottle", "Ink Bottle"});
    itemManager.addItem({"Eraser", "Eraser"});
  }

  Book &createBaseBook() {
    // Always create the base book with id 0 and GenerativeSource
    books.try_emplace(
        0,
        "Empty hands", 1, 0, 0, BoundedValue(10, 10), gs, DEFAULT_BUFFER_SIZE, true, true
    );
    return books.at(0);
  }

  Book &getOrCreateDefaultBook(GameContext const &gameContext) {
    // For compatibility: always return the base book
    return books.at(0);
  }

  Book &getCurrentBook() const {
    return currentBook.get();
  }

  void setCurrentBook(std::size_t id) {
    auto it = books.find(id);
    if (it != books.end()) {
      currentBook = std::ref(it->second);
    } else {
      throw std::invalid_argument("Book with given ID does not exist");
    }
  }

  auto &getTextHolder() const { return getCurrentBook().getTextHolder(); }
  tgui::String getActiveChar() const { return getTextHolder().getFirstChar(); }
  tgui::String getMainString() const {
    auto &th = getTextHolder();
    if (getHowManyVisibleLetters() > th.maxSize()) {
      return "";
    }
    auto txt = th.currentText(getHowManyVisibleLetters());
    if (txt.size() <= 1) {
      return "";
    }
    return txt.substr(1);
  }
  size_t getHowManyVisibleLetters() const { return player.getHowManyVisibleLetters(); }
};

struct FightInfo {
  std::vector<Mob> mobs;
  std::set<Mob *> currentlyUnderAttack;

};