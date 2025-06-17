#pragma once

#include <string>
#include <stdexcept>
#include <iostream>

namespace error {

// Базовый класс для ошибок приложения
class GameError : public std::runtime_error {
public:
    explicit GameError(const std::string& message) : std::runtime_error(message) {}
    explicit GameError(const char* message) : std::runtime_error(message) {}
};

// Ошибки загрузки ресурсов
class ResourceLoadError : public GameError {
public:
    explicit ResourceLoadError(const std::string& resource, const std::string& reason = "")
        : GameError("Failed to load resource '" + resource + "'" + (reason.empty() ? "" : ": " + reason)) {}
};

// Ошибки файловых операций
class FileError : public GameError {
public:
    explicit FileError(const std::string& path, const std::string& operation = "access")
        : GameError("Failed to " + operation + " file '" + path + "'") {}
};

// Ошибки инициализации
class InitializationError : public GameError {
public:
    explicit InitializationError(const std::string& component, const std::string& reason = "")
        : GameError("Failed to initialize " + component + (reason.empty() ? "" : ": " + reason)) {}
};

// Ошибки валидации данных
class ValidationError : public GameError {
public:
    explicit ValidationError(const std::string& field, const std::string& reason = "")
        : GameError("Validation error for field '" + field + "'" + (reason.empty() ? "" : ": " + reason)) {}
};

// Функция для логирования ошибок (может быть заменена на более продвинутое логирование)
inline void logError(const std::string& message) {
    std::cerr << "[ERROR] " << message << std::endl;
}

// Функция для логирования предупреждений
inline void logWarning(const std::string& message) {
    std::cerr << "[WARNING] " << message << std::endl;
}

// Функция для логирования информации
inline void logInfo(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

} // namespace error 