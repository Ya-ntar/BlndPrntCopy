#pragma once

#include "PubBus/Message.hpp"
#include "../text_processing/backEnd/tools/TextHolder.h"

using MobID = const std::size_t;

// Input Msg


struct KeySentMsg : pub::Message {
  const string key;
};

// Scene Management
struct SceneMessage : pub::Message {
  std::string changeToScene;
};


// Inner Game Msg

namespace player {
enum class State {
  HP_LESSENED,
  HP_GREW,
  ERRORS_LESSENED,
  ERRORS_GREW,
  STABILISE
};

struct PlayerChangedMsg : pub::Message {
  const State val;
};

struct PlayerDeath : pub::Message {
};
}  // namespace player
namespace mob {
enum class State {
  HP_LESSENED,
  HP_GREW,
  DIED
};
struct MobAttacking : pub::Message {
  const int32_t damage;
  const MobID target;
};

struct MobChangedMsg : pub::Message {
  const State val;
  const MobID target;
};
}  // namespace mob

namespace fight {
enum class State {
  TARGET_CHANGE
};
struct FightChange : pub::Message {
  const State val;
};
struct AtkEnded : pub::Message {
  int32_t hits;
};
}  // namespace fight
// technical

namespace textProcessing {
enum class CharCheckState {
  CORRECT_INPUT,
  INCORRECT_INPUT,
  NOTHING
};

struct TextProcessStateMsg : pub::Message {
  const CharCheckState val;
};

struct DefenseChangeStateMsg : pub::Message {
  const size_t player_ptr;
  const size_t mob_ptr;
  const string str;

};
struct AttackLogicStateChange : pub::Message {
  textProcessing::CharCheckState state;
  int32_t hits{};
};

}  // namespace textProcessing



// Output Msg


// Tool Msg

struct TimerMsg : pub::Message {
  std::string timerType;
  float timeLeft;
  bool is_finished;
};