#pragma once
#include <algorithm>
#include <random>
#include <vector>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "AssetPaths.h"
class SoundGetter {
  std::vector<sf::SoundBuffer> soundBuffs;
  std::vector<sf::Sound> sounds;
  std::random_device rd;  //не лишняя ли трата?

  void update_sounds() {
    for (auto &sound : soundBuffs) {
      sounds.emplace_back(sound);
    }
  }
  void loadSound(std::string_view filename) {
    sf::SoundBuffer sound_buffer;
    if (!(sound_buffer.loadFromFile(filename))) {
      throw std::invalid_argument("Unable to read sound from a file " + std::string(filename));
    }
    soundBuffs.push_back(sound_buffer);
  }

 public:
  SoundGetter() {
    loadSound(Assets::kSoundClickAlt);
    update_sounds();
  }

  void play() {
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sounds.size() - 1);
    sounds[dis(gen)].play();
  }
};
