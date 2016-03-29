# FreeTumble

(c) 2009 Seby and Suiland

Original development team:

- Seby <sebygames@gmail.com>, original game logic and engine
- Suiland, graphics

Current maintainers:

- Rémi Verschelde (Akien), bug fixes, packaging

## Description

FreeTumble is a free colourful puzzle game for Windows and Linux.
It features 3 game modes, with simple rules but different gameplay.

NORMAL: Click groups of 3 or more stones to make them disappear.
Each time you click a group, a new row will be added. Don't let the stones
reach the top of the grid or you're done, and make a good use of the various
bonus stones available!

MOTION: Pretty much the same as the 'Normal' game, but real time:
the new row will be added after an amount of time (decreasing as the player
level increases).

CLASSIC: Click groups of 2 or more stones to make them disappear.
Try to clear the entire grid to get the best score.

## Features

- Free and open-source game
- Three game modes
- In-game high scores
- Nice art, sound effects and music
- Four different skins
- Visual effects
- Persistent configuration (language, skin, options, volume)
- Various powerups in Normal and Motion modes
- Available in three languages (English, French, German)

## Build instructions

Build dependencies:

- CMake, http://cmake.org
- SFML 2.x, http://sfml-dev.org

To build and run in the source tree:

```sh
mkdir build && cd build
cmake .. && make
```

To build and install system-wide:

```sh
mkdir build && cd build
cmake -DFOR_INSTALL=ON .. && make
sudo make install
```

## License

FreeTumble's source code is licensed under the GNU General Public
License, version 3 or (at your option) any later version (GPLv3+).

### Graphics

The graphic files have the following license and attribution:

- Suiland, GPLv3+, same copyright as the source code

### Music

The music files have the following licenses and attributions:

- Alex Smith (cynicmusic), Crystal Cave (song18), CC BY-SA 3.0 - http://opengameart.org/content/crystal-cave-song18
- Bart Kelsey (bart), Contemplation, CC BY-SA 3.0 - http://opengameart.org/content/contemplation
- Bart Kelsey (bart), Contemplation II, CC BY-SA 3.0 - http://opengameart.org/content/contemplation-ii
- Przemysław Sikorski (rezoner), Puzzle tune 1, CC BY 3.0 - http://opengameart.org/content/puzzle-tune-1

### Sound effects

The sound files have the following license and attribution:

- Obani (@ObaniGemini), CC BY-SA 4.0 - https://github.com/LibreGameArchive/freetumble/pull/9

### Font

The FreeSansBold.ttf font from the GNU FreeFont project is distributed
under the GPLv3+.
