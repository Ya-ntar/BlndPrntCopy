#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "world_player_and_container/Player.h"
#include "world_player_and_container/Mob.h"
#include <map>
#include <utility>
#include "world_player_and_container/Book.h"
#include <functional>
#include <experimental/filesystem>

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
  MobDatabase mob_database;
  sf::RenderWindow &window;
  player::PlayerContext player_context{DEFAULT_TIME, DEFAULT_VIS_LETTERS,
                                       {DEFAULT_HP, DEFAULT_HP},
                                       {DEFAULT_ERRORS, DEFAULT_ERRORS}};
  std::map<int, BookResource> book_resources;

  explicit GameContext(sf::RenderWindow &window) :
      gui(window),
      window(window) {}

  void loadMobs() {
    namespace fs = std::experimental::filesystem;
    std::string mob_dir = "assets/gameResources/mobs/";
    for (const auto &entry : fs::directory_iterator(mob_dir)) {
      if (entry.path().extension() == ".json") {
        mob_database.loadFromJson(entry.path().string());
      }
    }
  }

  void addBookResource(int id, const BookResource &resource) {
    book_resources.try_emplace(id, resource);
  }
  const BookResource &getBookResource(int id) const {
    return book_resources.at(id);
  }
};

struct RunInfo {
 private:
  FileTextSource fs{std::string(Assets::kTextTestRu), DEFAULT_LENGTH};
  size_t coins = 0;
  int lvl = 0;
  string log;
  ItemManager item_manager;
  std::map<size_t, Book> books;
 public:
  std::reference_wrapper<Book> current_book;
  player::Player player;

  void levelUp() {
    lvl++;
  }
  void takeCoins(size_t hm) {
    coins -= hm;
  }
  void addCoins(size_t hm) {
    coins += hm;
  }
  void setLog(string new_log) {
    log = std::move(new_log);
  }
  explicit RunInfo(GameContext &gameContext)
      : current_book(createBaseBook()),
        player(gameContext.queueBus, gameContext.player_context) {
    item_manager.addItem({"InkBottle", "Ink Bottle"});
    item_manager.addItem({"Eraser", "Eraser"});

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

  const ItemManager &getManager() const { return item_manager; }
  ItemManager &getManager() { return item_manager; }
  Book &getCurrentBook() const { return current_book.get(); }
  int getCurrentCoins() const { return coins; }
  int getCurrentLvl() const { return lvl; }
  void setCurrentBook(std::size_t id) {
    auto it = books.find(id);
    if (it != books.end()) {
      current_book = std::ref(it->second);
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
  string getLog() {
    return log;
  }
};

struct FightInfo {
  std::map<MobID, Mob> mobs;
  std::set<Mob *> currently_under_attack;
  int totalCoins = 0;

  void addMob(Mob &&mob) { //!!! Eats
    auto id = mob.getUniqueId();
    totalCoins += mob.getType().getCoins();
    mobs.try_emplace(mob.getUniqueId(), std::move(mob));
    if (currently_under_attack.empty()) {
      currently_under_attack.emplace(&mobs.at(id));
    }

  }

  void removeMob(MobID mobId) {
    auto mob_it = mobs.find(mobId);
    if (mob_it == mobs.end()) {
      throw std::invalid_argument("Mob with given ID does not exist");
    }

    Mob *mobPtr = &mob_it->second;

    auto attack_it = currently_under_attack.find(mobPtr);
    if (attack_it != currently_under_attack.end()) {
      currently_under_attack.erase(attack_it);

      if (currently_under_attack.empty() && mobs.size() > 1) {
        for (auto &[id, mob] : mobs) {
          if (id != mobId) {
            currently_under_attack.emplace(&mob);
            break;
          }
        }
      }
    }

    mobs.erase(mob_it);
  }

};