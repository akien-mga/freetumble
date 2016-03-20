#!/usr/bin/python

import os

# Define build environment
env = Environment()

env.Append(CPPFLAGS = os.getenv('CPPFLAGS', ['-g', '-O2']))
env.Append(LINKFLAGS = os.getenv('LINKFLAGS', ''))
env.Append(LIBS = ['sfml-audio', 'sfml-graphics', 'sfml-system', 'sfml-window'])

sources = [
    'src/main.cpp',
    'src/gameEngine/MenuEntry.cpp',
    'src/gameEngine/Menu.cpp',
    'src/gameEngine/GameInput.cpp',
    'src/gameEngine/GameMain.cpp',
    'src/gameEngine/GameModel.cpp',
    'src/gameEngine/GameScores.cpp',
    'src/gfxEngine/EntityManager.cpp',
    'src/gfxEngine/GameEntity.cpp',
    'src/gfxEngine/GfxEngine.cpp',
]

env.Program(target = 'freetumble', source = sources)
