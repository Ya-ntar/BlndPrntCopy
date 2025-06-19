#pragma once

#include "scenes/MainManager.h"
#include "GameContext.h"
#include <SFML/Audio.hpp>
#include "utils/Utils.h"
#include "utils/ErrorHandler.h"

static const int WINDOW_SIZE = 800;

class Game {
  sf::Image icon;
  sf::RenderWindow window{sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE}),
                          "Blind Print", sf::Style::Default};
  GameContext context{window};

 public:
  Game() {
    if (icon.loadFromFile(Assets::ICON)) {
      window.setIcon(icon);
    }
    context.gui.setFont(tgui::Font(Assets::kFontConsolas));
  }

  int run() {
    SoundGetter sound_getter;
    context.loadMobs();
    MainManager main_manager(context);
    main_manager.load();

    while (window.isOpen()) {
      try {
        main_manager.processDeferred();
        while (const std::optional _event = window.pollEvent()) {
          if (_event->is<sf::Event::Closed>()) {
            window.close();
          }
          if (_event->is<sf::Event::KeyPressed>()) {
            sound_getter.play();
          }
          main_manager.handleEvent(_event.value());
          context.gui.handleEvent(_event.value());
        }
        context.queueBus.processLast();
        window.clear();

        context.gui.draw();
        window.display();
      }
      catch (const std::exception &e) {
        error::logError(e.what());
        return 1;
      }
    }
    return 0;
  }
};



