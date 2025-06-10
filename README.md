# Blind Print

## Description

Blind Print is a text-based RPG with roguelike elements about the endless library and an invisible labour.
Combat mechanics are based on a blind typing trainer.
This game was developed as part of a student project.

Also, that's my first attempt at creating games! :)
## Technical

Рекомендуется использовать Ninja для сборки

Обработка текста (хотя и пытается быть модульной и независимой от фронтенда), использует TGUI::String в качестве основы
для хранения символов. Возможно необходимо абстрагировать класс, хранящий строки.

## Касательно код-ревью 
Временно сделанная публичной копия, чтобы мне было легче просить совета у других людей. Может незначительно отставать в версии от рабочего проекта.
Проект достаточно большой, число классов перевалило за 30, а число строк кода медленно, но верно идёт к 10к. 

Проблема 1: Мне никогда до этого не доводилось самостоятельно планировать архитектуру. Архитектура, спланированная изначально, была раскритикована преподавателем, причём данные мне советы шли вразрез всему, что мне было известно. Как итог, нынешняя картина это ни рыба, ни мясо с самым наивным разделением, которое пришло мне в голову. Код раздут, хотя делает не так уж много. 
Проблема 2: Я не понимаю как тестировать подобные программы.
Проблема 3: Я знаю C++ ровно настолько, чтобы знать, что C++ я знаю недостаточно.
Проблема 4: Я не знаю, как менять фундаментальные особенности своего кода так, чтобы не разрушать его функциональность полностью.

В соответствии с ТЗ, мне нужно стремиться к google codestyle. Для достижения этой цели, я использую Clang и линтер (SonarQube), которые - на момент написания этого ридми - на меня заслуженно кричат. 
Заострять внимание на рефакторинге не стоит. Основная проблема заключается в местах, где линтеры помочь не могут, а именно картине в целом.

## Общее описание (нынешней) структуры



## Used Instruments and code
### Code snippets
- [PubBus by eXpl0it3r] [reprository](https://github.com/eXpl0it3r/PubBus/tree/faa3f4e4818be5fd86424986bd3bf9b0bd6a876e/include/PubBus) (Public Domain).  - slight changes in includes

### Libraries
- **SFML** (Simple and Fast Multimedia Library) – https://www.sfml-dev.org  
  License: zlib/png (i.e.`licenses/SFMLLicense.md`).
- **TGUI** (Texus' Graphical User Interface) – https://tgui.eu  
    Лицензия: zlib/png (i.e. `licenses/TGUILicense.txt`).  
