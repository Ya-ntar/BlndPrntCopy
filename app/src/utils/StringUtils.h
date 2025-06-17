#pragma once

#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "ErrorHandler.h"

inline std::vector<std::string> splitLines(const std::string &text) {
  std::vector<std::string> lines;
  std::istringstream stream(text);
  std::string line;
  while (std::getline(stream, line)) {
    lines.push_back(line);
  }
  return lines;
}

inline void padLinesToMaxWidth(std::vector<std::string> &lines) {
  size_t maxLen = 0;
  for (const auto &line : lines) {
    maxLen = std::max(line.length(), maxLen);
  }
  for (auto &line : lines) {
    if (line.length() < maxLen) {
      line += std::string(maxLen - line.length(), ' ');
    }
  }
}

inline std::string joinLines(const std::vector<std::string> &lines) {
  std::string result;
  for (const auto &line : lines) {
    result += line + '\n';
  }
  return result;
}

// Center ASCII art to the given width (or to the maximum line length if width == 0)
inline std::string centerAsciiArt(const std::string &art, size_t width = 0) {
  auto lines = splitLines(art);
  size_t maxLen = 0;
  for (const auto &line : lines) {
    maxLen = std::max(line.length(), maxLen);
  }
  if (width == 0) {
    width = maxLen;
  }
  for (auto &line : lines) {
    size_t pad = (width > line.length()) ? (width - line.length()) / 2 : 0;
    line = std::string(pad, ' ') + line + std::string(width - pad - line.length(), ' ');
  }
  return joinLines(lines);
}

inline std::string getStringFromFile(const std::string &path) {
  std::string asciiArt;
  std::string line;

  std::ifstream file(path);
  if (file.is_open()) {
    while (getline(file, line)) {
      asciiArt += line;
      asciiArt += '\n';
    }
    file.close();
  } else {
    throw error::FileError(path, "read");
  }
  return asciiArt;
}