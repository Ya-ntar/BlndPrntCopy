//
// Created by user on 13.04.2025.
//

#pragma once

#include <fstream>
#include "TGUI/String.hpp"

using string = tgui::String;

class TextSource {
 public:
  virtual string loadNextPatch(size_t max_count) = 0;

  [[nodiscard]] virtual int64_t howMuchLeft() const = 0;

  [[nodiscard]] virtual bool isEmpty() const = 0;

  virtual ~TextSource() = default;
};

class StringTextSource : public TextSource {
 private:
  std::string src;
  size_t ptr = 0;
  size_t length = src.size();
 public:
  explicit StringTextSource(const string &buffer) : src(buffer) {};

  string loadNextPatch(size_t max_count) override {
    // from file
    string loaded = src.substr(ptr, std::min(max_count, src.size() - ptr));
    ptr += std::min(max_count, src.size() - ptr);
    return loaded;
  }

  [[nodiscard]] int64_t howMuchLeft() const override {
    return length - ptr;
  }

  [[nodiscard]] bool isEmpty() const override {
    return howMuchLeft() == 0;
  }

};

class GenerativeSource : public TextSource {

  string loadNextPatch(size_t max_count) override {

    string ans(max_count);
    for (int i = 0; i < max_count; ++i) {
      ans.append('0');
    }
    return ans;
  };

  [[nodiscard]] int64_t howMuchLeft() const override { return -1; };

  [[nodiscard]] bool isEmpty() const override { return false; };
};

class FileTextSource : public TextSource {

 private:
  std::ifstream file;
  size_t ptr = 0;
  size_t length;
 public:
  explicit FileTextSource(const std::string &path, size_t length) : file(path), length(length) {
    if (!file.is_open()) {
      throw std::runtime_error("Cannot open file: " + path);
    }

  };

  string loadNextPatch(size_t rough_count) override {
    if (!file.is_open()) {
      throw std::runtime_error("File is not open!"); //toDo: дать path к файлу?
    }

    std::string result(rough_count * 4, '\0');
    file.read(result.data(), rough_count * 4);
    result.resize(file.gcount());

    return result;
  }

  [[nodiscard]] int64_t howMuchLeft() const override {
    return length - ptr;
  }

  [[nodiscard]] bool isEmpty() const override {
    return howMuchLeft() == 0;
  }

};
