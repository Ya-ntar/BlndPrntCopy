#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "world_player_and_container/Player.h"
#include "world_player_and_container/Mob.h"
#include <map>
#include <utility>
#include "world_player_and_container/Book.h"
#include <functional>

static const int DEFAULT_TIME = 1000;
static const int DEFAULT_VIS_LETTERS = 10;
static const int DEFAULT_HP = 10;
static const int DEFAULT_ERRORS = 10;

static const int DEFAULT_LENGTH = 300;
static const int DEFAULT_BUFFER_SIZE = 200;

struct BookResource {
  std::string name;
  std::string textPath;
  int lvl;
  int damage_per_hit;
  int extra_time;
  BoundedValue durability;
};

struct GameContext {
  tgui::Gui gui;
  QueuedMessageBus &queueBus = QueuedMessageBus::getInstance();
  MobDatabase mobDatabase;
  sf::RenderWindow &window;
  player::PlayerContext playerContext{DEFAULT_TIME, DEFAULT_VIS_LETTERS,
                                      {DEFAULT_HP, DEFAULT_HP},
                                      {DEFAULT_ERRORS, DEFAULT_ERRORS}};
  std::map<int, BookResource> bookResources;

  explicit GameContext(sf::RenderWindow &window) :
      gui(window),
      window(window) {}

  void loadMobs() {
    mobDatabase.loadFromJson(Assets::MOB_CAT_JSON);
    mobDatabase.loadFromJson(Assets::MOB_FROG_JSON);
  }

  void addBookResource(int id, const BookResource &resource) {
    bookResources.try_emplace(id, resource);
  }
  const BookResource &getBookResource(int id) const {
    return bookResources.at(id);
  }
};

struct RunInfo {
 private:
  FileTextSource fs{std::string(Assets::TEXT_TEST_RU), DEFAULT_LENGTH};
  int coins = 0;
  int lvl = 0;
  ItemManager itemManager;
  std::map<size_t, Book> books;
 public:
  std::reference_wrapper<Book> currentBook;
  player::Player player;

  explicit RunInfo(GameContext &gameContext)
      : currentBook(createBaseBook()),
        player(gameContext.queueBus, gameContext.playerContext) {
    itemManager.addItem({"InkBottle", "Ink Bottle"});
    itemManager.addItem({"Eraser", "Eraser"});

    // Добавляем тестовые ресурсы книг
    gameContext.addBookResource(1, {"Test Book 1", "assets/texts/test_book1.txt",
                                    1, 5, 0, BoundedValue(20, 20)});
    const auto &res1 = gameContext.getBookResource(1);
    FileTextSource src2(res1.textPath, DEFAULT_LENGTH);
    addBook(res1, std::make_unique<FileTextSource>(std::move(src2)));
  }

  Book &createBaseBook() {
    auto textSource = std::make_unique<GenerativeSource>();
    books.try_emplace(
        0,
        "Empty hands",
        1,
        0,
        0,
        BoundedValue(10, 10),
        std::move(textSource),
        DEFAULT_BUFFER_SIZE,
        true,
        true
    );
    return books.at(0);
  }
  void addBook(const BookResource &resource, std::unique_ptr<TextSource> textSource) {
    Book book{resource.name,
              resource.lvl,
              resource.damage_per_hit,
              resource.extra_time,
              resource.durability,
              std::move(textSource),  // передаем владение
              DEFAULT_BUFFER_SIZE,
              true};
    books.try_emplace(
        book.getId(),
        std::move(book)
    );
  }

  const std::map<size_t, Book> &getBooks() const { return books; }
  Book &getBookById(int id) { return books.at(id); }

  const ItemManager &getManager() const { return itemManager; }
  ItemManager &getManager() { return itemManager; }
  Book &getCurrentBook() const { return currentBook.get(); }
  int getCurrentCoins() const { return coins; }
  int getCurrentLvl() const { return lvl; }
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
  std::map<MobID, Mob> mobs;
  std::set<Mob *> currentlyUnderAttack;

  void addMob(Mob &&mob) { //!!! Eats
    auto id = mob.getUniqueId();
    mobs.try_emplace(mob.getUniqueId(), std::move(mob));
    if (currentlyUnderAttack.empty()) {
      currentlyUnderAttack.emplace(&mobs.at(id));
    }

  }

  void removeMob(MobID mobId) {
    auto mobIt = mobs.find(mobId);
    if (mobIt == mobs.end()) {
      throw std::invalid_argument("Mob with given ID does not exist");
    }
    
    Mob* mobPtr = &mobIt->second;
    

    auto attackIt = currentlyUnderAttack.find(mobPtr);
    if (attackIt != currentlyUnderAttack.end()) {
      currentlyUnderAttack.erase(attackIt);
      

      if (currentlyUnderAttack.empty() && mobs.size() > 1) {
        for (auto& [id, mob] : mobs) {
          if (id != mobId) {
            currentlyUnderAttack.emplace(&mob);
            break;
          }
        }
      }
    }
    

    mobs.erase(mobIt);
  }

};