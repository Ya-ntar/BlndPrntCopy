# Blind Print

## Description

Blind Print is a text-based RPG with roguelike elements about the endless library and an invisible labour.
Combat mechanics are based on a blind typing trainer.
This game was developed as part of a student project at ITMO University.

Also, that's my first attempt at creating games! :)
## Technical

Рекомендуется использовать Ninja для сборки

Обработка текста (хотя и пытается быть модульной и независимой от фронтенда), использует TGUI::String в качестве основы
для хранения символов. Возможно необходимо абстрагировать класс, хранящий строки.

## Used Instruments and code
### Code snippets
- [PubBus by eXpl0it3r] [reprository](https://github.com/eXpl0it3r/PubBus/tree/faa3f4e4818be5fd86424986bd3bf9b0bd6a876e/include/PubBus) (Public Domain).  - slight changes in includes

### Libraries
- **SFML** (Simple and Fast Multimedia Library) – https://www.sfml-dev.org  
  License: zlib/png (i.e.`licenses/SFMLLicense.md`).
- **TGUI** (Texus' Graphical User Interface) – https://tgui.eu  
    Лицензия: zlib/png (i.e. `licenses/TGUILicense.txt`).  