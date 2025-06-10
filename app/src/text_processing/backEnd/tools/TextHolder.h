//
// Created by user on 13.04.2025.
//

#pragma once

#include <algorithm>
#include "TextSource.h"

class TextHolder {
 private:
  size_t bufferSize; // should be >= textDisplayer.maxSize()()
  string buffer;
  size_t ptr;
  TextSource &textSource;
  bool cleanAutomatically = true;

  void reloadIfNeeded() {
    if (buffer.empty() || ptr >= buffer.size()) {
      loadBuffer();
    }
  }

 public:
  TextHolder(size_t bufferSize, string buffer, size_t ptr, TextSource &textSource, bool cleanAutomatically)
      : bufferSize(bufferSize),
        buffer(std::move(buffer)),
        ptr(ptr),
        textSource(textSource), cleanAutomatically(cleanAutomatically) {
    reloadIfNeeded();

  };

  [[nodiscard]] size_t maxSize() const {
    return bufferSize;
  }

  void loadBuffer(string &b) {
    buffer = std::move(b);
  }

  void loadBuffer() {
    buffer.append(textSource.loadNextPatch(bufferSize));
    if (cleanAutomatically && buffer.size() > bufferSize) {
      cleanFirstNChars(ptr);
    }
  }

  void next(size_t hm) {
    ptr += hm;
    reloadIfNeeded();
  }

  void cleanFirstNChars(size_t hm) {
    hm = std::min(hm, buffer.size());
    buffer = buffer.substr(hm, buffer.size() - hm);
    if (ptr >= hm) { //toDo: поэкономить на спичках и убрать иф
      ptr -= hm;
    } else {
      ptr = 0;
    }
  }

  [[nodiscard]] string getBuffer() const {
    return buffer;
  }

  void next() {
    ptr++;
    reloadIfNeeded();
  }

  [[nodiscard]] string getFirstChar() const {
    if (buffer.empty()) {
      return "";
    }
    return buffer[ptr];
  }

  string currentText(size_t max_count) {
    if (buffer.size() - ptr < max_count) {
      loadBuffer();
    }
    return buffer.substr(ptr, std::min(max_count, buffer.size() - ptr));
  }

  [[nodiscard]] bool isEmpty() const {
    return textSource.isEmpty() && ptr >= buffer.size();
  }
};


