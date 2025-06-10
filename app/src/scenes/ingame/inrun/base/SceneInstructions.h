#pragma once
#include <memory>
#include <string>
#include <vector>

struct SceneInstruction {
  enum class Type {
    FIGHT
  };
  Type type;
  std::vector<std::string> mobs;
  std::string where;
  std::size_t uniqueId; //toDo: const


  SceneInstruction();

  SceneInstruction(Type type, std::vector<std::string> mobs, std::string where);
 public:
  static void setToZero() {
    globalIdCounter = 0;
  }
 private:
  static std::atomic<std::size_t> globalIdCounter;
};