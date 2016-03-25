#!/usr/bin/python

import os

# Define build environment
env = Environment()

env.Append(CPPFLAGS = os.getenv('CPPFLAGS', ['-g', '-O2']))
env.Append(LINKFLAGS = os.getenv('LINKFLAGS', ''))
env.Append(LIBS = ['sfml-audio', 'sfml-graphics', 'sfml-system', 'sfml-window'])

datadir = ARGUMENTS.get('DATADIR', '.')
if datadir != '.':
    env.Append(CPPFLAGS = '-DDATADIR=\\"' + datadir + '\\"')

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

freetumble = env.Program(target = 'freetumble', source = sources)

# Install instructions

prefix = ARGUMENTS.get('PREFIX', '/usr/local')
bindir = os.path.join(prefix, 'games')
sharedir = os.path.join(prefix, 'share')

data_folders = ['gfx', 'music', 'sfx', 'skins', 'tilesets']
data_files = ['misc/FreeSansBold.ttf', 'misc/reset_scores.dat']

if datadir != '.':
    env.Alias('install', Install(bindir, freetumble))

    for data_folder in data_folders:
        env.Alias('install', Install(datadir, data_folder))

    for data_file in data_files:
        env.Alias('install', Install(datadir, data_file))

    env.Alias('install', Install(os.path.join(sharedir, 'applications'),
                                 'misc/freetumble.desktop'))
    env.Alias('install', Install(os.path.join(sharedir, 'icons/hicolor/64x64/apps'),
                                 'misc/freetumble.png'))
