//
// Created by user on 21.04.2025.
//
#pragma once

#include "../../../base/Layout.h"
#include "../tools/RunningLineDisplay.h"
#include "../tools/HitsTime.h"

class DefendMenu {
  std::string path;
  std::optional<textProcessing::RunningLineDisplay> runningLine;
  std::optional<HitsTime> hitsTime;

  Layout &menuBase;
};