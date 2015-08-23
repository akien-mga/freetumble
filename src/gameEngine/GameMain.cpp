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

#include "GameMain.h"

#include <iostream>
#include <fstream>
#include <string.h>

GameMain::GameMain()
{
    srand ( time(NULL) );

    gfxEngine=new GfxEngine();
    gameModel=new GameModel();
    gameInput=new GameInput();

    gameScores=new GameScores();
    scoreGameType=GameModel::gameTypeNormal;
    scoreName="";
    gfxEngine->updateScores(gameScores, scoreGameType);

    activeMenu=menuNothing;
    loadConfig();
    buildMenus();

    xSave=0;
    ySave=0;
}

GameMain::~GameMain()
{
    delete(gameScores);
    delete(gameInput);
    delete(gameModel);
    delete(gfxEngine);

    delete (optionMenu);
    delete (mainMenu);
    delete (resetMenu);
    delete (soundMenu);
}

void GameMain::startGame()
{
    state=stateChoice;
    gfxEngine->playMusic(GfxEngine::musicIntro);
    // program main loop
    done = false;
    while (!done)
    {
        // time (menu = freeze time)
        if (activeMenu == menuNothing)
        {
            gfxEngine->updateEffects();

            if (!gameModel->getGameOver() && gameModel->getGameType() == GameModel::gameTypeMotion)
            {
                gameModel->updateTime(gfxEngine->getGameTime());
                if (gameModel->getGameOver())
                {
                    gameOver();
                }
                if (gameModel->getRowUp())
                {
                    gfxEngine->animateUp();
                }
            }
        }
        if (state == stateGameOverAnim)
        {
            if (gfxEngine->getGameOverTime() > gameOverAnimDelay)
            {
                state=stateGameOver;
                gfxEngine->restartBestScoresClock();
                if (gameModel->getGameType() != GameModel::gameTypeClassic)
                {
                    gfxEngine->createGameOverEffect(gameModel);
                }
                testScore();
            }
        }

        gameInput->init();
        gfxEngine->getKeys(gameInput);

        if (activeMenu == menuNothing)
            proceedInputGame();
        else
            proceedInputMenu();

        // RENDERING
        if (activeMenu==menuCredits)
        {
            gfxEngine->renderCredits();
        }
        else if (activeMenu==menuHelp)
        {
            gfxEngine->renderHelp();
        }

        else
        {
            switch (state)
            {
                // intro menu
                case stateChoice:
                {
                    gfxEngine->renderIntro(choice, getMenu(activeMenu));
                    break;
                }

                case statePlaying:
                {
                    gfxEngine->renderGame(gameModel, getMenu(activeMenu));
                    break;
                }

                case statePlayingClick:
                {
                    gfxEngine->renderGameClicking(gameModel);
                    break;
                }

                case stateGameOverAnim:
                {
                    gfxEngine->renderGameOverAnim(gameModel);
                    break;
                }
                case stateGameOver:
                {
                    gfxEngine->renderGameOver(gameModel, gameScores);
                    break;
                }
                case stateEnterName:
                {
                    gfxEngine->renderEnterName(gameModel, scoreName);
                    break;
                }
            }
        }
    } // end main loop

    // end
}

void  GameMain::newGame() {
    state=statePlaying;
    switch(choice) {
        case choiceNormal:
        {
            gameModel->newGame(GameModel::gameTypeNormal);
            gameModel->setHighScore(gameScores->getNormalScore(0).score);
            gfxEngine->playMusic(GfxEngine::musicGame);
            break;
        }
        case choiceMotion:
        {
            gameModel->newGame(GameModel::gameTypeMotion);
            gameModel->setHighScore(gameScores->getMotionScore(0).score);
            gfxEngine->playMusic(GfxEngine::musicGame);
            break;
        }
        case choiceClassic:
        {
            gameModel->newGame(GameModel::gameTypeClassic);
            gameModel->setHighScore(gameScores->getClassicScore(0).score);
            gfxEngine->playMusic(GfxEngine::musicClassic);
            break;
        }
    }
}

void GameMain::moveInGame(int x, int y)
{
    gameModel->emptyZoneGrid();
    gameModel->setClickValue(0);

    //gfxEngine->setMouseOnMenuButton(false);
    if (x >= GAMEMENU_BUTTON_X0 && x <= GAMEMENU_BUTTON_XF
            && y >= GAMEMENU_BUTTON_Y0 && y <= GAMEMENU_BUTTON_YF)
    {
        if (!gfxEngine->getMouseOnMenuButton())
        {
            gfxEngine->setMouseOnMenuButton(true);
            gfxEngine->playSound(GfxEngine::soundMenuOver);
        }
        return;
    }
    else
    {
        if (gfxEngine->getMouseOnMenuButton())
        {
            gfxEngine->setMouseOnMenuButton(false);
            //gfxEngine->playSound(GfxEngine::soundMenuOver);
        }
    }

    if (gameModel->getCanCancel()
            && x >= CANCEL_BUTTON_X0 && x <= CANCEL_BUTTON_XF
            && y >= CANCEL_BUTTON_Y0 && y <= CANCEL_BUTTON_YF)
    {
        if (!gfxEngine->getMouseOnCancelButton())
        {
            gfxEngine->setMouseOnCancelButton(true);
            gfxEngine->playSound(GfxEngine::soundMenuOver);
        }
        return;
    }
    else
    {
        if (gfxEngine->getMouseOnCancelButton())
        {
            gfxEngine->setMouseOnCancelButton(false);
            //gfxEngine->playSound(GfxEngine::soundMenuOver);
        }
    }

    if ((x < OFFSET_X) || (y < OFFSET_Y) || (x > OFFSET_X + GRID_W * TILE_W) || (y > OFFSET_Y + GRID_H * TILE_H))
    {
        gameModel->resetClickValue();
        gameModel->setTx(-1);
        return ;
    }

    gameModel->setTx((x - OFFSET_X) / TILE_W);
    gameModel->setTy((y - OFFSET_Y) / TILE_H);

    int tx=gameModel->getTx();
    int ty=gameModel->getTy();

    gameModel->setOverZone(false);
    if (gameModel->getGameGrid(tx, ty) > 0)
    {
        int nbZone=gameModel->findZone(tx, ty);
        if (gameModel->getGameType() == GameModel::gameTypeClassic)
        {
            if (nbZone > 1)
            {
                gameModel->setOverZone(true);
            }
            else
            {
                gameModel->resetClickValue();
            }
        }
        else
        {
            if (nbZone > 2 || (gameModel->getGameGrid(tx, ty) >= GameModel::tileOneRemove
                                && gameModel->getGameGrid(tx, ty) != GameModel::tileUnremovable))
            {
                gameModel->setOverZone(true);
            }
            else
            {
                gameModel->resetClickValue();
            }
        }
    }
    else
    {
        // do something
    }
}

void GameMain::moveInMenu(int x, int y)
{
    choice=choiceNothing;
    //gfxEngine->setIntroMouseOver(GfxEngine::mouseOverNothing);

    if (x >= MENU_NORMAL_X && x <= MENU_NORMAL_X + MENU_ITEM_W && y >= MENU_NORMAL_Y && y <= MENU_NORMAL_Y + MENU_ITEM_H)
    {
        choice=choiceNormal;
        if (gfxEngine->getIntroMouseOver() != GfxEngine::mouseOverNormal)
        {
            gfxEngine->setIntroMouseOver(GfxEngine::mouseOverNormal);
            gfxEngine->playSound(GfxEngine::soundMenuOver);

            if (scoreGameType != GameModel::gameTypeNormal)
            {
                scoreGameType=GameModel::gameTypeNormal;
                gfxEngine->updateScores(gameScores, scoreGameType);
            }
        }
    }
    else if (x >= MENU_MOTION_X && x <= MENU_MOTION_X + MENU_ITEM_W && y >= MENU_MOTION_Y && y <= MENU_MOTION_Y + MENU_ITEM_H)
    {
        choice=choiceMotion;
        if (gfxEngine->getIntroMouseOver() != GfxEngine::mouseOverMotion)
        {
            gfxEngine->setIntroMouseOver(GfxEngine::mouseOverMotion);
            gfxEngine->playSound(GfxEngine::soundMenuOver);

            if (scoreGameType != GameModel::gameTypeMotion)
            {
                scoreGameType=GameModel::gameTypeMotion;
                gfxEngine->updateScores(gameScores, scoreGameType);
            }
        }
    }
    else if (x >= MENU_CLASSIC_X && x <= MENU_CLASSIC_X + MENU_ITEM_W && y >= MENU_CLASSIC_Y && y <= MENU_CLASSIC_Y + MENU_ITEM_H)
    {
        choice=choiceClassic;
        if (gfxEngine->getIntroMouseOver() != GfxEngine::mouseOverClassic)
        {
            gfxEngine->setIntroMouseOver(GfxEngine::mouseOverClassic);
            gfxEngine->playSound(GfxEngine::soundMenuOver);

            if (scoreGameType != GameModel::gameTypeClassic)
            {
                scoreGameType=GameModel::gameTypeClassic;
                gfxEngine->updateScores(gameScores, scoreGameType);
            }
        }

    }
    else if (x >= MENU_HELP_X && x <= MENU_HELP_X + MENU_ICON_W && y >= MENU_HELP_Y && y <= MENU_HELP_Y + MENU_ICON_H)
    {
        if (gfxEngine->getIntroMouseOver() != GfxEngine::mouseOverHelp)
        {
            gfxEngine->setIntroMouseOver(GfxEngine::mouseOverHelp);
            gfxEngine->playSound(GfxEngine::soundMenuOver);
        }
    }
    else if (x >= MENU_CREDITS_X && x <= MENU_CREDITS_X + MENU_ICON_W && y >= MENU_CREDITS_Y && y <= MENU_CREDITS_Y + MENU_ICON_H)
    {
        if (gfxEngine->getIntroMouseOver() != GfxEngine::mouseOverCredits)
        {
            gfxEngine->setIntroMouseOver(GfxEngine::mouseOverCredits);
            gfxEngine->playSound(GfxEngine::soundMenuOver);
        }
    }
    else if (x >= MENU_OPTIONS_X && x <= MENU_OPTIONS_X + MENU_ICON_W && y >= MENU_OPTIONS_Y && y <= MENU_OPTIONS_Y + MENU_ICON_H)
    {
        if (gfxEngine->getIntroMouseOver() != GfxEngine::mouseOverOptions)
        {
            gfxEngine->setIntroMouseOver(GfxEngine::mouseOverOptions);
            gfxEngine->playSound(GfxEngine::soundMenuOver);
        }
    }
    else if (x >= MENU_QUIT_X && x <= MENU_QUIT_X + MENU_ICON_W && y >= MENU_QUIT_Y && y <= MENU_QUIT_Y + MENU_ICON_H)
    {
        if (gfxEngine->getIntroMouseOver() != GfxEngine::mouseOverQuit)
        {
            gfxEngine->setIntroMouseOver(GfxEngine::mouseOverQuit);
            gfxEngine->playSound(GfxEngine::soundMenuOver);
        }
    }
    else gfxEngine->setIntroMouseOver(GfxEngine::mouseOverNothing);
}

void GameMain::click(int x, int y)
{
    if (x >= GAMEMENU_BUTTON_X0 && x <= GAMEMENU_BUTTON_XF
            && y >= GAMEMENU_BUTTON_Y0 && y <= GAMEMENU_BUTTON_YF)
    {
        activeMenu=menuMain;
        gfxEngine->setMouseOnMenuButton(false);
        gfxEngine->playSound(GfxEngine::soundMenuClick);
        return;
    }

    if (gameModel->getCanCancel()
            && x >= CANCEL_BUTTON_X0 && x <= CANCEL_BUTTON_XF
            && y >= CANCEL_BUTTON_Y0 && y <= CANCEL_BUTTON_YF)
    {
        gfxEngine->setMouseOnCancelButton(false);
        gfxEngine->playSound(GfxEngine::soundMenuClick);
        gameModel->restoreState();

        return;
    }

    //moveInGame (x, y);
    if (gameModel->getTx() < 0) return ;
    if (!gameModel->getOverZone()) return ;

    if (!gameModel->getOverZone()) return;

    gameModel->setTx((x - OFFSET_X) / TILE_W);
    gameModel->setTy((y - OFFSET_Y) / TILE_H);

    gfxEngine->createScoreTextEffect(gameModel->getClickValue());

    gameModel->removeZone();
    moveInGame(xSave, ySave);

    gfxEngine->playSound(GfxEngine::soundBrick);

    if (gameModel->getGameOver())
    {
        gameOver();
    }
    if (gameModel->getRowUp())
    {
        gfxEngine->animateUp();
    }
}

void GameMain::gameOver()
{
    state=stateGameOverAnim;
    gfxEngine->restartGameOverClock();

    gfxEngine->playMusic(GfxEngine::musicGameOver);

    if (gameModel->getGameType() == GameModel::gameTypeClassic && gameModel->getClassicBonus() >= 0)
    {
        gfxEngine->createClassicBonusEffect(gameModel->getClassicBonus());
    }
}

void GameMain::testScore()
{
    int actualScore=gameModel->getScore();
    int tenthScore=gameScores->getScore(scoreGameType, 9).score;
    if (actualScore > tenthScore)
    {
        state=stateEnterName;
    }
}

void GameMain::proceedInputGame()
{


        switch (state)
        {
            // intro menu
            case stateChoice:
            {
                if (gameInput->getQuitKey() || gameInput->getEscKey())
                {
                    done=true;
                    gfxEngine->closeApp();
                }
                if (gameInput->getMouseMotion())
                {
                    moveInMenu(gameInput->getXMouse(), gameInput->getYMouse());
                    gameInput->setMouseMotion(false);
                }
                if (gameInput->getMouseLeftClick())
                {
                    if (choice != choiceNothing)
                    {
                        //state=statePlaying;
                        gfxEngine->playSound(GfxEngine::soundMenuClick);
                        newGame();
                    }
                    else
                    {
                        switch (gfxEngine->getIntroMouseOver())
                        {
                            case (GfxEngine::mouseOverHelp):
                            {
                                activeMenu=menuHelp;
                                gfxEngine->playSound(GfxEngine::soundMenuClick);
                                break;
                            }
                            case (GfxEngine::mouseOverCredits):
                            {
                                activeMenu=menuCredits;
                                gfxEngine->playSound(GfxEngine::soundMenuClick);
                                break;
                            }
                            case (GfxEngine::mouseOverOptions):
                            {
                                activeMenu=menuOptions;
                                gfxEngine->playSound(GfxEngine::soundMenuClick);
                                break;
                            }
                            case (GfxEngine::mouseOverQuit):
                            {
                                gfxEngine->playSound(GfxEngine::soundMenuClick);
                                done=true;
                                gfxEngine->closeApp();
                                return;
                            }
                        }
                    }
                    gameInput->setMouseLeftClick(false);
                }
                break;
            }

            case statePlaying:
            {
                if (gameInput->getEscKey())
                {
                    //state=stateChoice;
                    activeMenu=menuMain;
                }
                if (gameInput->getQuitKey())
                {
                    done=true;
                    gfxEngine->closeApp();
                }
                if (gameInput->getRemoveKey()) {
                    gameModel->restoreState();
                    moveInGame(xSave, ySave);
                }
                if (gameInput->getMouseMotion())
                {
                    moveInGame(gameInput->getXMouse(), gameInput->getYMouse());
                }
                if (gameInput->getMouseLeftClick())
                {
                    xSave=gameInput->getXMouse();
                    ySave=gameInput->getYMouse();
                    if (gameModel->getOverZone())
                    {
                        gfxEngine->createRemoveEffect(gameModel);
                        //click(xSave, ySave);
                    }
                    click(xSave, ySave);
                }
                break;
            }


            case stateGameOver:
            {
                if (gameInput->getEscKey())
                {
                    state=stateChoice;
                }
                if (gameInput->getQuitKey())
                {
                    done=true;
                    gfxEngine->closeApp();
                }
                if (gameInput->getMouseMotion())
                {
                    xSave=gameInput->getXMouse();
                    ySave=gameInput->getYMouse();
                    // left button ?
                    if (xSave > MENU_POSX && xSave < MENU_POSX + MENU_W / 2 && ySave >= SCORESMENU_BUTTONL_Y && ySave <= SCORESMENU_BUTTONL_Y + MENU_BUTTONL_H)
                    {
                        if (!gfxEngine->getMouseOnLeftButton())
                        {
                            gfxEngine->setMouseOnLeftButton(true);
                            gfxEngine->playSound(GfxEngine::soundMenuOver);
                        }
                    }
                    else
                    {
                        gfxEngine->setMouseOnLeftButton(false);
                    }
                    if (xSave > MENU_POSX + MENU_W / 2 && xSave < MENU_POSX + MENU_W && ySave >= SCORESMENU_BUTTONL_Y && ySave <= SCORESMENU_BUTTONL_Y + MENU_BUTTONL_H)
                    {
                        if (!gfxEngine->getMouseOnRightButton())
                        {
                            gfxEngine->setMouseOnRightButton(true);
                            gfxEngine->playSound(GfxEngine::soundMenuOver);
                        }
                    }
                    else
                    {
                        gfxEngine->setMouseOnRightButton(false);
                    }
                }
                if (gameInput->getMouseLeftClick())
                {
                    if (gfxEngine->getMouseOnLeftButton())
                    {
                        state=stateChoice;
                        gfxEngine->playMusic(GfxEngine::musicIntro);
                        gameScores->resetCurrentScore();
                    }
                    else if (gfxEngine->getMouseOnRightButton())
                    {
                        gameScores->resetCurrentScore();
                        newGame();
                    }
                    //gameModel->newGame(GameModel::gameTypeNormal);
                }
                break;
            }

            case stateEnterName:
            {
                if (gameInput->getQuitKey()) {
                    done=true;
                }
                if (gameInput->getRemoveKey()) {
                    if (scoreName.size() > 0) scoreName.erase(scoreName.size()-1);
                }
                if (gameInput->getMiscKey()) {
                    if (scoreName.size() <= +SCORE_NAME_MAX) scoreName += gameInput->getCharKey();
                }
                if (gameInput->getEnterKey()) {
                    gameScores->addScore(scoreGameType, gameModel->getScore(), scoreName);
                    gfxEngine->updateScores(gameScores, scoreGameType);
                    state=stateGameOver;
                    gfxEngine->restartBestScoresClock();
                }
                break;
            }

            case statePlayingClick: case stateGameOverAnim:
            {
                if (gameInput->getEscKey())
                {
                    state=stateChoice;
                    gfxEngine->playMusic(GfxEngine::musicIntro);
                }
                if (gameInput->getQuitKey())
                {
                    done=true;
                    gfxEngine->closeApp();
                }
                if (gameInput->getMouseMotion())
                {
                    xSave=gameInput->getXMouse();
                    ySave=gameInput->getYMouse();
                }

                break;
            }
        }
}

void GameMain::proceedInputMenu()
{
    if (activeMenu==menuCredits || activeMenu==menuHelp)
    {
        if (gameInput->getQuitKey())
        {
            done=true;
            gfxEngine->closeApp();
            return;
        }
        if (gameInput->getEscKey() || gameInput->getMouseLeftClick())
        {
            //state=stateChoice;
            activeMenu=menuNothing;
        }
        return;
    }

    int oldMenu=activeMenu;

    if (gameInput->getQuitKey())
    {
        done=true;
        gfxEngine->closeApp();
        return;
    }
    if (gameInput->getEscKey())
    {
        activeMenu=menuNothing;
        checkMenuConfiguration();
        return;
    }
    if (gameInput->getMouseMotion())
    {
        gameInput->setMouseMotion(false);
        xSave=gameInput->getXMouse();
        ySave=gameInput->getYMouse();

        int n=gfxEngine->getMenuMouseOver(xSave, ySave, getMenu(activeMenu));
        if (getMenu(activeMenu)->getSelectedEntry() != n)
        {
            getMenu(activeMenu)->setSelectedEntry(n);
            gfxEngine->playSound(GfxEngine::soundMenuOver);
        }
    }
    if (gameInput->getMouseLeftClick() || gameInput->getMouseRightClick())
    {
        bool leftButton=gameInput->getMouseLeftClick();

        switch (activeMenu)
        {
            case menuOptions:
            {
                switch (optionMenu->getSelectedEntry())
                {
                    //case idMenuOptionsSound:
                    //case idMenuOptionsMusic:
                    case idMenuOptionsEffects:
                    case idMenuOptionsLanguages:
                    case idMenuOptionsSkins:
                    {
                        if (leftButton)
                            optionMenu->getSelectedMenuEntry()->selectNextChoice();
                        else
                            optionMenu->getSelectedMenuEntry()->selectPreviousChoice() ;
                        checkMenuConfiguration();
                        break;
                    }
                    case idMenuOptionsSound: activeMenu=menuSound; break;
                    case idMenuOptionsReset: activeMenu=menuReset; break;
                    case idMenuOptionsBack:
                    {
                        if (state == stateChoice)
                            activeMenu=menuNothing;
                        else
                            activeMenu=menuMain;
                        checkMenuConfiguration();
                        break;
                    }
                }
                break;
            }
            case menuMain:
            {
                switch (mainMenu->getSelectedEntry())
                {
                    case idMenuQuitQuit:
                    {
                        activeMenu=menuNothing;
                        state=stateChoice;
                        gfxEngine->playMusic(GfxEngine::musicIntro);
                        break;
                    }
                    case idMenuQuitContinue:
                    {
                        activeMenu=menuNothing;
                        //checkMenuConfiguration();
                        break;
                    }
                    case idMenuQuitOptions:
                    {
                        activeMenu=menuOptions;
                        break;
                    }
                    case idMenuQuitHelp:
                    {
                        activeMenu=menuHelp;
                        break;
                    }
                    case idMenuQuitCredits:
                    {
                        activeMenu=menuCredits;
                        break;
                    }
                }
                break;
            }

            case menuReset:
            {
                switch (resetMenu->getSelectedEntry())
                {
                    case idMenuResetReset:
                    {
                        activeMenu=menuOptions;
                        resetScores();
                        break;
                    }
                    case idMenuResetBack:
                    {
                        activeMenu=menuOptions;
                        break;
                    }
                }
            }

            case menuSound:
            {
                switch (soundMenu->getSelectedEntry())
                {
                    case idMenuSoundSound:
                    case idMenuSoundMusic:
                    case idMenuSoundSoundVolume:
                    case idMenuSoundMusicVolume:
                    {
                        if (leftButton)
                            soundMenu->getSelectedMenuEntry()->selectNextChoice();
                        else
                            soundMenu->getSelectedMenuEntry()->selectPreviousChoice() ;

                        if (soundMenu->getSelectedEntry() == idMenuSoundSoundVolume)
                            gfxEngine->setSoundVolume(soundMenu->getSelectedMenuEntry()->getChoiceIndex() * 10);

                        else if (soundMenu->getSelectedEntry() == idMenuSoundMusicVolume)
                            gfxEngine->setMusicVolume(soundMenu->getSelectedMenuEntry()->getChoiceIndex() * 10);
                        else
                            checkMenuConfiguration();
                        break;
                    }
                    case idMenuSoundBack:
                    {
                        activeMenu=menuOptions;
                        break;
                    }
                }
            }
        }

        if (getMenu(oldMenu)->getSelectedEntry() > -1)
            gfxEngine->playSound(GfxEngine::soundMenuClick);

        gameInput->init();
        if (oldMenu != activeMenu && activeMenu != menuNothing)
        {
            // update mouse over
            gameInput->setMouseMotion(true);
            gameInput->setXMouse(xSave);
            gameInput->setYMouse(ySave);
            proceedInputMenu();
        }
    }

}

void GameMain::buildMenus()
{
    int i=0;
    int lang=gfxEngine->getLanguage();

    //delete optionMenu;
    optionMenu=new Menu(MENU_OPTIONS_TITLE[lang]);

    MenuEntry* entryOptions1=new MenuEntry(MENU_OPTIONS_SOUND_CONFIG[lang], MenuEntry::typeButton);
    optionMenu->addEntry(entryOptions1);

    MenuEntry* entryOptions3=new MenuEntry(MENU_OPTIONS_EFFECTS[lang], MenuEntry::typeChoice);
    entryOptions3->addChoice(MENU_YES[lang]);
    entryOptions3->addChoice(MENU_NO[lang]);
    if (!gfxEngine->getEffectsEnabled()) entryOptions3->setChoiceIndex(1);
    optionMenu->addEntry(entryOptions3);

    MenuEntry* entryOptions4=new MenuEntry(MENU_OPTIONS_LANGUAGE[lang], MenuEntry::typeChoice);
    for (i=0; i < N_LANGUAGES; i++) entryOptions4->addChoice(LANGUAGES[lang][i]);
    entryOptions4->setChoiceIndex(gfxEngine->getLanguage());
    optionMenu->addEntry(entryOptions4);

    MenuEntry* entryOptions5=new MenuEntry(MENU_OPTIONS_SKIN[lang], MenuEntry::typeChoice);
    for (i=0; i < NB_SKINS; i++) entryOptions5->addChoice(WSKINS[i]);
    entryOptions5->setChoiceIndex(gfxEngine->getSkin());
    optionMenu->addEntry(entryOptions5);

    MenuEntry* entryOptions6=new MenuEntry(MENU_OPTIONS_RESET_SCORES[lang], MenuEntry::typeButton);
    optionMenu->addEntry(entryOptions6);

    MenuEntry* lastEntry=new MenuEntry(MENU_BACK[lang], MenuEntry::typeButton);
    optionMenu->addEntry(lastEntry);

    //delete mainMenu;
    mainMenu=new Menu(MENU_MAIN_TITLE[lang]);
    MenuEntry* entryQuit1=new MenuEntry(MENU_QUIT_QUIT[lang], MenuEntry::typeButton);
    mainMenu->addEntry(entryQuit1);
    MenuEntry* entryQuit2=new MenuEntry(MENU_OPTIONS_TITLE[lang], MenuEntry::typeButton);
    mainMenu->addEntry(entryQuit2);
    MenuEntry* entryQuit3=new MenuEntry(MENU_HELP[lang], MenuEntry::typeButton);
    mainMenu->addEntry(entryQuit3);
    MenuEntry* entryQuit4=new MenuEntry(MENU_CREDITS[lang], MenuEntry::typeButton);
    mainMenu->addEntry(entryQuit4);
    MenuEntry* entryQuit5=new MenuEntry(MENU_BACK[lang], MenuEntry::typeButton);
    mainMenu->addEntry(entryQuit5);

    //delete resetMenu;
    resetMenu=new Menu(MENU_RESET_TITLE[lang]);
    MenuEntry* entryReset1=new MenuEntry(MENU_RESET_RESET[lang], MenuEntry::typeButton);
    resetMenu->addEntry(entryReset1);
    MenuEntry* entryReset2=new MenuEntry(MENU_BACK[lang], MenuEntry::typeButton);
    resetMenu->addEntry(entryReset2);

    //delete soundMenu;
    soundMenu=new Menu(MENU_OPTIONS_SOUND_CONFIG[lang]);
    MenuEntry* entrySoundOptions1=new MenuEntry(MENU_OPTIONS_SOUND[lang], MenuEntry::typeChoice);
    entrySoundOptions1->addChoice(MENU_YES[lang]);
    entrySoundOptions1->addChoice(MENU_NO[lang]);
    if (!gfxEngine->getSoundEnabled()) entrySoundOptions1->setChoiceIndex(1);
    soundMenu->addEntry(entrySoundOptions1);

    MenuEntry* entrySoundOptions2=new MenuEntry(MENU_OPTIONS_SOUND_VOLUME[lang], MenuEntry::typeChoice);
    entrySoundOptions2->addChoice(L"0%");
    entrySoundOptions2->addChoice(L"10%");
    entrySoundOptions2->addChoice(L"20%");
    entrySoundOptions2->addChoice(L"30%");
    entrySoundOptions2->addChoice(L"40%");
    entrySoundOptions2->addChoice(L"50%");
    entrySoundOptions2->addChoice(L"60%");
    entrySoundOptions2->addChoice(L"70%");
    entrySoundOptions2->addChoice(L"80%");
    entrySoundOptions2->addChoice(L"90%");
    entrySoundOptions2->addChoice(L"100%");
    entrySoundOptions2->setChoiceIndex(gfxEngine->getSoundVolume() / 10);
    soundMenu->addEntry(entrySoundOptions2);

    MenuEntry* entrySoundOptions3=new MenuEntry(MENU_OPTIONS_MUSIC[lang], MenuEntry::typeChoice);
    entrySoundOptions3->addChoice(MENU_YES[lang]);
    entrySoundOptions3->addChoice(MENU_NO[lang]);
    if (!gfxEngine->getMusicEnabled()) entrySoundOptions3->setChoiceIndex(1);
    soundMenu->addEntry(entrySoundOptions3);

    MenuEntry* entrySoundOptions4=new MenuEntry(MENU_OPTIONS_MUSIC_VOLUME[lang], MenuEntry::typeChoice);
    entrySoundOptions4->addChoice(L"0%");
    entrySoundOptions4->addChoice(L"10%");
    entrySoundOptions4->addChoice(L"20%");
    entrySoundOptions4->addChoice(L"30%");
    entrySoundOptions4->addChoice(L"40%");
    entrySoundOptions4->addChoice(L"50%");
    entrySoundOptions4->addChoice(L"60%");
    entrySoundOptions4->addChoice(L"70%");
    entrySoundOptions4->addChoice(L"80%");
    entrySoundOptions4->addChoice(L"90%");
    entrySoundOptions4->addChoice(L"100%");
    entrySoundOptions4->setChoiceIndex(8);
    entrySoundOptions4->setChoiceIndex(gfxEngine->getMusicVolume() / 10);
    soundMenu->addEntry(entrySoundOptions4);

    MenuEntry* entrySoundOptions5=new MenuEntry(MENU_BACK[lang], MenuEntry::typeButton);
    soundMenu->addEntry(entrySoundOptions5);
}

Menu* GameMain::getMenu(int n)
{
    if (n == menuOptions) return optionMenu;
    if (n == menuMain) return mainMenu;
    if (n == menuReset) return resetMenu;
    if (n == menuSound) return soundMenu;
    return NULL;
}

void GameMain::checkMenuConfiguration()
{
    if (soundMenu->getMenuEntry(idMenuSoundSound)->getChoiceIndex() == 0)
        gfxEngine->setSoundEnabled(true);
    else
        gfxEngine->setSoundEnabled(false);


    if (soundMenu->getMenuEntry(idMenuSoundMusic)->getChoiceIndex() == 0)
    {
        if (!gfxEngine->getMusicEnabled())
        {
            gfxEngine->setMusicEnabled(true);
            if (state == statePlaying)
            {
                if (choice == choiceClassic)
                    gfxEngine->playMusic(GfxEngine::musicClassic);
                else
                    gfxEngine->playMusic(GfxEngine::musicGame);
            }
            else
                gfxEngine->playMusic(GfxEngine::musicIntro);
        }
    }
    else
    {
        gfxEngine->setMusicEnabled(false);
        gfxEngine->stopMusic();
    }

    if (optionMenu->getMenuEntry(idMenuOptionsEffects)->getChoiceIndex() == 0)
        gfxEngine->setEffectsEnabled(true);
    else
        gfxEngine->setEffectsEnabled(false);

    gfxEngine->setSkin(optionMenu->getMenuEntry(idMenuOptionsSkins)->getChoiceIndex());

    if (gfxEngine->getLanguage() != optionMenu->getMenuEntry(idMenuOptionsLanguages)->getChoiceIndex())
    {
        gfxEngine->setLanguage(optionMenu->getMenuEntry(idMenuOptionsLanguages)->getChoiceIndex());
        buildMenus();
    }
    saveConfig();
}

void GameMain::resetScores()
{
    gameScores->reset();

    if (state == statePlaying)
    {
        switch(choice)
        {
        case choiceNormal: gameModel->setHighScore(gameScores->getNormalScore(0).score); break;
        case choiceMotion: gameModel->setHighScore(gameScores->getMotionScore(0).score); break;
        case choiceClassic: gameModel->setHighScore(gameScores->getClassicScore(0).score); break;
        }
    }
}

void GameMain::loadConfig()
{
    ifstream f("data/game.ini");
    if (!f.is_open()) return;

    char c[32];
    int percent;

    // sound
    f>>c;
    f>>c;
    if (strcmp(c, "yes") == 0) gfxEngine->setSoundEnabled(true);
    else gfxEngine->setSoundEnabled(false);

    f>>c;
    f>>percent;
    gfxEngine->setSoundVolume(percent);

    // music
    f>>c;
    f>>c;
    if (strcmp(c, "yes") == 0) gfxEngine->setMusicEnabled(true);
    else gfxEngine->setMusicEnabled(false);

    f>>c;
    f>>percent;
    gfxEngine->setMusicVolume(percent);

    // effects
    f>>c;
    f>>c;
    if (strcmp(c, "yes") == 0) gfxEngine->setEffectsEnabled(true);
    else gfxEngine->setEffectsEnabled(false);

    // language
    f>>c;
    f>>c;
    if (strcmp(c, "FR") == 0) gfxEngine->setLanguage(LANG_FR);
    else if (strcmp(c, "DE") == 0) gfxEngine->setLanguage(LANG_DE);
    else gfxEngine->setLanguage(LANG_EN);

    // language
    f>>c;
    f>>c;
    if (strcmp(c, "Blue") == 0) gfxEngine->setSkin(skinBlue);
    else if (strcmp(c, "Green") == 0) gfxEngine->setSkin(skinGreen);
    else if (strcmp(c, "Brown") == 0) gfxEngine->setSkin(skinBrown);
    else gfxEngine->setSkin(skinRosa);

    f.close();
}

void GameMain::saveConfig()
{
    ofstream f("data/game.ini");
    if (!f.is_open()) return;

    // sound
    f << "sound" << endl;
    if (gfxEngine->getSoundEnabled()) f << "yes" << endl;
    else f << "no" << endl;

    f << "volume" << endl;
    f << gfxEngine->getSoundVolume() << endl;

    // music
    f << "music" << endl;
    if (gfxEngine->getMusicEnabled()) f << "yes" << endl;
    else f << "no" << endl;

    f << "volume" << endl;
    f << gfxEngine->getMusicVolume() << endl;

    // effects
    f << "effects" << endl;
    if (gfxEngine->getEffectsEnabled()) f << "yes" << endl;
    else f << "no" << endl;

    // language
    f << "lang" << endl;
    if (gfxEngine->getLanguage() == LANG_FR) f << "FR" << endl;
    else if (gfxEngine->getLanguage() == LANG_DE) f << "DE" << endl;
    else f << "EN" << endl;

    // skin
    f << "skin" << endl;
    if (gfxEngine->getSkin() == skinBlue) f << "Blue" << endl;
    else if (gfxEngine->getSkin() == skinGreen) f << "Green" << endl;
    else if (gfxEngine->getSkin() == skinBrown) f << "Brown" << endl;
    else f << "Rosa" << endl;

    f.close();
}
