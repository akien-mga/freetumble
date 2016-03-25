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

#include "GfxEngine.h"
#include "MyTools.h"
#include "GameEntity.h"
#include <sstream>
#include <iostream>
#include "../languages.h"

std::wstring string2wstring(const std::string& s)
{
    std::wstring result(s.length(),L' ');
    std::copy(s.begin(), s.end(), result.begin());
    return result;
}

GfxEngine::GfxEngine()
{
    statusOK=true;
    initialize();
}

GfxEngine::~GfxEngine()
{
    int i;

    delete (textScore);
    delete (textHighScore);
    delete (textValue);
    delete (textLevel);
    delete (textScoreName);
    delete (textMode);
    for (i=0; i < 10; i++)
    {
        delete (textNames[i]);
        delete (textScores[i]);
    }
    for (i=0; i < 2; i++)
    {
        delete (normalDescription[i]);
        delete (motionDescription[i]);
        delete (classicDescription[i]);
    }
    delete (EM);
    delete (app);
}

void GfxEngine::initialize()
{
    soundEnabled=true;
    soundVolume=80;
    musicEnabled=true;
    musicVolume=100;
    effectsEnabled=true;
    language=LANG_EN;
    skin=skinBrown;
    tileSet=tsPastel;

    mouseOnMenuButton=false;
    mouseOnLeftButton=false;
    mouseOnRightButton=false;
    mouseOnCancelButton=false;
    introMouseOver=mouseOverNothing;

    oldXMouse=0;
    oldYMouse=0;
    initWindow();
    upAnimation=false;
    consoleIndex=0;
    loadPictures();
    loadSounds();
    EM=new EntityManager();
    initScores();
}

void GfxEngine::initWindow()
{
    // create a new window
    string title=APP_NAME + " " + APP_VERSION;
    app = new sf::RenderWindow(sf::VideoMode(800, 600), title); // , sf::Style::Close);
    app->setVerticalSyncEnabled(true);
}

void GfxEngine::setSkin(int skin)
{
    if (this->skin != skin)
    {
        this->skin=skin;
        loadSkin();
    }
}

void GfxEngine::setLanguage(int language)
{
    if (this->language != language)
        this->language=language;
}

void GfxEngine::loadPictures()
{
    // load textures;

    intro.loadFromFile(getDataFile("gfx/menuBg.png"));
    introSprite.setTexture(intro);
    //introSprite.setPosition(200, 50);

    loadSkin();
    loadTileSet();
    bgSprite.setTexture(bg);

    gameOver.loadFromFile(getDataFile("gfx/gameover.png"));
    gameOverSprite.setTexture(gameOver);
    gameOverSprite.setPosition(300, 225);


    levelSprite.setTexture(levelImage);
    levelSprite.setPosition(679, 379);

    modeNormal.loadFromFile(getDataFile("gfx/modeNormal.png"));
    modeNormalOver.loadFromFile(getDataFile("gfx/modeNormalOver.png"));
    normalSprite.setTexture(modeNormal);
    normalSprite.setPosition(MENU_NORMAL_X, MENU_NORMAL_Y);
    modeNormalMini.loadFromFile(getDataFile("gfx/modeNormalMini.png"));
    normalMiniSprite.setTexture(modeNormalMini);
    modeNormalHiscores.loadFromFile(getDataFile("gfx/modeNormalHiscores.png"));

    modeMotion.loadFromFile(getDataFile("gfx/modeMotion.png"));
    modeMotionOver.loadFromFile(getDataFile("gfx/modeMotionOver.png"));
    motionSprite.setTexture(modeMotion);
    motionSprite.setPosition(MENU_MOTION_X, MENU_MOTION_Y);
    modeMotionMini.loadFromFile(getDataFile("gfx/modeMotionMini.png"));
    motionMiniSprite.setTexture(modeMotionMini);
    modeMotionHiscores.loadFromFile(getDataFile("gfx/modeMotionHiscores.png"));

    modeClassic.loadFromFile(getDataFile("gfx/modeClassic.png"));
    modeClassicOver.loadFromFile(getDataFile("gfx/modeClassicOver.png"));
    classicSprite.setTexture(modeClassic);
    classicSprite.setPosition(MENU_CLASSIC_X, MENU_CLASSIC_Y);
    modeClassicMini.loadFromFile(getDataFile("gfx/modeClassicMini.png"));
    classicMiniSprite.setTexture(modeClassicMini);
    modeClassicHiscores.loadFromFile(getDataFile("gfx/modeClassicHiscores.png"));

    buttonHelpImage.loadFromFile(getDataFile("gfx/buttonHelp.png"));
    buttonHelpSprite.setTexture(buttonHelpImage);
    buttonHelpSprite.setPosition(MENU_HELP_X, MENU_HELP_Y);

    buttonCreditsImage.loadFromFile(getDataFile("gfx/buttonCredits.png"));
    buttonCreditsSprite.setTexture(buttonCreditsImage);
    buttonCreditsSprite.setPosition(MENU_CREDITS_X, MENU_CREDITS_Y);

    buttonOptionsImage.loadFromFile(getDataFile("gfx/buttonOptions.png"));
    buttonOptionsSprite.setTexture(buttonOptionsImage);
    buttonOptionsSprite.setPosition(MENU_OPTIONS_X, MENU_OPTIONS_Y);

    buttonQuitImage.loadFromFile(getDataFile("gfx/buttonQuit.png"));
    buttonQuitSprite.setTexture(buttonQuitImage);
    buttonQuitSprite.setPosition(MENU_QUIT_X, MENU_QUIT_Y);

    miniButtonSprite.setTexture(buttonHelpImage);
    miniButtonSprite.setTextureRect(sf::IntRect(0, 0, buttonHelpImage.getSize().x / 2, buttonHelpImage.getSize().y));
    miniButtonSprite.scale(0.4f, 0.4f);

    cancelButtonImage.loadFromFile(getDataFile("gfx/cancel64.png"));
    cancelButtonSprite.setTexture(cancelButtonImage);
    cancelButtonSprite.setPosition(CANCEL_BUTTON_X0, CANCEL_BUTTON_Y0);

    menuButtonSprite.setTexture(menuButtonImage);
    menuButtonSprite.setPosition(GAMEMENU_BUTTON_X0, GAMEMENU_BUTTON_Y0);

    blackFootImage.create(800, 40);
    blackFootSprite.setTexture(blackFootImage);
    blackFootSprite.setPosition(blackFootSprite.getPosition().x, 547);

    whiteScreenImage.create(600, 480);

    helpBg.loadFromFile(getDataFile("gfx/helpBg.png"));
    helpBgSprite.setTexture(helpBg);

    creditsBg.loadFromFile(getDataFile("gfx/creditsBg.png"));
    creditsBgSprite.setTexture(creditsBg);

    sfmlLogoImage.loadFromFile(getDataFile("gfx/logoSFML.png"));
    sfmlLogoSprite.setTexture(sfmlLogoImage);
    sfmlLogoSprite.setPosition(510, 210);

    blackScreen.create(800, 600);
    blackScreenSprite.setTexture(blackScreen);
    blackScreenSprite.setColor(sf::Color(0, 0, 0, 192));

    menuImage.loadFromFile(getDataFile("gfx/menu.png"));
    menuSpriteTop.setTexture(menuImage);
    menuSpriteTop.setTextureRect(sf::IntRect(MENU_TOP_X0, MENU_TOP_Y0, MENU_TOP_W, MENU_TOP_H));
    menuSpriteButton.setTexture(menuImage);
    menuSpriteButton.setTextureRect(sf::IntRect(MENU_BUTTON_X0, MENU_BUTTON_Y0, MENU_BUTTON_W, MENU_BUTTON_H));
    menuSpriteButtonOver.setTexture(menuImage);
    menuSpriteButtonOver.setTextureRect(sf::IntRect(MENU_BUTTON_OVER_X0, MENU_BUTTON_OVER_Y0, MENU_BUTTON_OVER_W, MENU_BUTTON_OVER_H));
    menuSpriteButtonL.setTexture(menuImage);
    menuSpriteButtonL.setTextureRect(sf::IntRect(MENU_BUTTONL_X0, MENU_BUTTONL_Y0, MENU_BUTTONL_W, MENU_BUTTONL_H));
    menuSpriteButtonR.setTexture(menuImage);
    menuSpriteButtonR.setTextureRect(sf::IntRect(MENU_BUTTONR_X0, MENU_BUTTONR_Y0, MENU_BUTTONR_W, MENU_BUTTONR_H));

    menuSpriteButtonLOver.setTexture(menuImage);
    menuSpriteButtonLOver.setTextureRect(sf::IntRect(MENU_BUTTONL_OVER_X0, MENU_BUTTONL_OVER_Y0, MENU_BUTTONL_OVER_W, MENU_BUTTONL_OVER_H));
    menuSpriteButtonROver.setTexture(menuImage);
    menuSpriteButtonROver.setTextureRect(sf::IntRect(MENU_BUTTONR_OVER_X0, MENU_BUTTONR_OVER_Y0, MENU_BUTTONR_OVER_W, MENU_BUTTONR_OVER_H));

    menuSpriteBottom.setTexture(menuImage);
    menuSpriteBottom.setTextureRect(sf::IntRect(MENU_BOTTOM_X0, MENU_BOTTOM_Y0, MENU_BOTTOM_W, MENU_BOTTOM_H));
    menuSpriteHiScore1.setTexture(menuImage);
    menuSpriteHiScore1.setTextureRect(sf::IntRect(MENU_SCORE1_X0, MENU_SCORE1_Y0, MENU_SCORE1_W, MENU_SCORE1_H));
    menuSpriteHiScore2.setTexture(menuImage);
    menuSpriteHiScore2.setTextureRect(sf::IntRect(MENU_SCORE2_X0, MENU_SCORE2_Y0, MENU_SCORE2_W, MENU_SCORE2_H));

    menuSpriteSpace.setTexture(menuImage);
    menuSpriteSpace.setTextureRect(sf::IntRect(0, 40, MENU_W, 40 + MENU_SPACE_H));

    //sf::Sprite menuSpriteTop;
    //sf::Sprite menuSpriteBottom;
    //sf::Sprite menuSpriteButton;
    //sf::Sprite menuSpriteButtonOver;

    dangerImage.loadFromFile(getDataFile("gfx/danger.png"));
    int i=0;
    for (i=0; i<GRID_W; i++)
    {
        dangerSprite[i].setTexture(dangerImage);
        dangerSprite[i].setPosition(i * TILE_W + OFFSET_X + (TILE_W - DANGER_W) / 2, OFFSET_Y-TILE_H);
    }
    singleTile.setTexture(tiles);
    singleTile.setScale(0.8f, 0.8f);

    for (i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            tile[i][j].setTexture(tiles);
            tile[i][j].setPosition(i * TILE_W + OFFSET_X, j * TILE_H + OFFSET_Y);
        }
        nextRowTile[i].setTexture(nextTiles);
        //nextRowTile[i].setPosition(i * TILE_W + OFFSET_X + TILE_W / 4, GRID_H * TILE_H + OFFSET_Y + TILE_H / 4);
        nextRowTile[i].setPosition(i * TILE_W + OFFSET_X + 7, GRID_H * TILE_H + OFFSET_Y + 26);
        //nextRowTile[i].setScale(0.5f, 0.5f);
    }

    holeSprite.setTexture(holeImage);
    holeSprite.setPosition(holeSprite.getPosition().x, GRID_H * TILE_H + OFFSET_Y + 7);

    effectsSprite.setTexture(effectsImage);

    boomImage.loadFromFile(getDataFile("gfx/boom40.png"));

    lightningHImage.loadFromFile(getDataFile("gfx/LightningH.png"));
    lightningVImage.loadFromFile(getDataFile("gfx/LightningV.png"));

    font.loadFromFile(getDataFile("FreeSansBold.ttf"));


    textHighScore=new sf::Text("0", font, STRING_SIZE_SCORE);
    //textHighScore->setPosition(665,186);
    textHighScore->setPosition(665,177);

    textScore=new sf::Text("0", font, STRING_SIZE_SCORE);
    //textScore->setPosition(665,280);
    textScore->setPosition(665,282);

    textValue=new sf::Text("0", font, STRING_SIZE_SCORE);
    //textValue->setPosition(665,312);
    textValue->setPosition(665,317);
    textValue->setColor(sf::Color(128, 128, 255, 255));

    textLevel=new sf::Text("0", font, STRING_SIZE_SCORE);
    //textLevel->setPosition(740,390);
    textLevel->setPosition(706,424);

    textScoreName=new sf::Text("", font, 30);
    textScoreName->setPosition(125,551);

    textMode=new sf::Text("NORMAL", font, 22);
    textMode->setPosition(660,525);
    textMode->setColor(sf::Color::Black);

    normalDescription[0]=new sf::Text(
        "Click zones with 3 or more stones of the same color to make them disappear.", font, STRING_SIZE_DESCRIPTION);
    normalDescription[0]->setPosition(10,542);
    normalDescription[0]->setColor(sf::Color::Black);

    normalDescription[1]=new sf::Text(
        "Don't let the stones rise to the top ! (Turn based)", font, STRING_SIZE_DESCRIPTION);
    normalDescription[1]->setPosition(10,567);
    normalDescription[1]->setColor(sf::Color::Black);

    motionDescription[0]=new sf::Text(
        "Click zones with 3 or more stones of the same color to make them disappear.", font, STRING_SIZE_DESCRIPTION);
    motionDescription[0]->setPosition(10,542);
    motionDescription[0]->setColor(sf::Color::Black);

    motionDescription[1]=new sf::Text(
        "Don't let the stones rise to the top ! (Real time)", font, STRING_SIZE_DESCRIPTION);
    motionDescription[1]->setPosition(10,567);
    motionDescription[1]->setColor(sf::Color::Black);

    classicDescription[0]=new sf::Text(
        "Click zones with 2 or more stones of the same color to make them disappear.", font, STRING_SIZE_DESCRIPTION);
    classicDescription[0]->setPosition(10,542);
    classicDescription[0]->setColor(sf::Color::Black);

    classicDescription[1]=new sf::Text(
        "Try to clear the entire grid !", font, STRING_SIZE_DESCRIPTION);
    classicDescription[1]->setPosition(10,567);
    classicDescription[1]->setColor(sf::Color::Black);

    menuString=new sf::Text("", font, STRING_SIZE_MENU_TITLE);
}

void GfxEngine::loadSkin()
{
    string dirBase = getDataFile("skins/");
    string dirBaseStd = getDataFile("skins/blue/");

    switch (skin)
    {
        case skinBlue: dirBase += "blue/"; break;
        case skinGreen: dirBase += "green/"; break;
        case skinRosa: dirBase += "rosa/"; break;
        case skinBrown: dirBase += "brown/"; break;
    }

    if (!bg.loadFromFile(dirBase + "bg.png"))
    {
        bg.loadFromFile(dirBaseStd + "bg.png");
    }

    if (!holeImage.loadFromFile(dirBase + "hole40.png"))
    {
        holeImage.loadFromFile(dirBaseStd + "hole40.png");
    }

    if (!levelImage.loadFromFile(dirBase + "level.png"))
    {
        levelImage.loadFromFile(dirBaseStd + "level.png");
    }

    if (!menuButtonImage.loadFromFile(dirBase + "menu_button.png"))
    {
        menuButtonImage.loadFromFile(dirBaseStd + "menu_button.png");
    }
}

void GfxEngine::loadTileSet()
{
    string dirBase = getDataFile("tilesets/");
    string dirBaseStd = getDataFile("tilesets/pastel/");

    switch (tileSet)
    {
        case tsPastel: dirBase += "pastel/"; break;
        case tsMarble: dirBase += "marble/"; break;
        //case skinRosa: dirBase += "rosa/"; break;
        case tsGems: dirBase += "gems/"; break;
    }


    if (!tiles.loadFromFile(dirBase + "tiles.png"))
    {
        tiles.loadFromFile(dirBaseStd + "tiles.png");
    }

    if (!nextTiles.loadFromFile(dirBase + "nexttiles.png"))
    {
        nextTiles.loadFromFile(dirBaseStd + "nexttiles.png");
    }

    if (!effectsImage.loadFromFile(dirBase + "effects.png"))
    {
        effectsImage.loadFromFile(dirBaseStd + "effects.png");
    }
}

void GfxEngine::loadSounds()
{
    // load audio;
    brickBuffer.loadFromFile(getDataFile("sfx/brick.wav"));
    sound[soundBrick].setBuffer(brickBuffer);

    explosionBuffer.loadFromFile(getDataFile("sfx/explosion.wav"));
    sound[soundExplode].setBuffer(explosionBuffer);

    thunderBuffer.loadFromFile(getDataFile("sfx/thunder.wav"));
    sound[soundThunder].setBuffer(thunderBuffer);

    colorBuffer.loadFromFile(getDataFile("sfx/color.wav"));
    sound[soundColor].setBuffer(colorBuffer);

    dangerBuffer.loadFromFile(getDataFile("sfx/danger.wav"));
    sound[soundDanger].setBuffer(dangerBuffer);

    menuOverBuffer.loadFromFile(getDataFile("sfx/menu_over.wav"));
    sound[soundMenuOver].setBuffer(menuOverBuffer);

    menuClickBuffer.loadFromFile(getDataFile("sfx/menu_click.wav"));
    sound[soundMenuClick].setBuffer(menuClickBuffer);

    for (int i=0; i < NB_SOUNDS; i++)
        sound[i].setVolume((float)soundVolume);

    music.setVolume((float)musicVolume);
}

void GfxEngine::renderIntro(int choice, Menu* myMenu)
{
    //App->draw(bgSprite);

    app->draw(introSprite);

    if (introMouseOver == mouseOverNormal)
    {
        normalSprite.setTexture(modeNormalOver);
    }
    else
    {
        normalSprite.setTexture(modeNormal);
    }

    if (introMouseOver == mouseOverMotion)
    {
        motionSprite.setTexture(modeMotionOver);
    }
    else
    {
        motionSprite.setTexture(modeMotion);
    }

    if (introMouseOver == mouseOverClassic)
    {
        classicSprite.setTexture(modeClassicOver);
    }
    else
    {
        classicSprite.setTexture(modeClassic);
    }

    if (introMouseOver == mouseOverHelp)
    {
        buttonHelpSprite.setTextureRect(sf::IntRect(MENU_ICON_W, 0, MENU_ICON_W, MENU_ICON_H));
    }
    else
    {
        buttonHelpSprite.setTextureRect(sf::IntRect(0, 0, MENU_ICON_W, MENU_ICON_H));
    }

    if (introMouseOver == mouseOverOptions)
    {
        buttonOptionsSprite.setTextureRect(sf::IntRect(MENU_ICON_W, 0, MENU_ICON_W, MENU_ICON_H));
    }
    else
    {
        buttonOptionsSprite.setTextureRect(sf::IntRect(0, 0, MENU_ICON_W, MENU_ICON_H));
    }

    if (introMouseOver == mouseOverCredits)
    {
        buttonCreditsSprite.setTextureRect(sf::IntRect(MENU_ICON_W, 0, MENU_ICON_W, MENU_ICON_H));
    }
    else
    {
        buttonCreditsSprite.setTextureRect(sf::IntRect(0, 0, MENU_ICON_W, MENU_ICON_H));
    }

    if (introMouseOver == mouseOverQuit)
    {
        buttonQuitSprite.setTextureRect(sf::IntRect(MENU_ICON_W, 0, MENU_ICON_W, MENU_ICON_H));
    }
    else
    {
        buttonQuitSprite.setTextureRect(sf::IntRect(0, 0, MENU_ICON_W, MENU_ICON_H));
    }

    app->draw(normalSprite);
    app->draw(motionSprite);
    app->draw(classicSprite);

    app->draw(buttonHelpSprite);
    app->draw(buttonOptionsSprite);
    app->draw(buttonCreditsSprite);
    app->draw(buttonQuitSprite);

    renderConsole();
    renderBestScores();
    renderMenu(myMenu);


    app->display();
}

void GfxEngine::renderConsole()
{
    if (getConsoleTime() > CONSOLE_NEXT_LINE_DELAY)
    {
        consoleClock.restart();
        consoleIndex++;
        if (consoleIndex >= CONSOLE_MAX_LINES) consoleIndex=0;
    }

    menuString->setColor(sf::Color::White);
    menuString->setCharacterSize(CONSOLE_STRING_SIZE);

    int first=consoleIndex-2;
    if (first < 0) first=0;

    for (int i=first; i <= consoleIndex; i++)
    {

        if (i == consoleIndex && getConsoleTime() < CONSOLE_LINE_DELAY)
        {
            int strSize=CONSOLE[i][language].length();
            int n=strSize * getConsoleTime()/CONSOLE_LINE_DELAY;
            menuString->setString(CONSOLE[i][language].substr(0, n));
        }
        else
        {
            menuString->setString(CONSOLE[i][language]);
        }

        int xTrans=0;
        if (i >= CONSOLE_HELP && i <= CONSOLE_QUIT)
        {
            xTrans=35;
            switch (i)
            {
                case (CONSOLE_HELP): miniButtonSprite.setTexture(buttonHelpImage); break;
                case (CONSOLE_OPTIONS): miniButtonSprite.setTexture(buttonOptionsImage); break;
                case (CONSOLE_CREDITS): miniButtonSprite.setTexture(buttonCreditsImage); break;
                case (CONSOLE_QUIT): miniButtonSprite.setTexture(buttonQuitImage); break;
            }
            miniButtonSprite.setPosition(CONSOLE_X, CONSOLE_Y0 + (i - first) * CONSOLE_LINE_H);
            app->draw(miniButtonSprite);
        }

        menuString->setPosition(CONSOLE_X + xTrans, CONSOLE_Y0 + (i - first) * CONSOLE_LINE_H);
        app->draw(*menuString);
    }
    //menuString->setPosition(CONSOLE_X, CONSOLE_Y1);
    //menuString->setString(CONSOLE[1][language]);
    //app->draw(*menuString);

    //menuString->setPosition(CONSOLE_X, CONSOLE_Y2);
    //menuString->setString(CONSOLE[2][language]);
    //app->draw(*menuString);
}

void GfxEngine::renderGameOver(GameModel* gameModel, GameScores* gameScores)
{
    app->draw(bgSprite);
    if (gameModel->getGameType() == GameModel::gameTypeClassic || !effectsEnabled)
    {
        renderGrid(gameModel);
    }

    //app->draw(gameOverSprite);



    renderEffects();

    renderGameOverBestScores(gameScores, gameModel->getGameType());
    renderScores(gameModel);

    menuButtonSprite.setTextureRect(sf::IntRect(0, 0, GAMEMENU_BUTTON_W, GAMEMENU_BUTTON_H));
    app->draw(menuButtonSprite);

    app->display();
}

void GfxEngine::renderGameOverAnim(GameModel* gameModel)
{
    app->draw(bgSprite);

    if (gameModel->getGameType() == GameModel::gameTypeClassic || !effectsEnabled)
    {
        renderGrid(gameModel);
    }
    else
    {
        for (int i=0; i < GRID_W; i++)
        {
            for (int j=0; j < GRID_H; j++)
            {
                if (gameModel->getGameGrid(i, j) > 0)
                {
                    renderTile(i, j, gameModel->getGameGrid(i, j) - 1, xRand * 1.5, yRand * 1.5);
                }
            }
        }
    }
    renderEffects();
    renderScores(gameModel);

    menuButtonSprite.setTextureRect(sf::IntRect(0, 0, GAMEMENU_BUTTON_W, GAMEMENU_BUTTON_H));
    app->draw(menuButtonSprite);

    app->draw(gameOverSprite);

    app->display();
}

void GfxEngine::renderGame(GameModel* gameModel, Menu* myMenu)
{
    app->draw(bgSprite);

    renderGrid(gameModel);

    if (gameModel->getGameType() != GameModel::gameTypeClassic && !gameModel->getGameOver())
    {
        renderNextRow(gameModel);
        renderDanger(gameModel);
    }

    renderModeIcone(gameModel->getGameType());  // NEW DESIGN

    renderEffects();
    renderScores(gameModel);

    menuButtonSprite.setTextureRect(sf::IntRect(0, mouseOnMenuButton ? GAMEMENU_BUTTON_H : 0,
                                    GAMEMENU_BUTTON_W, GAMEMENU_BUTTON_H));
    app->draw(menuButtonSprite);

    if (gameModel->getGameType() == GameModel::gameTypeClassic)
    {
        if (gameModel->getCanCancel())
        {
            if (mouseOnCancelButton)
                cancelButtonSprite.setTextureRect(sf::IntRect(CANCEL_BUTTON_W, 0, CANCEL_BUTTON_W, CANCEL_BUTTON_H));
            else
                cancelButtonSprite.setTextureRect(sf::IntRect(0, 0, CANCEL_BUTTON_W, CANCEL_BUTTON_H));
            cancelButtonSprite.setColor(sf::Color(255, 255, 255, 255));
            app->draw(cancelButtonSprite);
        }
        else
        {
            cancelButtonSprite.setTextureRect(sf::IntRect(0, 0, CANCEL_BUTTON_W, CANCEL_BUTTON_H));
            cancelButtonSprite.setColor(sf::Color(255, 255, 255, 80));
            app->draw(cancelButtonSprite);
        }
    }

    renderMenu(myMenu);

    app->display();
}

void GfxEngine::renderGameClicking(GameModel* gameModel)
{
    app->draw(bgSprite);

    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            if (gameModel->getGameGrid(i, j) > 0)
            {
                if (gameModel->getOverZone() && gameModel->getZoneGrid(i, j))
                {
                    //
                }
                else
                {
                    renderTile(i, j, gameModel->getGameGrid(i, j) - 1);
                }
            }
        }
    }

    if (gameModel->getGameType() != GameModel::gameTypeClassic && !gameModel->getGameOver())
    {
        renderNextRow(gameModel);
        renderDanger(gameModel);
    }

    renderEffects();
    renderScores(gameModel);

    app->display();
}

void GfxEngine::renderGrid(GameModel* gameModel)
{
    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            if (gameModel->getGameGrid(i, j) > 0) {
                if (gameModel->getOverZone() && gameModel->getZoneGrid(i, j) && !gameModel->getGameOver())
                {
                    renderTile(i, j, gameModel->getGameGrid(i, j) - 1, xRand, yRand);
                }
                else
                {
                    renderTile(i, j, gameModel->getGameGrid(i, j) - 1);
                }
            }
        }
    }
}



void GfxEngine::renderModeIcone(int mode)
{
    switch (mode)
    {
        case GameModel::gameTypeNormal:
        {
            normalMiniSprite.setPosition((714 - normalMiniSprite.getTextureRect().width / 2), 482);
            app->draw(normalMiniSprite);
            break;
        }
        case GameModel::gameTypeMotion:
        {
            motionMiniSprite.setPosition((714 - motionMiniSprite.getTextureRect().width / 2), 482);
            app->draw(motionMiniSprite);
            break;
        }
        case GameModel::gameTypeClassic:
        {
            classicMiniSprite.setPosition((714 - classicMiniSprite.getTextureRect().width / 2), 482);
            app->draw(classicMiniSprite);
            break;
        }
    }
}

void GfxEngine::renderNextRow(GameModel* gameModel)
{
    int x=GRID_W;

    if (gameModel->getGameType() == GameModel::gameTypeMotion)
    {
        x=1 + gameModel->getPercentDelay() * GRID_W;
        if (x > GRID_W) x=GRID_W;
    }

    for (int i=0; i < GRID_W; i++)
    {
        holeSprite.setPosition(i * TILE_W + OFFSET_X, holeSprite.getPosition().y);

        app->draw(holeSprite);

        if (i < x)
        {
            int mTile=gameModel->getNextRow(i) - 1.;
            nextRowTile[i].setTextureRect(sf::IntRect(mTile * 28, 0, 28, 27));
            app->draw(nextRowTile[i]);
        }
    }
}

void GfxEngine::renderTile(int x, int y, int mTile, float xTrans, float yTrans)
{
    int scaleY=TILE_H;
    int xPos=x * TILE_W + OFFSET_X + xTrans;
    int yPos=y * TILE_H + OFFSET_Y + yTrans;

    if (getUpTime() <= upDelay)
    {
        float fade=(upDelay - getUpTime()) / upDelay;

        yTrans+=OFFSET_Y * fade;
        yPos=y * TILE_H + OFFSET_Y + yTrans;

        if (y == GRID_H - 1)
        {
            scaleY=TILE_H * (1.0f - fade);
            scaleY=(TILE_H * GRID_H + OFFSET_Y) - yPos;
        }
    }
    tile[x][y].setPosition(xPos, yPos);

    tile[x][y].setTextureRect(sf::IntRect(mTile * TILE_W, 0, TILE_W, scaleY));
    app->draw(tile[x][y]);

    if (!effectsEnabled) return;

    mTile++;
    if (mTile >= GameModel::tileOneRemove && mTile <= GameModel::tileFourRemove)
    {
        effectsSprite.setPosition(xPos, yPos);
        effectsSprite.setTextureRect(sf::IntRect(0, 0, TILE_W, scaleY));
        int alpha=128 + 127 * cosf(getGameTime() * 8);
        effectsSprite.setColor(sf::Color(255, 255, 255, alpha));
        app->draw(effectsSprite);
    }
    else if (mTile == GameModel::tileBomb)
    {
        effectsSprite.setPosition(xPos, yPos);
        effectsSprite.setTextureRect(sf::IntRect(TILE_W, 0, TILE_W, scaleY));
        int alpha=128 + 127 * cosf(getGameTime() * 9);
        effectsSprite.setColor(sf::Color(255, 255, 255, alpha));
        app->draw(effectsSprite);
    }
    else if (mTile == GameModel::tileArrows)
    {
        effectsSprite.setPosition(xPos, yPos);
        effectsSprite.setTextureRect(sf::IntRect(TILE_W * 2, 0, TILE_W, scaleY));
        int alpha=128 + 127 * cosf(getGameTime() * 7);
        effectsSprite.setColor(sf::Color(255, 255, 255, alpha));
        app->draw(effectsSprite);
    }
}

void GfxEngine::renderScores(GameModel* gameModel)
{
    int scoreXf=761;
    std::ostringstream valStream;
    if (gameModel->getClickValue() > 0)
    {
        valStream << "+" << gameModel->getClickValue();
        textValue->setString(valStream.str());
        textValue->setPosition(scoreXf - textValue->getLocalBounds().width, textValue->getPosition().y);
        app->draw(*textValue);
    }

    std::ostringstream scoreStream;
    scoreStream << gameModel->getScore();
    textScore->setString(scoreStream.str());

    std::ostringstream highScoreStream;
    highScoreStream << gameModel->getHighScore();
    textHighScore->setString(highScoreStream.str());

    textScore->setPosition(scoreXf - textScore->getLocalBounds().width, textScore->getPosition().y);
    textHighScore->setPosition(scoreXf - textHighScore->getLocalBounds().width, textHighScore->getPosition().y);
    app->draw(*textScore);
    app->draw(*textHighScore);

    if (gameModel->getGameType() != GameModel::gameTypeClassic)
    {
        app->draw(levelSprite); // NEW DESIGN
        std::ostringstream levelStream;
        levelStream << gameModel->getLevel();
        textLevel->setString(levelStream.str());
        textLevel->setPosition(714 - textLevel->getLocalBounds().width / 2, textLevel->getPosition().y);
        app->draw(*textLevel);
    }
}

void GfxEngine::renderDanger(GameModel* gameModel)
{
    bool isInDanger=false;
    if ((int)(getGameTime() * 3) % 2 == 0) return;
    for (int i=0; i < GRID_W; i++)
    {
        if (gameModel->getDanger(i))
        {
            app->draw(dangerSprite[i]);
            isInDanger=true;
        }
    }
    if (!soundEnabled) return;
    if (isInDanger && sound[soundDanger].getStatus() != sf::Sound::Playing)
    {
        sound[soundDanger].play();
    }
    else if (!isInDanger && sound[soundDanger].getStatus() == sf::Sound::Playing)
    {
        sound[soundDanger].stop();
    }
}

void GfxEngine::renderEffects()
{
    if (!EM->IsEmpty())
    {
        EM->InitIterator();
        GameEntity* ge = EM->NextItem();
        while (ge != NULL)
        {
            if (ge->type == GameEntity::typeSprite)
            {
                app->draw(ge->sprite);
            }
            else if (ge->type == GameEntity::typeText)
            {
                app->draw(ge->text);
            }
            ge=EM->NextItem();
        }
    }
}

void GfxEngine::renderBestScores()
{
    app->draw(modeScoreSprite);
    //app->draw(*typeScore);

    // TODO

    for (int i=0; i < 10; i++)
    {
        app->draw(*textNames[i]);
        app->draw(*textScores[i]);
        app->draw(*textPos[i]);
    }
}

void GfxEngine::renderGameOverBestScores(GameScores* gameScores, int gameType)
{
    int yTop=(SCREEN_H - (MENU_TOP_H + MENU_SCORE_H * 10 + MENU_BUTTONL_H + MENU_BOTTOM_H + MENU_SPACE_H)) / 2;
    int middle=MENU_POSX + MENU_W / 2;

    int yTrans=0;
    if (getBestScoresTime() < bestScoresDelay)
    {
        yTrans -= (bestScoresDelay - getBestScoresTime()) / bestScoresDelay * (MENU_TOP_H + MENU_SCORE_H * 10 + MENU_BUTTONL_H + MENU_BOTTOM_H + MENU_SPACE_H);
    }

    yTop += yTrans;

    //app->draw(blackScreenSprite);

    menuString->setColor(sf::Color::White);
    menuSpriteTop.setPosition(MENU_POSX, yTop);
    app->draw(menuSpriteTop);

    menuString->setString(STRING_HI_SCORES[language]);
    menuString->setCharacterSize(STRING_SIZE_MENU_TITLE);
    menuString->setPosition(middle - menuString->getLocalBounds().width / 2, yTop + 11);
    app->draw(*menuString);

    menuString->setCharacterSize(STRING_SIZE_SCORE);

    for (int i=0; i < 10; i++)
    {
        int y=yTop + MENU_TOP_H + i * MENU_SCORE_H;
        if (i % 2 == 0)
        {
            menuSpriteHiScore1.setPosition(MENU_POSX, y);
            app->draw(menuSpriteHiScore1);
        }
        else
        {
            menuSpriteHiScore2.setPosition(MENU_POSX, y);
            app->draw(menuSpriteHiScore2);
        }

        if (gameScores->getCurrentScore() % 10 == i)
        {
            menuString->setColor(sf::Color(100 + sinf(getGameTime() * 3) * 100,
                                        100 + sinf(getGameTime() * 4) * 100,
                                        100 + cosf(getGameTime() * 5) * 100));
        }
        else
        {
            menuString->setColor(sf::Color::Black);
        }

        menuString->setString(PODIUM_STRING[i]);
        menuString->setPosition(MENU_POSX + 25, y + 3);
        app->draw(*menuString);

        switch (gameType)
        {
            case GameModel::gameTypeNormal:
            {
                menuString->setString(gameScores->getNormalScore(i).name);
                menuString->setPosition(MENU_POSX + 65, y + 3);
                app->draw(*menuString);

                std::ostringstream scoreStream;
                scoreStream << gameScores->getNormalScore(i).score;
                menuString->setString(scoreStream.str());
                menuString->setPosition(MENU_POSX + 475 - menuString->getLocalBounds().width, y + 3);
                app->draw(*menuString);

                break;
            }

            case GameModel::gameTypeMotion:
            {
                menuString->setString(gameScores->getMotionScore(i).name);
                menuString->setPosition(MENU_POSX + 65, y + 3);
                app->draw(*menuString);

                std::ostringstream scoreStream;
                scoreStream << gameScores->getMotionScore(i).score;
                menuString->setString(scoreStream.str());
                menuString->setPosition(MENU_POSX + 475 - menuString->getLocalBounds().width, y + 3);
                app->draw(*menuString);

                break;
            }

            case GameModel::gameTypeClassic:
            {
                menuString->setString(gameScores->getClassicScore(i).name);
                menuString->setPosition(MENU_POSX + 65, y + 3);
                app->draw(*menuString);

                std::ostringstream scoreStream;
                scoreStream << gameScores->getClassicScore(i).score;
                menuString->setString(scoreStream.str());
                menuString->setPosition(MENU_POSX + 475 - menuString->getLocalBounds().width, y + 3);
                app->draw(*menuString);

                break;
            }
        }
    }

    // separator
    menuSpriteSpace.setPosition(MENU_POSX, yTop + MENU_TOP_H + 10 * MENU_SCORE_H);
    app->draw(menuSpriteSpace);

    menuString->setColor(sf::Color::White);

    int yButton=SCORESMENU_BUTTONL_Y; //yTop + MENU_TOP_H + 10 * MENU_SCORE_H + MENU_SPACE_H;

    if (mouseOnLeftButton)
    {
        menuSpriteButtonLOver.setPosition(MENU_POSX, yButton + yTrans);
        app->draw(menuSpriteButtonLOver);
    }
    else
    {
        menuSpriteButtonL.setPosition(MENU_POSX, yButton + yTrans);
        app->draw(menuSpriteButtonL);
    }

    menuString->setString(STRING_BACK_TO_MENU[language]);
    menuString->setPosition(MENU_POSX + MENU_W / 4 - menuString->getLocalBounds().width / 2, yButton + 13 + yTrans);
    app->draw(*menuString);

    if (mouseOnRightButton)
    {
        menuSpriteButtonROver.setPosition(MENU_POSX + MENU_W / 2, yButton + yTrans);
        app->draw(menuSpriteButtonROver);
    }
    else
    {
        menuSpriteButtonR.setPosition(MENU_POSX + MENU_W / 2, yButton + yTrans);
        app->draw(menuSpriteButtonR);
    }

    menuString->setString(STRING_PLAY_AGAIN[language]);
    menuString->setPosition(MENU_POSX + 3 * MENU_W / 4 - menuString->getLocalBounds().width / 2, yButton + 13 + yTrans);
    app->draw(*menuString);

    menuSpriteBottom.setPosition(MENU_POSX, yTop + MENU_TOP_H + 10 * MENU_SCORE_H + MENU_BUTTONL_H + MENU_SPACE_H);
    app->draw(menuSpriteBottom);
}

void GfxEngine::renderEnterName(GameModel* gameModel, string name)
{
    app->draw(bgSprite);
    if (gameModel->getGameType() == GameModel::gameTypeClassic || !effectsEnabled)
    {
        renderGrid(gameModel);
    }
    renderEffects();
    renderScores(gameModel);
    menuButtonSprite.setTextureRect(sf::IntRect(0, 0, GAMEMENU_BUTTON_W, GAMEMENU_BUTTON_H));
    app->draw(menuButtonSprite);

    //app->draw(gameOverSprite);

    int middle=MENU_POSX + MENU_W / 2;
    int yTop= (SCREEN_H - (MENU_TOP_H + MENU_BOTTOM_H + MENU_BUTTON_H)) / 2;

    menuString->setColor(sf::Color::White);

    menuSpriteTop.setPosition(MENU_POSX, yTop);
    app->draw(menuSpriteTop);
    menuString->setString(ENTER_NAME[language]);
    menuString->setCharacterSize(STRING_SIZE_MENU_TITLE);
    menuString->setPosition(middle - menuString->getLocalBounds().width / 2, yTop + 11);
    app->draw(*menuString);

    menuSpriteButtonOver.setPosition(MENU_POSX, yTop + MENU_TOP_H);
    app->draw(menuSpriteButtonOver);

    textScoreName->setString(string2wstring(name) + L"_");
    textScoreName->setPosition(middle - textScoreName->getLocalBounds().width / 2, yTop + MENU_TOP_H + 11);
    if ((int)(getGameTime() * 3) % 2 > 0) textScoreName->setString(string2wstring(name));
    app->draw(*textScoreName);

    //app->draw(blackFootSprite);
    menuSpriteBottom.setPosition(MENU_POSX, yTop + MENU_TOP_H + MENU_BUTTON_H);
    app->draw(menuSpriteBottom);


    app->display();
}

void GfxEngine::renderMenu(Menu* myMenu)
{
    int middle=MENU_POSX + MENU_W / 2;
    if (myMenu == NULL) return;

    int n=myMenu->getNEntries();

    int yTop=(SCREEN_H - (MENU_TOP_H + MENU_BOTTOM_H + n * MENU_BUTTON_H)) / 2;

    app->draw(blackScreenSprite);

    menuString->setColor(sf::Color::White);
    menuSpriteTop.setPosition(MENU_POSX, yTop);
    app->draw(menuSpriteTop);
    menuString->setString(myMenu->getTitle());
    menuString->setCharacterSize(STRING_SIZE_MENU_TITLE);
    menuString->setPosition(middle - menuString->getLocalBounds().width / 2, yTop + 11);
    app->draw(*menuString);

    for (int i=0; i < n; i++)
    {
        int y=yTop + MENU_TOP_H + i * MENU_BUTTON_H;

        if (myMenu->getSelectedEntry() == i)
        {
            menuSpriteButtonOver.setPosition(MENU_POSX, y);
            app->draw(menuSpriteButtonOver);
        }
        else
        {
            menuSpriteButton.setPosition(MENU_POSX, y);
            app->draw(menuSpriteButton);
        }

        menuString->setString(myMenu->getFullEntry(i));
        menuString->setCharacterSize(STRING_SIZE_MENU_ITEM);
        menuString->setPosition(middle - menuString->getLocalBounds().width / 2, y + 11);
        app->draw(*menuString);
    }

    menuSpriteBottom.setPosition(MENU_POSX, yTop + MENU_TOP_H + n * MENU_BUTTON_H);
    app->draw(menuSpriteBottom);
}

void GfxEngine::renderCredits()
{
    app->draw(creditsBgSprite);

    menuString->setColor(sf::Color::White);
    //menuString->setCharacterSize(HELP_TITLE_SIZE);
    //menuString->setString(CREDITS_TITLE[language]);
    //menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 40);
    //app->draw(*menuString);

    //menuString->setString(APP_NAME + " " + APP_VERSION);
    //menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 15);
    //app->draw(*menuString);

    menuString->setCharacterSize(HELP_SUBTITLE_SIZE);
    menuString->setString(CREDITS_PROGRAMMER[language]);
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 120);
    app->draw(*menuString);
    menuString->setCharacterSize(HELP_TEXT_SIZE);
    menuString->setString(L"Seby <sebygames@googlemail.com>");
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 150);
    app->draw(*menuString);

    menuString->setCharacterSize(HELP_SUBTITLE_SIZE);
    menuString->setString(CREDITS_GRAPHICS[language]);
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 190);
    app->draw(*menuString);
    menuString->setCharacterSize(HELP_TEXT_SIZE);
    menuString->setString(L"Suiland");
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 220);
    app->draw(*menuString);

    menuString->setCharacterSize(HELP_SUBTITLE_SIZE);
    menuString->setString(CREDITS_SOUND[language]);
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 260);
    app->draw(*menuString);
    menuString->setCharacterSize(HELP_TEXT_SIZE);
    menuString->setString(L"Royalty free sounds");
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 290);
    app->draw(*menuString);

    menuString->setCharacterSize(HELP_SUBTITLE_SIZE);
    menuString->setString(CREDITS_MUSIC[language]);
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 340);
    app->draw(*menuString);
    menuString->setCharacterSize(HELP_TEXT_SIZE);
    menuString->setString(L"Another synthetic tribute to a muted-trumpet player (from Solcarlus)");
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 370);
    app->draw(*menuString);
    menuString->setString(L"Background noise (from Solcarlus)");
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 400);
    app->draw(*menuString);
    menuString->setString(L"Emergency landing (from Solcarlus)");
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 430);
    app->draw(*menuString);
    menuString->setString(L"Inside part 2 (from Nienvox)");
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 460);
    app->draw(*menuString);

    menuString->setCharacterSize(HELP_SUBTITLE_SIZE);
    menuString->setColor(sf::Color(255, 255, 255, 128 + 127 * cos(5 * getGameTime())));
    menuString->setString(CLICK_TO_CONTINUE[language]);
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 510);
    app->draw(*menuString);

    app->draw(sfmlLogoSprite);

    app->display();
}

void GfxEngine::renderHelp()
{
    app->draw(helpBgSprite);

    menuString->setColor(sf::Color::White);
    menuString->setCharacterSize(HELP_TITLE_SIZE);
    menuString->setString(HELP_TITLE[language]);
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, HELP_TITLE_POSITION);
    app->draw(*menuString);

    int yTrans=11;

    normalMiniSprite.setPosition(HELP_LEFT_TITLE, 55 + yTrans);
    app->draw(normalMiniSprite);

    menuString->setCharacterSize(HELP_SMALLTEXT_SIZE);
    menuString->setString(HELP_NORMAL1[language]);
    menuString->setPosition(HELP_LEFT_TEXT, 85 + yTrans);
    app->draw(*menuString);

    menuString->setString(HELP_NORMAL2[language]);
    menuString->setPosition(HELP_LEFT_TEXT, 115 + yTrans);
    app->draw(*menuString);

    motionMiniSprite.setPosition(HELP_LEFT_TITLE, 150 + yTrans);
    app->draw(motionMiniSprite);

    menuString->setString(HELP_MOTION1[language]);
    menuString->setPosition(HELP_LEFT_TEXT, 180 + yTrans);
    app->draw(*menuString);

    menuString->setString(HELP_MOTION2[language]);
    menuString->setPosition(HELP_LEFT_TEXT, 210 + yTrans);
    app->draw(*menuString);

    classicMiniSprite.setPosition(HELP_LEFT_TITLE, 245 + yTrans);
    app->draw(classicMiniSprite);

    menuString->setString(HELP_CLASSIC1[language]);
    menuString->setPosition(HELP_LEFT_TEXT, 275 + yTrans);
    app->draw(*menuString);

    menuString->setString(HELP_CLASSIC2[language]);
    menuString->setPosition(HELP_LEFT_TEXT, 305 + yTrans);
    app->draw(*menuString);

    singleTile.setTextureRect(sf::IntRect(5 * TILE_W, 0, TILE_W, TILE_H));
    singleTile.setPosition(48,350 + yTrans);
    app->draw(singleTile);

    singleTile.setTextureRect(sf::IntRect(6 * TILE_W, 0, TILE_W, TILE_H));
    singleTile.setPosition(HELP_LEFT_ICON,360 + yTrans);
    app->draw(singleTile);

    singleTile.setTextureRect(sf::IntRect(8 * TILE_W, 0, TILE_W, TILE_H));
    singleTile.setPosition(HELP_LEFT_ICON,400 + yTrans);
    app->draw(singleTile);

    singleTile.setTextureRect(sf::IntRect(9 * TILE_W, 0, TILE_W, TILE_H));
    singleTile.setPosition(HELP_LEFT_ICON,440 + yTrans);
    app->draw(singleTile);

    singleTile.setTextureRect(sf::IntRect(10 * TILE_W, 0, TILE_W, TILE_H));
    singleTile.setPosition(HELP_LEFT_ICON,480 + yTrans);
    app->draw(singleTile);

    menuString->setString(HELP_BONUS_COLOR[language]);
    menuString->setPosition(HELP_LEFT_ICON_TEXT, 365 + yTrans);
    app->draw(*menuString);

    menuString->setString(HELP_BONUS_BOMB[language]);
    menuString->setPosition(HELP_LEFT_ICON_TEXT, 405 + yTrans);
    app->draw(*menuString);

    menuString->setString(HELP_BONUS_ARROW[language]);
    menuString->setPosition(HELP_LEFT_ICON_TEXT, 445 + yTrans);
    app->draw(*menuString);

    menuString->setString(HELP_BONUS_SKULL[language]);
    menuString->setPosition(HELP_LEFT_ICON_TEXT, 485 + yTrans);
    app->draw(*menuString);

    menuString->setCharacterSize(HELP_SUBTITLE_SIZE);
    menuString->setColor(sf::Color(255, 255, 255, 128 + 127 * cos(5 * getGameTime())));
    menuString->setString(CLICK_TO_CONTINUE[language]);
    menuString->setPosition((SCREEN_W - menuString->getLocalBounds().width) / 2, 525);
    app->draw(*menuString);

    app->display();
}

void GfxEngine::getKeys(GameInput* gameInput)
{
    // message processing loop
    xRand = 4.0 * (rand() / ((double)RAND_MAX)) - 2;
    yRand = 4.0 * (rand() / ((double)RAND_MAX)) - 2;

    // Process events
    sf::Event Event;
    while (app->pollEvent(Event))
    {
        // Close window : exit
        if (Event.type == sf::Event::Closed)
        {
                gameInput->setQuitKey(true);
        }

        // Touche 'echap' appuyée
        if (Event.type == sf::Event::KeyPressed) {
            if (Event.key.code == sf::Keyboard::Escape)
            {
                gameInput->setEscKey(true);
            }
            else if (Event.key.code == sf::Keyboard::Return)
            {
                gameInput->setEnterKey(true);
            }
            else if (Event.key.code == sf::Keyboard::BackSpace)
            {
                gameInput->setRemoveKey(true);
            }
            else if (Event.key.code >= sf::Keyboard::A && Event.key.code <= sf::Keyboard::Z)
            {
                gameInput->setMiscKey(true);
                gameInput->setCharKey('A' + (Event.key.code - sf::Keyboard::A));
            }
            else if (Event.key.code >= sf::Keyboard::Num0 && Event.key.code <= sf::Keyboard::Num9)
            {
                gameInput->setMiscKey(true);
                gameInput->setCharKey('0' + (Event.key.code - sf::Keyboard::Num0));
            }
            else if (Event.key.code == sf::Keyboard::Space)
            {
                gameInput->setMiscKey(true);
                gameInput->setCharKey('_');
            }
         }

         if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F10)) {
            sf::Image Screen = app->capture();
            Screen.saveToFile("screenshot.jpg");
        }


         if (Event.type == sf::Event::MouseButtonPressed) {
            if (Event.mouseButton.button == sf::Mouse::Left)
                gameInput->setMouseLeftClick(true);
            else if (Event.mouseButton.button == sf::Mouse::Right)
                gameInput->setMouseRightClick(true);
            gameInput->setXMouse(oldXMouse);
            gameInput->setYMouse(oldYMouse);
         }

        if (Event.type == sf::Event::MouseMoved) {
            gameInput->setMouseMotion(true);
            sf::Vector2f vec=app->mapPixelToCoords(sf::Vector2i(Event.mouseMove.x, Event.mouseMove.y));
            oldXMouse=(int)(vec.x);
            oldYMouse=(int)(vec.y);
            gameInput->setXMouse(oldXMouse);
            gameInput->setYMouse(oldYMouse);
         }
    }
}

void GfxEngine::updateEffects()
{
    EM->Animate(getGameTime());
}

void GfxEngine::animateUp()
{
    upAnimation=true;
    upClock.restart();
}


void GfxEngine::createGameOverEffect(GameModel* gameModel)
{
    if (!effectsEnabled) return;

    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            if (gameModel->getGameGrid(i, j) > 0)
            {
                GameEntity* entity=new GameEntity(  OFFSET_X + i * TILE_W + TILE_W / 2,
                                                    OFFSET_Y + j * TILE_H + TILE_H / 2, 0);
                //GameEntity* entity=new GameEntity(50, 50, 0);
                entity->velocity.x=300.0 * (rand() / ((double)RAND_MAX)) - 150;
                entity->velocity.y=-300.0 * (rand() / ((double)RAND_MAX)) - 50;
                entity->gravity.y=550.0f;
                entity->lifetime=4.0f;
                entity->spin=800.0 * (rand() / ((double)RAND_MAX)) - 400;
                sf::Sprite sprite;
                sprite.setTexture(tiles);
                sprite.setOrigin(TILE_W / 2, TILE_H / 2);
                int mTile=gameModel->getGameGrid(i, j) - 1;
                sprite.setTextureRect(sf::IntRect(mTile * TILE_W, 0, TILE_W, TILE_H));
                //sprite.setTextureRect(sf::IntRect(0,0,80,0));
                entity->sprite=sprite;
                EM->Add(entity);
            }
        }
    }
}

void GfxEngine::createRemoveEffect(GameModel* gameModel)
{
    if (gameModel->getRemoveType() == GameModel::removeTypeColor)
    {
        playSound(soundColor);
    }

    if (!effectsEnabled) return;

    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            if (gameModel->getGameGrid(i, j) > 0 && gameModel->getZoneGrid(i,j))
            {
                GameEntity* entity=new GameEntity(  OFFSET_X + i * TILE_W + TILE_W / 2,
                                                    OFFSET_Y+j*TILE_H + TILE_H / 2,
                                                    GameEntity::typeSprite);

                entity->velocity.x=30.0 * (rand()/((double)RAND_MAX)) - 15;
                entity->velocity.y=28.0 * (rand()/((double)RAND_MAX));
                entity->gravity.y=550.0f;
                entity->lifetime=4.0f;
                entity->spin=600.0 * (rand()/((double)RAND_MAX)) - 300;
                sf::Sprite sprite;
                sprite.setTexture(tiles);
                sprite.setOrigin(TILE_W/2, TILE_H/2);
                sprite.setPosition(-100, -100);
                int mTile=gameModel->getGameGrid(i, j)-1;
                sprite.setTextureRect(sf::IntRect(mTile * TILE_W, 0, TILE_W, TILE_H));
                //sprite.setTextureRect(sf::IntRect(0,0,80,0));
                sprite.setColor(sf::Color(255, 255, 255, 128));
                entity->sprite=sprite;
                EM->Add(entity);

                if (gameModel->getRemoveType() == GameModel::removeTypeBomb
                        && gameModel->getGameGrid(i, j) == GameModel::tileBomb)
                {
                    createBombEffect(i,j);
                }
            }
        }
    }
    if (gameModel->getRemoveType()==GameModel::removeTypeArrows) createLightningEffect(gameModel);
}

void GfxEngine::createBombEffect(int x, int y)
{
    playSound(soundExplode);

    if (!effectsEnabled) return;

    int xPos=OFFSET_X + x * TILE_W + TILE_W / 2;
    int yPos=OFFSET_Y + y * TILE_H + TILE_H / 2;

    GameEntity* entity=new GameEntityBoom(  xPos, yPos, GameEntity::typeSprite);
    entity->lifetime=0.6;
    sf::Sprite sprite;
    sprite.setTexture(boomImage);
    sprite.setOrigin(TILE_W / 2, TILE_H / 2);
    sprite.setPosition(xPos, yPos);
    sprite.setScale(5.0f, 5.0f);
    sprite.setColor(sf::Color(255, 255, 255, 164));
    entity->sprite=sprite;
    EM->Add(entity);
}

void GfxEngine::createLightningEffect(GameModel* gameModel)
{
    playSound(soundThunder);

    if (!effectsEnabled) return;

    int i, j;

    for (i=0; i < GRID_W; i++)
    {
        bool isLightning=false;
        for (j=0; j < GRID_H; j++)
        {
            if (gameModel->getGameGrid(i, j) == GameModel::tileArrows && gameModel->getZoneGrid(i,j)) {
                isLightning=true;
            }
        }
        if (isLightning) {
                GameEntityLightning* entity=new GameEntityLightning(OFFSET_X + i * TILE_W, OFFSET_Y, GameEntity::typeSprite);
                entity->lifetime=0.6f;
                entity->SetTexture(lightningVImage);
                EM->Add(entity);
        }
    }

    for (j=0; j<GRID_H; j++) {
        bool isLightning=false;
        for (i=0; i<GRID_W; i++) {
            if (gameModel->getGameGrid(i, j) == GameModel::tileArrows && gameModel->getZoneGrid(i,j)) {
                isLightning=true;
            }
        }
        if (isLightning) {
                GameEntityLightning* entity=new GameEntityLightning(OFFSET_X, OFFSET_Y + j * TILE_H, GameEntity::typeSprite);
                entity->lifetime=0.6f;
                entity->SetTexture(lightningHImage);
                EM->Add(entity);
        }
    }

    GameEntity* entity=new GameEntityWhiteScreen(OFFSET_X, OFFSET_Y, GameEntity::typeSprite);
    entity->lifetime=0.3f;
    sf::Sprite sprite;
    sprite.setTexture(whiteScreenImage);
    sprite.setColor(sf::Color(255, 255, 255, 255));
    entity->sprite=sprite;
    EM->Add(entity);
}

void GfxEngine::createScoreTextEffect(int score)
{
    if (!effectsEnabled) return;

    GameEntity* entity=new GameEntityFlyingText( oldXMouse - 10, oldYMouse - 10, GameEntity::typeText);
    entity->lifetime=1.3;
    entity->gravity.y=-300.0f;

    std::ostringstream scoreStream;
    scoreStream << "+" << score;

    sf::Text myString;
    myString.setString(scoreStream.str());
    myString.setFont(font);
    myString.setCharacterSize(25);
    myString.setPosition(entity->x, entity->y);;
    entity->text=myString;
    EM->Add(entity);
}

void GfxEngine::createClassicBonusEffect(int bonus)
{
    playSound(soundColor);

    GameEntity* entity=new GameEntityFlyingText( 200, 500, GameEntity::typeText);
    entity->lifetime=3.5;
    entity->gravity.y=-100.0f;

    std::ostringstream scoreStream;
    if (bonus == 0)
        scoreStream << "Grid clear ! +" << bonusScore[bonus] << " points !";
    else if (bonus == 1)
        scoreStream << bonus << " stone left : +" << bonusScore[bonus] << " points !";
    else
        scoreStream << bonus << " stones left : +" << bonusScore[bonus] << " points !";
    sf::Text myString;
    myString.setString(scoreStream.str());
    myString.setFont(font);
    myString.setCharacterSize(40);
    myString.setPosition(800,600);
    entity->text=myString;
    EM->Add(entity);
}

void GfxEngine::playSound(int n)
{
    if (!soundEnabled) return;
    sound[n].play();
}

void GfxEngine::playMusic(int n)
{
    if (!musicEnabled) return;

    stopMusic();
    music.setLoop(true);

    switch(n)
    {
        case musicIntro:
        {
            if (!music.openFromFile(getDataFile(musicIntroFile)))
            {
                std::cout << "Error loading " << musicIntroFile << std::endl;
            }
            break;
        }
        case musicGame:
        {
            if (!music.openFromFile(getDataFile(musicGameFile)))
            {
                std::cout << "Error loading " << musicGameFile << std::endl;
            }
            break;
        }
        case musicClassic:
        {
            if (!music.openFromFile(getDataFile(musicClassicFile)))
            {
                std::cout << "Error loading " << musicClassicFile << std::endl;
            }
            break;
        }
        case musicGameOver:
        {
            if (!music.openFromFile(getDataFile(musicGameOverFile)))
            {
                std::cout << "Error loading " << musicGameOverFile << std::endl;
            }
            break;
        }
    }

    music.play();
}

void GfxEngine::stopMusic()
{
    music.stop();
}

void GfxEngine::initScores()
{


    sf::Color color1(240, 200, 90, 255);
    sf::Color color2(0, 200, 200, 255);
    for (int i=0; i < 10; i++)
    {
        //delete (textPos[i]);
        textPos[i]=new sf::Text(PODIUM_STRING[i], font, STRING_SIZE_HISCORES);
        textPos[i]->setPosition(425, 142 + i * 35);
        textPos[i]->setColor(i % 2 == 0 ? color1 : color2);

        //delete (textNames[i]);
        textNames[i]=new sf::Text("bob", font, STRING_SIZE_HISCORES);
        textNames[i]->setPosition(470, 142 + i * 35);
        textNames[i]->setColor(i % 2 == 0 ? color1 : color2);

        // delete (textScores[i]);
        textScores[i]=new sf::Text("bob", font, STRING_SIZE_HISCORES);
        textScores[i]->setPosition(680, 142 + i * 35);
        textScores[i]->setColor(i % 2 == 0 ? color1 : color2);
    }
}


void GfxEngine::updateScores(GameScores* gameScores, int gameType)
{
    switch (gameType)
    {
        case GameModel::gameTypeNormal: modeScoreSprite.setTexture(modeNormalHiscores, true); break;
        case GameModel::gameTypeMotion: modeScoreSprite.setTexture(modeMotionHiscores, true); break;
        case GameModel::gameTypeClassic: modeScoreSprite.setTexture(modeClassicHiscores, true); break;
    }
    modeScoreSprite.setPosition(597 - modeScoreSprite.getTextureRect().width / 2, 58);

    for (int i=0; i < 10; i++)
    {
        switch (gameType)
        {
            case GameModel::gameTypeNormal:
            {
            textNames[i]->setString(gameScores->getNormalScore(i).name);

            std::ostringstream scoreStream;

            scoreStream << gameScores->getNormalScore(i).score;
            textScores[i]->setString(scoreStream.str());

            break;
            }

            case GameModel::gameTypeMotion:
            {
            textNames[i]->setString(gameScores->getMotionScore(i).name);

            std::ostringstream scoreStream;

            scoreStream << gameScores->getMotionScore(i).score;
            textScores[i]->setString(scoreStream.str());

            break;
            }

            case GameModel::gameTypeClassic:
            {
            textNames[i]->setString(gameScores->getClassicScore(i).name);

            std::ostringstream scoreStream;

            scoreStream << gameScores->getClassicScore(i).score;
            textScores[i]->setString(scoreStream.str());

            break;
            }
        }
    }
}

void GfxEngine::closeApp()
{
    app->close();
}



int GfxEngine::getMenuMouseOver(int x, int y, Menu* myMenu)
{
    if (myMenu == NULL) return -1;

    int CORR=10;

    if (x < MENU_POSX + CORR) return -1;
    if (x > MENU_POSX + MENU_W - CORR) return -1;

    int n=myMenu->getNEntries();

    int yTop=(SCREEN_H - (MENU_TOP_H + MENU_BOTTOM_H + n * MENU_BUTTON_H)) / 2;

    if (y < yTop + MENU_TOP_H) return -1;
    if (y >= yTop + MENU_TOP_H + n * MENU_BUTTON_H) return - 1;

    for (int i=0; i<n; i++)
    {
        int y0=yTop + MENU_TOP_H + i * MENU_BUTTON_H;
        int yf=yTop + MENU_TOP_H + (i + 1) * MENU_BUTTON_H;

        if ( y > y0 && y < yf ) return i;
    }
    return -1;
}

void GfxEngine::setSoundVolume(int soundVolume)
{
    this->soundVolume=soundVolume;
    for (int i=0; i < NB_SOUNDS; i++)
        sound[i].setVolume((float)soundVolume);
}

void GfxEngine::setMusicVolume(int musicVolume)
{
    this->musicVolume=musicVolume;
    music.setVolume((float)musicVolume);
}
