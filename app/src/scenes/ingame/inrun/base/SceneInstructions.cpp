
#include <atomic>
#include "SceneInstructions.h"

std::atomic<std::size_t> SceneInstruction::globalIdCounter{0};

SceneInstruction::SceneInstruction() : type(Type::FIGHT), uniqueId(globalIdCounter++) {}

SceneInstruction::SceneInstruction(SceneInstruction::Type type, std::vector<std::string> mobs, std::string where)
    : type(type), mobs(std::move(mobs)), where(std::move(where)), uniqueId(globalIdCounter++) {}
