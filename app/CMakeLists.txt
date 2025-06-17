cmake_minimum_required(VERSION 3.5)
project(blind_print)

set(CMAKE_CXX_STANDARD 26)
#
# set(SFML_DIR "D:/LibrariesCompilers/SFML/lib/cmake/SFML")
# find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network)

# set(CMAKE_EXE_LINKER_FLAGS "-static")

#[[
add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
add_link_options(-fsanitize=address)
]]


set(SOURCE_DIRS
        src/
)
file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "src/*.h"
        "src/*.hpp"
)


add_executable(blind_print
        main.cpp
        ${SOURCES}
        src/utils/StringUtils.h
        src/scenes/ingame/inrun/CrossroadScene.h
        src/scenes/ingame/inrun/SceneGenerator.h
        src/scenes/ingame/inrun/base/SceneInstructionSelectable.h
        src/scenes/ingame/inrun/base/SceneInstructions.h
        src/scenes/ingame/inrun/action/fight/scenes/DefendScene.h
        src/utils/AssetPaths.h
        src/utils/Utils.h
        src/world_player_and_container/WorldPlayerAndContainer.h
        src/scenes/DummyScene.h
)


include(FetchContent)
#[[add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")]]
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)


# Set properties that will be used while building TGUI
set(BUILD_SHARED_LIBS OFF)      # Determines whether TGUI build a static or dynamic/shared library
set(TGUI_BACKEND SFML_GRAPHICS)  # Sets which backend TGUI will use

FetchContent_Declare(
        TGUI
        GIT_REPOSITORY https://github.com/texus/TGUI
        GIT_TAG v1.9.0

)
FetchContent_MakeAvailable(TGUI)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)


target_link_libraries(blind_print PRIVATE nlohmann_json::nlohmann_json TGUI::TGUI SFML::Graphics SFML::Audio SFML::Network)

# === Тесты ===
project(test_ItemManager)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)

add_executable(test_ItemManager tests/test_ItemManager.cpp
        tests/test_SceneManager.cpp src/scenes/ingame/inrun/inRunManager.cpp)
target_include_directories(test_ItemManager PRIVATE src tests)
target_link_libraries(test_ItemManager PRIVATE Catch2::Catch2WithMain nlohmann_json::nlohmann_json TGUI::TGUI SFML::Graphics SFML::Audio SFML::Network)
