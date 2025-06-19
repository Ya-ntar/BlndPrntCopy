#pragma once

#include <memory>
#include <string>
#include <vector>
#include <random>
#include "action/fight/InFightManager.h"
#include "base/SceneInstructions.h"

class SceneGenerator {
 private:
  InRunManager *parent;
  GameContext &context;
  RunInfo &runInfo;
  Layout &menuBase;
 public:
  [[nodiscard]] std::vector<SceneInstruction> generateInstructions() const {
    std::vector<SceneInstruction> result;
    const auto &mobTypes = context.mob_database.getMobTypes();
    std::vector<ItemID> id_vec;
    id_vec.reserve(mobTypes.size());
    for (const auto &type : mobTypes) {
      id_vec.emplace_back(type.first);
    }

    if (mobTypes.empty()) {
      return result;
    }

    std::random_device random_device;
    std::mt19937 gen{random_device()};
    std::uniform_int_distribution mobCountDist(1, 4);
    std::uniform_int_distribution mobIndexDist(0, static_cast<int>(mobTypes.size()) - 1);

    for (int i = 0; i < 3; ++i) {
      int mobCount = mobCountDist(gen);
      std::vector<std::string> mobs;
      for (int j = 0; j < mobCount; ++j) {
        mobs.push_back(id_vec[mobIndexDist(gen)]);
      }
      result.emplace_back(SceneInstruction::Type::FIGHT, mobs,
                          "random_location" + std::to_string(i + 1));
    }
    return result;
  }

  std::unique_ptr<InFightManager> generateScene(const SceneInstruction &instruction) {
    if (instruction.type == SceneInstruction::Type::FIGHT) {
      FightInfo fightInfo;
      for (const auto &mobName : instruction.mobs) {
        fightInfo.addMob(context.mob_database.createMob(context.queueBus, mobName));
      }

      return std::make_unique<InFightManager>(parent, context, runInfo, fightInfo, menuBase);
    }
    return nullptr;
  }

  std::unique_ptr<InFightManager> getScene() {
    auto instructions = generateInstructions();
    if (instructions.empty()) {
      return nullptr;
    }
    return generateScene(instructions[0]);
  }

  SceneGenerator(InRunManager *parent, GameContext &context, RunInfo &runInfo, Layout &base) : parent(
      parent),
                                                                                               context(context),
                                                                                               runInfo(runInfo),
                                                                                               menuBase(
                                                                                                   base) {};

};
