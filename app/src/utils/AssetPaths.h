#pragma once

#include <string>
#include <string_view>

namespace Assets {
// Icons and images
inline constexpr std::string_view ICON = "assets/BPIcon.jpg";

// Fonts
inline constexpr const char *const FONT_CONSOLAS = "assets/consolas.ttf"; //can't load from view for some reason

// ASCII art
inline constexpr std::string_view ASCII_BLIND = "assets/ASCII/blind.txt";

// Sounds
inline constexpr std::string_view SOUND_CLICK_ALT = "assets/sounds/ui_sound/click_2.wav";
inline constexpr std::string_view SOUND_CLICK = "assets/sounds/ui_sound/click.wav";
inline constexpr std::string_view SOUND_SLAP = "assets/sounds/slap.wav";

// Texts
inline constexpr std::string_view TEXT_TEST = "assets/texts/test.txt";
inline constexpr std::string_view TEXT_TEST_RU = "assets/texts/test_ru.txt";

// Game resources
inline constexpr const char *const MOB_CAT_JSON = "assets/gameResources/mobs/cat.json";
inline constexpr const char *const MOB_FROG_JSON = "assets/gameResources/mobs/frog.json";
inline constexpr std::string_view MOB_CAT_TXT = "assets/gameResources/mobs/cat.txt";
inline constexpr std::string_view MOB_FROG_TXT = "assets/gameResources/mobs/frog.txt";

// Forms
inline constexpr const char *const FORM_BASE = "forms/base.txt";
inline constexpr const char *const FORM_MENU = "forms/menu.txt";
inline constexpr const char *const FORM_INVENTORY = "forms/inventory.txt";
inline constexpr const char *const FORM_DEATH = "forms/death.txt";
inline constexpr const char *const FORM_HOMEBASE = "forms/homebase.txt";
inline constexpr const char *const FORM_ATTACK_WIDGETS = "forms/attack_widgets.txt";
inline constexpr const char *const FORM_MOB_BASE = "forms/standard/fighting/MobBase.txt";
inline constexpr const char *const FORM_FIGHTING_MENU_WIDGETS = "forms/fighting_menu_widgets.txt";
} 