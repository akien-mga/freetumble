/**  This file is part of FreeTumble.
  *
  *  FreeTumble is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  FreeTumble is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with FreeTumble.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef CGAMEMAIN_H_INCLUDED
#define CGAMEMAIN_H_INCLUDED

#include <time.h>
#include <string>

#include "../contraints.h"
#include "../gfxEngine/GfxEngine.h"
#include "GameModel.h"
#include "GameInput.h"
#include "GameScores.h"
#include "Menu.h"
#include "MenuEntry.h"

using namespace std ;

const int idMenuOptionsSound=0;
//const int idMenuOptionsMusic=1;
const int idMenuOptionsEffects=1;
const int idMenuOptionsLanguages=2;
const int idMenuOptionsSkins=3;
const int idMenuOptionsReset=4;
const int idMenuOptionsBack=5;

const int idMenuQuitQuit=0;
const int idMenuQuitOptions=1;
const int idMenuQuitHelp=2;
const int idMenuQuitCredits=3;
const int idMenuQuitContinue=4;

const int idMenuResetReset=0;
const int idMenuResetBack=1;

const int idMenuSoundSound=0;
const int idMenuSoundSoundVolume=1;
const int idMenuSoundMusic=2;
const int idMenuSoundMusicVolume=3;
const int idMenuSoundBack=4;


class GameMain
{

public:
        GameMain();
        ~GameMain();

        void startGame();

        enum statusEnum
        {
            stateChoice=0,
            statePlaying,
            statePlayingClick,
            stateGameOverAnim,
            stateGameOver,
            stateEnterName
        };

        enum choiceEnum
        {
            choiceNothing=0,
            choiceNormal,
            choiceMotion,
            choiceClassic,
        };

private:
    GfxEngine* gfxEngine;
    GameModel* gameModel;
    GameInput* gameInput;
    GameScores* gameScores;

    int state;
    int choice;
    int scoreGameType;

    int activeMenu;

    float delay;
    float oldTime;

    void newGame();
    void moveInMenu(int x, int y);
    void moveInGame(int x, int y);
    void click(int x, int y);

    void gameOver();
    int xSave;
    int ySave;

    void testScore();
    std::string scoreName;
    int scoreNameSize;

    void proceedInputGame();
    void proceedInputMenu();
    bool done;

    void checkMenuConfiguration();
    Menu* optionMenu;
    Menu* mainMenu;
    Menu* resetMenu;
    Menu* soundMenu;

    void buildMenus();

    Menu* getMenu(int n);
    enum menuEnum
    {
        menuNothing=0,
        menuOptions,
        menuMain,
        menuReset,
        menuSound,
        menuHelp,
        menuCredits
    };

    void resetScores();
    void loadConfig();
    void saveConfig();
};

#endif // CGAMEMAIN_H_INCLUDED
