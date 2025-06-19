#include "src/Game.h"
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include "src/utils/ErrorHandler.h"

void testBookMenuScenario() {
  // Create window and GUI
  sf::RenderWindow window{sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE}),
                          "Blind Print", sf::Style::Default};

  GameContext context(window);
  RunInfo runInfo(context);
  auto &gui = context.gui;
  // Create layout and manager
  Layout layout(context, runInfo);
  InRunManager manager(nullptr, context);

  // Create a temporary group for the book menu
  auto group = tgui::Group::create();
  gui.add(group);

  // Test multiple creation/destruction cycles
  for (int i = 0; i < 5; i++) {
    std::cout << "BookMenu test iteration " << i << std::endl;
    manager.showBookMenu();
    sf::sleep(sf::milliseconds(100));
    manager.processDeferred();

    manager.hideBookMenu();
    manager.processDeferred();
    std::cout << "BookMenu iteration " << i << " completed" << std::endl;
  }
}

void stupidTest() {
  std::cout << "Starting stupid test..." << std::endl;

  // Create window and GUI
  sf::RenderWindow window{sf::VideoMode({800, 600}), "Stupid Test", sf::Style::Default};
  GameContext context(window);
  RunInfo runInfo(context);

  auto group = tgui::Group::create();
  context.gui.add(group);

  auto curr_hp = tgui::TextArea::create();
  auto max_hp = tgui::TextArea::create();
  auto curr_err = tgui::TextArea::create();
  auto max_err = tgui::TextArea::create();
  auto coins = tgui::TextArea::create();
  auto level = tgui::TextArea::create();
  auto armour = tgui::TextArea::create();

  group->add(curr_hp);
  group->add(max_hp);
  group->add(curr_err);
  group->add(max_err);
  group->add(coins);
  group->add(level);
  group->add(armour);

  std::cout << "Created all widgets" << std::endl;

  auto front = std::make_shared<RunInfoFront>(
      runInfo,
      curr_hp,
      max_hp,
      curr_err,
      max_err,
      coins,
      armour,
      level
  );

  for (int i = 0; i < 5; i++) {
    std::cout << "Test iteration " << i << std::endl;
    auto curr_hp = tgui::TextArea::create();
    auto max_hp = tgui::TextArea::create();
    auto curr_err = tgui::TextArea::create();
    auto max_err = tgui::TextArea::create();
    auto coins = tgui::TextArea::create();
    auto level = tgui::TextArea::create();
    auto armour = tgui::TextArea::create();
    auto front = std::make_shared<RunInfoFront>(
        runInfo,
        curr_hp,
        max_hp,
        curr_err,
        max_err,
        coins,
        armour,
        level
    );

    std::cout << "Created RunInfoFront" << std::endl;

    sf::sleep(sf::milliseconds(100));

    std::cout << "Destroying RunInfoFront" << std::endl;
  }

  std::cout << "Test completed" << std::endl;
}

int main() {
/*
  try {
    std::cout << "Running stupid test..." << std::endl;
    stupidTest();
  }
  catch (const std::exception &e) {
    error::logError(e.what());
    return 1;
  }
*/

  /*try {
    std::cout << "\nRunning book menu test..." << std::endl;
    testBookMenuScenario();
  }
  catch (const std::exception &e) {
    error::logError(e.what());
    return 1;
  }*/
   Game game;
   game.run();
  return 0;
}



