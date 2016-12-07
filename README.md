# LuaPacman

## Installation

This project runs on MacOS with Xcode 8.1

The project uses two external frameworks:
- SDL2 from [https://www.libsdl.org/release/SDL2-2.0.5.dmg]
- Lua from [https://github.com/derkyjadex/Lua-Framework]

SDL is used to handles graphics and system interactions (e.g. key events)
Lua is the interpreter, all the game logic is in Lua

## Native extensions for our LuaPacman context

We extend the global Lua context with the following native functions
- drawRect: To display a filled rectangle on screen
- getKey: To grab keyboard inputs
- usleep: To avoid busy looping

## OOP with Lua

The pacman game is using several objects
- Pacman: Entry point, loading the map and grab key events
- Scene: Contains a list of cells and a list of characters (for now only pacman)
- Cell: Is a basic type handling the painting of background items
- Character: Handle the painting of overlay elements (e.g. pacman)
