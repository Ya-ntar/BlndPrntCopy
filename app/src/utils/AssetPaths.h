#pragma once

#include <string>
#include <string_view>

namespace Assets {
// Icons and images
inline constexpr std::string_view ICON = "assets/BPIcon.jpg";

// Fonts
inline constexpr const char *const kFontConsolas = "assets/consolas.ttf"; //can't load from view for some reason

// ASCII art
inline constexpr std::string_view kAsciiBlind = "assets/ASCII/blind.txt";

// Sounds
inline constexpr std::string_view kSoundClickAlt = "assets/sounds/ui_sound/click_2.wav";
inline constexpr std::string_view kSoundClick = "assets/sounds/ui_sound/click.wav";
inline constexpr std::string_view kSoundSlap = "assets/sounds/slap.wav";

// Texts
inline constexpr std::string_view kTextTest = "assets/texts/test.txt";
inline constexpr std::string_view kTextTestRu = "assets/texts/test_ru.txt";

// Game resources
inline constexpr const char *const kMobCatJson = "assets/gameResources/mobs/cat.json";
inline constexpr const char *const kMobFrogJson = "assets/gameResources/mobs/frog.json";
inline constexpr std::string_view kMobCatTxt = "assets/gameResources/mobs/cat.txt";
inline constexpr std::string_view kMobFrogTxt = "assets/gameResources/mobs/frog.txt";

// Forms
inline constexpr const char *const kFormBase = "forms/base.txt";
inline constexpr const char *const kFormMenu = "forms/menu.txt";
inline constexpr const char *const kFormInventory = "forms/inventory.txt";
inline constexpr const char *const kFormDeath = "forms/death.txt";
inline constexpr const char *const kFormHomebase = "forms/homebase.txt";
inline constexpr const char *const kFormAttackWidgets = "forms/attack_widgets.txt";
inline constexpr const char *const kFormMobBase = "forms/standard/fighting/MobBase.txt";
inline constexpr const char *const kFormFightingMenuWidgets = "forms/fighting_menu_widgets.txt";
} 