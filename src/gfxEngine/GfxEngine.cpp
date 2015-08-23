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
    skin=skinGreen;
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
    delay=-10.0f;
    bestScoresDelay=-10.0f;
    consoleDelay=0.0f;
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
    app->SetFramerateLimit(60);
    //app = new sf::RenderWindow(sf::VideoMode(800, 600), title, sf::Style::Fullscreen);
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
    // load images;

    intro.LoadFromFile("data/media/menuBg.png");
    introSprite.SetImage(intro);
    //introSprite.SetX(200);
    //introSprite.SetY(50);

    loadSkin();
    loadTileSet();
    bgSprite.SetImage(bg);

    gameOver.LoadFromFile("data/media/gameover.png");
    gameOverSprite.SetImage(gameOver);
    gameOverSprite.SetX(300);
    gameOverSprite.SetY(225);


    levelSprite.SetImage(levelImage);
    levelSprite.SetX(679);
    levelSprite.SetY(379);

    modeIcones.LoadFromFile("data/media/modeIcones.png");
    modeSprite.SetImage(modeIcones);
    modeSprite.SetX(675);
    modeSprite.SetY(450);

    modeNormal.LoadFromFile("data/media/modeNormal.png");
    modeNormalOver.LoadFromFile("data/media/modeNormalOver.png");
    normalSprite.SetImage(modeNormal);
    normalSprite.SetX(MENU_NORMAL_X);
    normalSprite.SetY(MENU_NORMAL_Y);
    modeNormalMini.LoadFromFile("data/media/modeNormalMini.png");
    normalMiniSprite.SetImage(modeNormalMini);
    modeNormalHiscores.LoadFromFile("data/media/modeNormalHiscores.png");

    modeMotion.LoadFromFile("data/media/modeMotion.png");
    modeMotionOver.LoadFromFile("data/media/modeMotionOver.png");
    motionSprite.SetImage(modeMotion);
    motionSprite.SetX(MENU_MOTION_X);
    motionSprite.SetY(MENU_MOTION_Y);
    modeMotionMini.LoadFromFile("data/media/modeMotionMini.png");
    motionMiniSprite.SetImage(modeMotionMini);
    modeMotionHiscores.LoadFromFile("data/media/modeMotionHiscores.png");

    modeClassic.LoadFromFile("data/media/modeClassic.png");
    modeClassicOver.LoadFromFile("data/media/modeClassicOver.png");
    classicSprite.SetImage(modeClassic);
    classicSprite.SetX(MENU_CLASSIC_X);
    classicSprite.SetY(MENU_CLASSIC_Y);
    modeClassicMini.LoadFromFile("data/media/modeClassicMini.png");
    classicMiniSprite.SetImage(modeClassicMini);
    modeClassicHiscores.LoadFromFile("data/media/modeClassicHiscores.png");

    buttonHelpImage.LoadFromFile("data/media/buttonHelp.png");
    buttonHelpSprite.SetImage(buttonHelpImage);
    buttonHelpSprite.SetX(MENU_HELP_X);
    buttonHelpSprite.SetY(MENU_HELP_Y);

    buttonCreditsImage.LoadFromFile("data/media/buttonCredits.png");
    buttonCreditsSprite.SetImage(buttonCreditsImage);
    buttonCreditsSprite.SetX(MENU_CREDITS_X);
    buttonCreditsSprite.SetY(MENU_CREDITS_Y);

    buttonOptionsImage.LoadFromFile("data/media/buttonOptions.png");
    buttonOptionsSprite.SetImage(buttonOptionsImage);
    buttonOptionsSprite.SetX(MENU_OPTIONS_X);
    buttonOptionsSprite.SetY(MENU_OPTIONS_Y);

    buttonQuitImage.LoadFromFile("data/media/buttonQuit.png");
    buttonQuitSprite.SetImage(buttonQuitImage);
    buttonQuitSprite.SetX(MENU_QUIT_X);
    buttonQuitSprite.SetY(MENU_QUIT_Y);

    miniButtonSprite.SetImage(buttonHelpImage);
    miniButtonSprite.SetSubRect(sf::IntRect(0, 0, buttonHelpImage.GetWidth() / 2, buttonHelpImage.GetHeight()));
    miniButtonSprite.Scale(0.4f, 0.4f);

    cancelButtonImage.LoadFromFile("data/media/cancel64.png");
    cancelButtonSprite.SetImage(cancelButtonImage);
    cancelButtonSprite.SetPosition(CANCEL_BUTTON_X0, CANCEL_BUTTON_Y0);

    menuButtonSprite.SetImage(menuButtonImage);
    menuButtonSprite.SetX(GAMEMENU_BUTTON_X0);
    menuButtonSprite.SetY(GAMEMENU_BUTTON_Y0);

    blackFootImage.Create(800, 40);
    blackFootSprite.SetImage(blackFootImage);
    blackFootSprite.SetY(547);

    whiteScreenImage.Create(600, 480, sf::Color(255, 255, 255, 255));

    helpBg.LoadFromFile("data/media/helpBg.png");
    helpBgSprite.SetImage(helpBg);

    creditsBg.LoadFromFile("data/media/creditsBg.png");
    creditsBgSprite.SetImage(creditsBg);

    sfmlLogoImage.LoadFromFile("data/media/logoSfml.jpg");
    sfmlLogoSprite.SetImage(sfmlLogoImage);
    sfmlLogoSprite.SetPosition(550, 180);

    blackScreen.Create(800, 600, sf::Color(0, 0, 0, 192));
    blackScreenSprite.SetImage(blackScreen);

    menuImage.LoadFromFile("data/media/menu.png");
    menuSpriteTop.SetImage(menuImage);
    menuSpriteTop.SetSubRect(sf::IntRect(MENU_TOP_X0, MENU_TOP_Y0, MENU_TOP_XF, MENU_TOP_YF));
    menuSpriteButton.SetImage(menuImage);
    menuSpriteButton.SetSubRect(sf::IntRect(MENU_BUTTON_X0, MENU_BUTTON_Y0, MENU_BUTTON_XF, MENU_BUTTON_YF));
    menuSpriteButtonOver.SetImage(menuImage);
    menuSpriteButtonOver.SetSubRect(sf::IntRect(MENU_BUTTON_OVER_X0, MENU_BUTTON_OVER_Y0, MENU_BUTTON_OVER_XF, MENU_BUTTON_OVER_YF));
    menuSpriteButtonL.SetImage(menuImage);
    menuSpriteButtonL.SetSubRect(sf::IntRect(MENU_BUTTONL_X0, MENU_BUTTONL_Y0, MENU_BUTTONL_XF, MENU_BUTTONL_YF));
    menuSpriteButtonR.SetImage(menuImage);
    menuSpriteButtonR.SetSubRect(sf::IntRect(MENU_BUTTONR_X0, MENU_BUTTONR_Y0, MENU_BUTTONR_XF, MENU_BUTTONR_YF));

    menuSpriteButtonLOver.SetImage(menuImage);
    menuSpriteButtonLOver.SetSubRect(sf::IntRect(MENU_BUTTONL_OVER_X0, MENU_BUTTONL_OVER_Y0, MENU_BUTTONL_OVER_XF, MENU_BUTTONL_OVER_YF));
    menuSpriteButtonROver.SetImage(menuImage);
    menuSpriteButtonROver.SetSubRect(sf::IntRect(MENU_BUTTONR_OVER_X0, MENU_BUTTONR_OVER_Y0, MENU_BUTTONR_OVER_XF, MENU_BUTTONR_OVER_YF));

    menuSpriteBottom.SetImage(menuImage);
    menuSpriteBottom.SetSubRect(sf::IntRect(MENU_BOTTOM_X0, MENU_BOTTOM_Y0, MENU_BOTTOM_XF, MENU_BOTTOM_YF));
    menuSpriteHiScore1.SetImage(menuImage);
    menuSpriteHiScore1.SetSubRect(sf::IntRect(MENU_SCORE1_X0, MENU_SCORE1_Y0, MENU_SCORE1_XF, MENU_SCORE1_YF));
    menuSpriteHiScore2.SetImage(menuImage);
    menuSpriteHiScore2.SetSubRect(sf::IntRect(MENU_SCORE2_X0, MENU_SCORE2_Y0, MENU_SCORE2_XF, MENU_SCORE2_YF));

    menuSpriteSpace.SetImage(menuImage);
    menuSpriteSpace.SetSubRect(sf::IntRect(0, 40, MENU_W, 40 + MENU_SPACE_H));

    //sf::Sprite menuSpriteTop;
    //sf::Sprite menuSpriteBottom;
    //sf::Sprite menuSpriteButton;
    //sf::Sprite menuSpriteButtonOver;

    dangerImage.LoadFromFile("data/media/danger.png");
    int i=0;
    for (i=0; i<GRID_W; i++)
    {
        dangerSprite[i].SetImage(dangerImage);
        dangerSprite[i].SetX(i * TILE_W + OFFSET_X + (TILE_W - DANGER_W) / 2);
        dangerSprite[i].SetY(OFFSET_Y-TILE_H);
    }
    singleTile.SetImage(tiles);
    singleTile.SetScale(0.8f, 0.8f);

    for (i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            tile[i][j].SetImage(tiles);
            tile[i][j].SetX(i * TILE_W + OFFSET_X);
            tile[i][j].SetY(j * TILE_H + OFFSET_Y);
        }
        nextRowTile[i].SetImage(nextTiles);
        //nextRowTile[i].SetX(i * TILE_W + OFFSET_X + TILE_W / 4);
        //nextRowTile[i].SetY(GRID_H * TILE_H + OFFSET_Y + TILE_H / 4);
        nextRowTile[i].SetX(i * TILE_W + OFFSET_X + 7);
        nextRowTile[i].SetY(GRID_H * TILE_H + OFFSET_Y + 26);
        //nextRowTile[i].SetScaleX(0.5f);
        //nextRowTile[i].SetScaleY(0.5f);
    }

    holeSprite.SetImage(holeImage);
    holeSprite.SetY(GRID_H * TILE_H + OFFSET_Y + 7);

    effectsSprite.SetImage(effectsImage);

    boomImage.LoadFromFile("data/media/boom40.png");

    lightningHImage.LoadFromFile("data/media/LightningH.png");
    lightningVImage.LoadFromFile("data/media/LightningV.png");

    font.LoadFromFile("data/media/AlteHaasGroteskBold.ttf");


    textHighScore=new sf::String("0", font, STRING_SIZE_SCORE);
    //textHighScore->SetPosition(665,186);
    textHighScore->SetPosition(665,177);

    textScore=new sf::String("0", font, STRING_SIZE_SCORE);
    //textScore->SetPosition(665,280);
    textScore->SetPosition(665,282);

    textValue=new sf::String("0", font, STRING_SIZE_SCORE);
    //textValue->SetPosition(665,312);
    textValue->SetPosition(665,317);
    textValue->SetColor(sf::Color(128, 128, 255, 255));

    textLevel=new sf::String("0", font, STRING_SIZE_SCORE);
    //textLevel->SetPosition(740,390);
    textLevel->SetPosition(706,424);

    textScoreName=new sf::String("", font, 30);
    textScoreName->SetPosition(125,551);

    textMode=new sf::String("NORMAL", font, 22);
    textMode->SetPosition(660,525);
    textMode->SetColor(sf::Color::Black);

    normalDescription[0]=new sf::String(
        "Click zones with 3 or more stones of the same color to make them disappear.", font, STRING_SIZE_DESCRIPTION);
    normalDescription[0]->SetPosition(10,542);
    normalDescription[0]->SetColor(sf::Color::Black);

    normalDescription[1]=new sf::String(
        "Don't let the stones rise to the top ! (Turn based)", font, STRING_SIZE_DESCRIPTION);
    normalDescription[1]->SetPosition(10,567);
    normalDescription[1]->SetColor(sf::Color::Black);

    motionDescription[0]=new sf::String(
        "Click zones with 3 or more stones of the same color to make them disappear.", font, STRING_SIZE_DESCRIPTION);
    motionDescription[0]->SetPosition(10,542);
    motionDescription[0]->SetColor(sf::Color::Black);

    motionDescription[1]=new sf::String(
        "Don't let the stones rise to the top ! (Real time)", font, STRING_SIZE_DESCRIPTION);
    motionDescription[1]->SetPosition(10,567);
    motionDescription[1]->SetColor(sf::Color::Black);

    classicDescription[0]=new sf::String(
        "Click zones with 2 or more stones of the same color to make them disappear.", font, STRING_SIZE_DESCRIPTION);
    classicDescription[0]->SetPosition(10,542);
    classicDescription[0]->SetColor(sf::Color::Black);

    classicDescription[1]=new sf::String(
        "Try to clear the entire grid !", font, STRING_SIZE_DESCRIPTION);
    classicDescription[1]->SetPosition(10,567);
    classicDescription[1]->SetColor(sf::Color::Black);

    menuString=new sf::String("", font, STRING_SIZE_MENU_TITLE);
}

void GfxEngine::loadSkin()
{
    string dirBase="data/media/skins/";
    string dirBaseStd="data/media/skins/blue/";

    switch (skin)
    {
        case skinBlue: dirBase += "blue/"; break;
        case skinGreen: dirBase += "green/"; break;
        case skinRosa: dirBase += "rosa/"; break;
        case skinBrown: dirBase += "brown/"; break;
    }

    if (!bg.LoadFromFile(dirBase + "bg.png"))
    {
        bg.LoadFromFile(dirBaseStd + "bg.png");
    }

    if (!holeImage.LoadFromFile(dirBase + "hole40.png"))
    {
        holeImage.LoadFromFile(dirBaseStd + "hole40.png");
    }

    if (!levelImage.LoadFromFile(dirBase + "level.png"))
    {
        levelImage.LoadFromFile(dirBaseStd + "level.png");
    }

    if (!menuButtonImage.LoadFromFile(dirBase + "menu_button.png"))
    {
        menuButtonImage.LoadFromFile(dirBaseStd + "menu_button.png");
    }
}

void GfxEngine::loadTileSet()
{
    string dirBase="data/media/tilesets/";
    string dirBaseStd="data/media/tilesets/pastel/";

    switch (tileSet)
    {
        case tsPastel: dirBase += "pastel/"; break;
        case tsMarble: dirBase += "marble/"; break;
        //case skinRosa: dirBase += "rosa/"; break;
        case tsGems: dirBase += "gems/"; break;
    }


    if (!tiles.LoadFromFile(dirBase + "tiles.png"))
    {
        tiles.LoadFromFile(dirBaseStd + "tiles.png");
    }

    if (!nextTiles.LoadFromFile(dirBase + "nexttiles.png"))
    {
        nextTiles.LoadFromFile(dirBaseStd + "nexttiles.png");
    }

    if (!effectsImage.LoadFromFile(dirBase + "effects.png"))
    {
        effectsImage.LoadFromFile(dirBaseStd + "effects.png");
    }
}

void GfxEngine::loadSounds()
{
    // load audio;
    brickBuffer.LoadFromFile("data/media/wav/brick.wav");
    sound[soundBrick].SetBuffer(brickBuffer);

    explosionBuffer.LoadFromFile("data/media/wav/explosion.wav");
    sound[soundExplode].SetBuffer(explosionBuffer);

    thunderBuffer.LoadFromFile("data/media/wav/thunder.wav");
    sound[soundThunder].SetBuffer(thunderBuffer);

    colorBuffer.LoadFromFile("data/media/wav/color.wav");
    sound[soundColor].SetBuffer(colorBuffer);

    dangerBuffer.LoadFromFile("data/media/wav/danger.wav");
    sound[soundDanger].SetBuffer(dangerBuffer);

    menuOverBuffer.LoadFromFile("data/media/wav/menu_over.wav");
    sound[soundMenuOver].SetBuffer(menuOverBuffer);

    menuClickBuffer.LoadFromFile("data/media/wav/menu_click.wav");
    sound[soundMenuClick].SetBuffer(menuClickBuffer);

    for (int i=0; i < NB_SOUNDS; i++)
        sound[i].SetVolume((float)soundVolume);

    music.SetVolume((float)musicVolume);
}

void GfxEngine::renderIntro(int choice, Menu* myMenu)
{
    //App->Draw(bgSprite);

    app->Draw(introSprite);

    if (introMouseOver == mouseOverNormal)
    {
        normalSprite.SetImage(modeNormalOver);
    }
    else
    {
        normalSprite.SetImage(modeNormal);
    }

    if (introMouseOver == mouseOverMotion)
    {
        motionSprite.SetImage(modeMotionOver);
    }
    else
    {
        motionSprite.SetImage(modeMotion);
    }

    if (introMouseOver == mouseOverClassic)
    {
        classicSprite.SetImage(modeClassicOver);
    }
    else
    {
        classicSprite.SetImage(modeClassic);
    }

    if (introMouseOver == mouseOverHelp)
    {
        buttonHelpSprite.SetSubRect(sf::IntRect(MENU_ICON_W, 0, MENU_ICON_W * 2, MENU_ICON_H));
    }
    else
    {
        buttonHelpSprite.SetSubRect(sf::IntRect(0, 0, MENU_ICON_W, MENU_ICON_H));
    }

    if (introMouseOver == mouseOverOptions)
    {
        buttonOptionsSprite.SetSubRect(sf::IntRect(MENU_ICON_W, 0, MENU_ICON_W * 2, MENU_ICON_H));
    }
    else
    {
        buttonOptionsSprite.SetSubRect(sf::IntRect(0, 0, MENU_ICON_W, MENU_ICON_H));
    }

    if (introMouseOver == mouseOverCredits)
    {
        buttonCreditsSprite.SetSubRect(sf::IntRect(MENU_ICON_W, 0, MENU_ICON_W * 2, MENU_ICON_H));
    }
    else
    {
        buttonCreditsSprite.SetSubRect(sf::IntRect(0, 0, MENU_ICON_W, MENU_ICON_H));
    }

    if (introMouseOver == mouseOverQuit)
    {
        buttonQuitSprite.SetSubRect(sf::IntRect(MENU_ICON_W, 0, MENU_ICON_W * 2, MENU_ICON_H));
    }
    else
    {
        buttonQuitSprite.SetSubRect(sf::IntRect(0, 0, MENU_ICON_W, MENU_ICON_H));
    }

    app->Draw(normalSprite);
    app->Draw(motionSprite);
    app->Draw(classicSprite);

    app->Draw(buttonHelpSprite);
    app->Draw(buttonOptionsSprite);
    app->Draw(buttonCreditsSprite);
    app->Draw(buttonQuitSprite);

    renderConsole();
    renderBestScores();
    renderMenu(myMenu);


    app->Display();
}

void GfxEngine::renderConsole()
{
    consoleDelay += app->GetFrameTime();
    if (consoleDelay > CONSOLE_NEXT_LINE_DELAY)
    {
        consoleDelay=0.0f;
        consoleIndex++;
        if (consoleIndex >= CONSOLE_MAX_LINES) consoleIndex=0;
    }

    menuString->SetColor(sf::Color::White);
    menuString->SetSize(CONSOLE_STRING_SIZE);

    int first=consoleIndex-2;
    if (first < 0) first=0;

    for (int i=first; i <= consoleIndex; i++)
    {

        if (i == consoleIndex && consoleDelay < CONSOLE_LINE_DELAY)
        {
            int strSize=CONSOLE[i][language].length();
            int n=strSize * consoleDelay/CONSOLE_LINE_DELAY;
            menuString->SetText(CONSOLE[i][language].substr(0, n));
        }
        else
        {
            menuString->SetText(CONSOLE[i][language]);
        }

        int xTrans=0;
        if (i >= CONSOLE_HELP && i <= CONSOLE_QUIT)
        {
            xTrans=35;
            switch (i)
            {
                case (CONSOLE_HELP): miniButtonSprite.SetImage(buttonHelpImage); break;
                case (CONSOLE_OPTIONS): miniButtonSprite.SetImage(buttonOptionsImage); break;
                case (CONSOLE_CREDITS): miniButtonSprite.SetImage(buttonCreditsImage); break;
                case (CONSOLE_QUIT): miniButtonSprite.SetImage(buttonQuitImage); break;
            }
            miniButtonSprite.SetPosition(CONSOLE_X, CONSOLE_Y0 + (i - first) * CONSOLE_LINE_H);
            app->Draw(miniButtonSprite);
        }

        menuString->SetPosition(CONSOLE_X + xTrans, CONSOLE_Y0 + (i - first) * CONSOLE_LINE_H);
        app->Draw(*menuString);
    }
   //menuString->SetPosition(CONSOLE_X, CONSOLE_Y1);
    //menuString->SetText(CONSOLE[1][language]);
    //app->Draw(*menuString);

    //menuString->SetPosition(CONSOLE_X, CONSOLE_Y2);
    //menuString->SetText(CONSOLE[2][language]);
    //app->Draw(*menuString);
}

void GfxEngine::renderGameOver(GameModel* gameModel, GameScores* gameScores)
{
    app->Draw(bgSprite);
    if (gameModel->getGameType() == GameModel::gameTypeClassic || !effectsEnabled)
    {
        renderGrid(gameModel);
    }

    //app->Draw(gameOverSprite);



    renderEffects();

    renderGameOverBestScores(gameScores, gameModel->getGameType());
    renderScores(gameModel);

    menuButtonSprite.SetSubRect(sf::IntRect(0, 0, GAMEMENU_BUTTON_W, GAMEMENU_BUTTON_H));
    app->Draw(menuButtonSprite);

    app->Display();
}

void GfxEngine::renderGameOverAnim(GameModel* gameModel)
{
    app->Draw(bgSprite);

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

    menuButtonSprite.SetSubRect(sf::IntRect(0, 0, GAMEMENU_BUTTON_W, GAMEMENU_BUTTON_H));
    app->Draw(menuButtonSprite);

    app->Draw(gameOverSprite);

    app->Display();
}

void GfxEngine::renderGame(GameModel* gameModel, Menu* myMenu)
{
    app->Draw(bgSprite);

    renderGrid(gameModel);

    if (gameModel->getGameType() != GameModel::gameTypeClassic && !gameModel->getGameOver())
    {
        renderNextRow(gameModel);
        renderDanger(gameModel);
    }

    renderModeIcone(gameModel->getGameType());  // NEW DESIGN

    renderEffects();
    renderScores(gameModel);

    menuButtonSprite.SetSubRect(sf::IntRect(0, mouseOnMenuButton ? GAMEMENU_BUTTON_H : 0,
                                    GAMEMENU_BUTTON_W, mouseOnMenuButton ? GAMEMENU_BUTTON_H * 2 : GAMEMENU_BUTTON_H));
    app->Draw(menuButtonSprite);

    if (gameModel->getGameType() == GameModel::gameTypeClassic)
    {
        if (gameModel->getCanCancel())
        {
            if (mouseOnCancelButton)
                cancelButtonSprite.SetSubRect(sf::IntRect(CANCEL_BUTTON_W, 0, 2 * CANCEL_BUTTON_W, CANCEL_BUTTON_H));
            else
                cancelButtonSprite.SetSubRect(sf::IntRect(0, 0, CANCEL_BUTTON_W, CANCEL_BUTTON_H));
            cancelButtonSprite.SetColor(sf::Color(255, 255, 255, 255));
            app->Draw(cancelButtonSprite);
        }
        else
        {
            cancelButtonSprite.SetSubRect(sf::IntRect(0, 0, CANCEL_BUTTON_W, CANCEL_BUTTON_H));
            cancelButtonSprite.SetColor(sf::Color(255, 255, 255, 80));
            app->Draw(cancelButtonSprite);
        }
    }

    renderMenu(myMenu);

    app->Display();
}

void GfxEngine::renderGameClicking(GameModel* gameModel)
{
    app->Draw(bgSprite);

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

    app->Display();
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
            normalMiniSprite.SetPosition((714 - normalMiniSprite.GetSubRect().GetWidth() / 2), 482);
            app->Draw(normalMiniSprite);
            break;
        }
        case GameModel::gameTypeMotion:
        {
            motionMiniSprite.SetPosition((714 - motionMiniSprite.GetSubRect().GetWidth() / 2), 482);
            app->Draw(motionMiniSprite);
            break;
        }
        case GameModel::gameTypeClassic:
        {
            classicMiniSprite.SetPosition((714 - classicMiniSprite.GetSubRect().GetWidth() / 2), 482);
            app->Draw(classicMiniSprite);
            break;
        }
    }


    /*
    modeSprite.SetSubRect(sf::IntRect(mode * 77, 0, mode * 77 + 77, 73));


    app->Draw(modeSprite);
    app->Draw(*textMode);
    */
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
        holeSprite.SetX(i * TILE_W + OFFSET_X);

        app->Draw(holeSprite);

        if (i < x)
        {
            int mTile=gameModel->getNextRow(i) - 1.;
            nextRowTile[i].SetSubRect(sf::IntRect(mTile * 28, 0, mTile * 28 + 28, 27));
            app->Draw(nextRowTile[i]);
        }
    }
}

void GfxEngine::renderTile(int x, int y, int mTile, float xTrans, float yTrans)
{
    int scaleY=TILE_H;
    int xPos=x * TILE_W + OFFSET_X + xTrans;
    int yPos=y * TILE_H + OFFSET_Y + yTrans;

    if (delay > 0.0f)
    {
        float fade=delay / upDelay;

        yTrans+=OFFSET_Y * fade;
        yPos=y * TILE_H + OFFSET_Y + yTrans;

        if (y == GRID_H - 1)
        {
            scaleY=TILE_H * (1.0f - fade);
            scaleY=(TILE_H * GRID_H + OFFSET_Y) - yPos;
        }
    }
    tile[x][y].SetX(xPos);
    tile[x][y].SetY(yPos);

    tile[x][y].SetSubRect(sf::IntRect(mTile * TILE_W, 0, mTile * TILE_W + TILE_W, scaleY));
    app->Draw(tile[x][y]);

    if (!effectsEnabled) return;

    mTile++;
    if (mTile >= GameModel::tileOneRemove && mTile <= GameModel::tileFourRemove)
    {
        effectsSprite.SetX(xPos);
        effectsSprite.SetY(yPos);
        effectsSprite.SetSubRect(sf::IntRect(0, 0, TILE_W, scaleY));
        int alpha=128 + 127 * cosf(clock.GetElapsedTime() * 8);
        effectsSprite.SetColor(sf::Color(255, 255, 255, alpha));
        app->Draw(effectsSprite);
    }
    else if (mTile == GameModel::tileBomb)
    {
        effectsSprite.SetX(xPos);
        effectsSprite.SetY(yPos);
        effectsSprite.SetSubRect(sf::IntRect(TILE_W, 0, TILE_W * 2, scaleY));
        int alpha=128 + 127 * cosf(clock.GetElapsedTime() * 9);
        effectsSprite.SetColor(sf::Color(255, 255, 255, alpha));
        app->Draw(effectsSprite);
    }
    else if (mTile == GameModel::tileArrows)
    {
        effectsSprite.SetX(xPos);
        effectsSprite.SetY(yPos);
        effectsSprite.SetSubRect(sf::IntRect(TILE_W * 2, 0, TILE_W * 3, scaleY));
        int alpha=128 + 127 * cosf(clock.GetElapsedTime() * 7);
        effectsSprite.SetColor(sf::Color(255, 255, 255, alpha));
        app->Draw(effectsSprite);
    }
}

void GfxEngine::renderScores(GameModel* gameModel)
{
    int scoreXf=761;
    std::ostringstream valStream;
    if (gameModel->getClickValue() > 0)
    {
        valStream << "+" << gameModel->getClickValue();
        textValue->SetText(valStream.str());
        textValue->SetX(scoreXf - textValue->GetRect().GetWidth());
        app->Draw(*textValue);
    }

    std::ostringstream scoreStream;
    scoreStream << gameModel->getScore();
    textScore->SetText(scoreStream.str());

    std::ostringstream highScoreStream;
    highScoreStream << gameModel->getHighScore();
    textHighScore->SetText(highScoreStream.str());

    textScore->SetX(scoreXf - textScore->GetRect().GetWidth());
    textHighScore->SetX(scoreXf - textHighScore->GetRect().GetWidth());
    app->Draw(*textScore);
    app->Draw(*textHighScore);

    if (gameModel->getGameType() != GameModel::gameTypeClassic)
    {
        app->Draw(levelSprite); // NEW DESIGN
        std::ostringstream levelStream;
        levelStream << gameModel->getLevel();
        textLevel->SetText(levelStream.str());
        textLevel->SetX(714 - textLevel->GetRect().GetWidth() / 2);
        app->Draw(*textLevel);
    }
}

void GfxEngine::renderDanger(GameModel* gameModel)
{
    bool isInDanger=false;
    if ((int)(clock.GetElapsedTime() * 3) % 2 == 0) return;
    for (int i=0; i < GRID_W; i++)
    {
        if (gameModel->getDanger(i))
        {
            app->Draw(dangerSprite[i]);
            isInDanger=true;
        }
    }
    if (!soundEnabled) return;
    if (isInDanger && sound[soundDanger].GetStatus() != sf::Sound::Playing)
    {
        sound[soundDanger].Play();
    }
    else if (!isInDanger && sound[soundDanger].GetStatus() == sf::Sound::Playing)
    {
        sound[soundDanger].Stop();
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
                app->Draw(ge->sprite);
            }
            else if (ge->type == GameEntity::typeText)
            {
                app->Draw(ge->string);
            }
            ge=EM->NextItem();
        }
    }
}

void GfxEngine::renderBestScores()
{
    app->Draw(modeScoreSprite);
    //app->Draw(*typeScore);

    // TODO

    for (int i=0; i < 10; i++)
    {
        app->Draw(*textNames[i]);
        app->Draw(*textScores[i]);
        app->Draw(*textPos[i]);
    }
}

void GfxEngine::renderGameOverBestScores(GameScores* gameScores, int gameType)
{
    int yTop=(SCREEN_H - (MENU_TOP_H + MENU_SCORE_H * 10 + MENU_BUTTONL_H + MENU_BOTTOM_H + MENU_SPACE_H)) / 2;
    int middle=MENU_POSX + MENU_W / 2;

    int yTrans=0;
    if (bestScoresDelay > 0.0f)
    {
        yTrans -= bestScoresDelay / bestScoresDelayMax * (MENU_TOP_H + MENU_SCORE_H * 10 + MENU_BUTTONL_H + MENU_BOTTOM_H + MENU_SPACE_H);
    }

    yTop += yTrans;

    //app->Draw(blackScreenSprite);

    menuString->SetColor(sf::Color::White);
    menuSpriteTop.SetPosition(MENU_POSX, yTop);
    app->Draw(menuSpriteTop);

    menuString->SetText(STRING_HI_SCORES[language]);
    menuString->SetSize(STRING_SIZE_MENU_TITLE);
    menuString->SetPosition(middle - menuString->GetRect().GetWidth() / 2, yTop + 11);
    app->Draw(*menuString);

    menuString->SetSize(STRING_SIZE_SCORE);

    for (int i=0; i < 10; i++)
    {
        int y=yTop + MENU_TOP_H + i * MENU_SCORE_H;
        if (i % 2 == 0)
        {
            menuSpriteHiScore1.SetPosition(MENU_POSX, y);
            app->Draw(menuSpriteHiScore1);
        }
        else
        {
            menuSpriteHiScore2.SetPosition(MENU_POSX, y);
            app->Draw(menuSpriteHiScore2);
        }

        if (gameScores->getCurrentScore() % 10 == i)
        {
            menuString->SetColor(sf::Color(100 + sinf(clock.GetElapsedTime() * 3) * 100,
                                        100 + sinf(clock.GetElapsedTime() * 4) * 100,
                                        100 + cosf(clock.GetElapsedTime() * 5) * 100));
        }
        else
        {
            menuString->SetColor(sf::Color::Black);
        }

        menuString->SetText(PODIUM_STRING[i]);
        menuString->SetPosition(MENU_POSX + 25, y + 3);
        app->Draw(*menuString);

        switch (gameType)
        {
            case GameModel::gameTypeNormal:
            {
                menuString->SetText(gameScores->getNormalScore(i).name);
                menuString->SetPosition(MENU_POSX + 65, y + 3);
                app->Draw(*menuString);

                std::ostringstream scoreStream;
                scoreStream << gameScores->getNormalScore(i).score;
                menuString->SetText(scoreStream.str());
                menuString->SetPosition(MENU_POSX + 475 - menuString->GetRect().GetWidth(), y + 3);
                app->Draw(*menuString);

                break;
            }

            case GameModel::gameTypeMotion:
            {
                menuString->SetText(gameScores->getMotionScore(i).name);
                menuString->SetPosition(MENU_POSX + 65, y + 3);
                app->Draw(*menuString);

                std::ostringstream scoreStream;
                scoreStream << gameScores->getMotionScore(i).score;
                menuString->SetText(scoreStream.str());
                menuString->SetPosition(MENU_POSX + 475 - menuString->GetRect().GetWidth(), y + 3);
                app->Draw(*menuString);

                break;
            }

            case GameModel::gameTypeClassic:
            {
                menuString->SetText(gameScores->getClassicScore(i).name);
                menuString->SetPosition(MENU_POSX + 65, y + 3);
                app->Draw(*menuString);

                std::ostringstream scoreStream;
                scoreStream << gameScores->getClassicScore(i).score;
                menuString->SetText(scoreStream.str());
                menuString->SetPosition(MENU_POSX + 475 - menuString->GetRect().GetWidth(), y + 3);
                app->Draw(*menuString);

                break;
            }
        }
    }

    // separator
    menuSpriteSpace.SetPosition(MENU_POSX, yTop + MENU_TOP_H + 10 * MENU_SCORE_H);
    app->Draw(menuSpriteSpace);

    menuString->SetColor(sf::Color::White);

    int yButton=SCORESMENU_BUTTONL_Y; //yTop + MENU_TOP_H + 10 * MENU_SCORE_H + MENU_SPACE_H;

    if (mouseOnLeftButton)
    {
        menuSpriteButtonLOver.SetPosition(MENU_POSX, yButton + yTrans);
        app->Draw(menuSpriteButtonLOver);
    }
    else
    {
        menuSpriteButtonL.SetPosition(MENU_POSX, yButton + yTrans);
        app->Draw(menuSpriteButtonL);
    }

    menuString->SetText(STRING_BACK_TO_MENU[language]);
    menuString->SetPosition(MENU_POSX + MENU_W / 4 - menuString->GetRect().GetWidth() / 2, yButton + 13 + yTrans);
    app->Draw(*menuString);

    if (mouseOnRightButton)
    {
        menuSpriteButtonROver.SetPosition(MENU_POSX + MENU_W / 2, yButton + yTrans);
        app->Draw(menuSpriteButtonROver);
    }
    else
    {
        menuSpriteButtonR.SetPosition(MENU_POSX + MENU_W / 2, yButton + yTrans);
        app->Draw(menuSpriteButtonR);
    }

    menuString->SetText(STRING_PLAY_AGAIN[language]);
    menuString->SetPosition(MENU_POSX + 3 * MENU_W / 4 - menuString->GetRect().GetWidth() / 2, yButton + 13 + yTrans);
    app->Draw(*menuString);

    menuSpriteBottom.SetPosition(MENU_POSX, yTop + MENU_TOP_H + 10 * MENU_SCORE_H + MENU_BUTTONL_H + MENU_SPACE_H);
    app->Draw(menuSpriteBottom);
}

void GfxEngine::renderEnterName(GameModel* gameModel, string name)
{
    app->Draw(bgSprite);
    if (gameModel->getGameType() == GameModel::gameTypeClassic || !effectsEnabled)
    {
        renderGrid(gameModel);
    }
    renderEffects();
    renderScores(gameModel);
    menuButtonSprite.SetSubRect(sf::IntRect(0, 0, GAMEMENU_BUTTON_W, GAMEMENU_BUTTON_H));
    app->Draw(menuButtonSprite);

    //app->Draw(gameOverSprite);

    int middle=MENU_POSX + MENU_W / 2;
    int yTop= (SCREEN_H - (MENU_TOP_H + MENU_BOTTOM_H + MENU_BUTTON_H)) / 2;

    menuString->SetColor(sf::Color::White);

    menuSpriteTop.SetPosition(MENU_POSX, yTop);
    app->Draw(menuSpriteTop);
    menuString->SetText(ENTER_NAME[language]);
    menuString->SetSize(STRING_SIZE_MENU_TITLE);
    menuString->SetPosition(middle - menuString->GetRect().GetWidth() / 2, yTop + 11);
    app->Draw(*menuString);

    menuSpriteButtonOver.SetPosition(MENU_POSX, yTop + MENU_TOP_H);
    app->Draw(menuSpriteButtonOver);

    textScoreName->SetText(string2wstring(name) + L"_");
    textScoreName->SetPosition(middle - textScoreName->GetRect().GetWidth() / 2, yTop + MENU_TOP_H + 11);
    if ((int)(clock.GetElapsedTime() * 3) % 2 > 0) textScoreName->SetText(string2wstring(name));
    app->Draw(*textScoreName);

    //app->Draw(blackFootSprite);
    menuSpriteBottom.SetPosition(MENU_POSX, yTop + MENU_TOP_H + MENU_BUTTON_H);
    app->Draw(menuSpriteBottom);


    app->Display();
}

void GfxEngine::renderMenu(Menu* myMenu)
{
    int middle=MENU_POSX + MENU_W / 2;
    if (myMenu == NULL) return;

    int n=myMenu->getNEntries();

    int yTop=(SCREEN_H - (MENU_TOP_H + MENU_BOTTOM_H + n * MENU_BUTTON_H)) / 2;

    app->Draw(blackScreenSprite);

    menuString->SetColor(sf::Color::White);
    menuSpriteTop.SetPosition(MENU_POSX, yTop);
    app->Draw(menuSpriteTop);
    menuString->SetText(myMenu->getTitle());
    menuString->SetSize(STRING_SIZE_MENU_TITLE);
    menuString->SetPosition(middle - menuString->GetRect().GetWidth() / 2, yTop + 11);
    app->Draw(*menuString);

    for (int i=0; i < n; i++)
    {
        int y=yTop + MENU_TOP_H + i * MENU_BUTTON_H;

        if (myMenu->getSelectedEntry() == i)
        {
            menuSpriteButtonOver.SetPosition(MENU_POSX, y);
            app->Draw(menuSpriteButtonOver);
        }
        else
        {
            menuSpriteButton.SetPosition(MENU_POSX, y);
            app->Draw(menuSpriteButton);
        }

        menuString->SetText(myMenu->getFullEntry(i));
        menuString->SetSize(STRING_SIZE_MENU_ITEM);
        menuString->SetPosition(middle - menuString->GetRect().GetWidth() / 2, y + 11);
        app->Draw(*menuString);
    }

    menuSpriteBottom.SetPosition(MENU_POSX, yTop + MENU_TOP_H + n * MENU_BUTTON_H);
    app->Draw(menuSpriteBottom);
}

void GfxEngine::renderCredits()
{
    app->Draw(creditsBgSprite);

    menuString->SetColor(sf::Color::White);
    //menuString->SetSize(HELP_TITLE_SIZE);
    //menuString->SetText(CREDITS_TITLE[language]);
    //menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 40);
    //app->Draw(*menuString);

    //menuString->SetText(APP_NAME + " " + APP_VERSION);
    //menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 15);
    //app->Draw(*menuString);

    menuString->SetSize(HELP_SUBTITLE_SIZE);
    menuString->SetText(CREDITS_PROGRAMMER[language]);
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 120);
    app->Draw(*menuString);
    menuString->SetSize(HELP_TEXT_SIZE);
    menuString->SetText(L"Seby <sebygames@googlemail.com>");
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 150);
    app->Draw(*menuString);

    menuString->SetSize(HELP_SUBTITLE_SIZE);
    menuString->SetText(CREDITS_GRAPHICS[language]);
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 190);
    app->Draw(*menuString);
    menuString->SetSize(HELP_TEXT_SIZE);
    menuString->SetText(L"Suiland");
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 220);
    app->Draw(*menuString);

    menuString->SetSize(HELP_SUBTITLE_SIZE);
    menuString->SetText(CREDITS_SOUND[language]);
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 260);
    app->Draw(*menuString);
    menuString->SetSize(HELP_TEXT_SIZE);
    menuString->SetText(L"Royalty free sounds");
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 290);
    app->Draw(*menuString);

    menuString->SetSize(HELP_SUBTITLE_SIZE);
    menuString->SetText(CREDITS_MUSIC[language]);
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 340);
    app->Draw(*menuString);
    menuString->SetSize(HELP_TEXT_SIZE);
    menuString->SetText(L"Another synthetic tribute to a muted-trumpet player (from Solcarlus)");
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 370);
    app->Draw(*menuString);
    menuString->SetText(L"Background noise (from Solcarlus)");
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 400);
    app->Draw(*menuString);
    menuString->SetText(L"Emergency landing (from Solcarlus)");
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 430);
    app->Draw(*menuString);
    menuString->SetText(L"Inside part 2 (from Nienvox)");
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 460);
    app->Draw(*menuString);

    menuString->SetSize(HELP_SUBTITLE_SIZE);
    menuString->SetColor(sf::Color(255, 255, 255, 128 + 127 * cos(5 * clock.GetElapsedTime())));
    menuString->SetText(CLICK_TO_CONTINUE[language]);
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 510);
    app->Draw(*menuString);

    app->Draw(sfmlLogoSprite);

    app->Display();
}

void GfxEngine::renderHelp()
{
    app->Draw(helpBgSprite);

    menuString->SetColor(sf::Color::White);
    menuString->SetSize(HELP_TITLE_SIZE);
    menuString->SetText(HELP_TITLE[language]);
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, HELP_TITLE_POSITION);
    app->Draw(*menuString);

    int yTrans=11;

    normalMiniSprite.SetPosition(HELP_LEFT_TITLE, 55 + yTrans);
    app->Draw(normalMiniSprite);

    menuString->SetSize(HELP_SMALLTEXT_SIZE);
    menuString->SetText(HELP_NORMAL1[language]);
    menuString->SetPosition(HELP_LEFT_TEXT, 85 + yTrans);
    app->Draw(*menuString);

    menuString->SetText(HELP_NORMAL2[language]);
    menuString->SetPosition(HELP_LEFT_TEXT, 115 + yTrans);
    app->Draw(*menuString);

    motionMiniSprite.SetPosition(HELP_LEFT_TITLE, 150 + yTrans);
    app->Draw(motionMiniSprite);

    menuString->SetText(HELP_MOTION1[language]);
    menuString->SetPosition(HELP_LEFT_TEXT, 180 + yTrans);
    app->Draw(*menuString);

    menuString->SetText(HELP_MOTION2[language]);
    menuString->SetPosition(HELP_LEFT_TEXT, 210 + yTrans);
    app->Draw(*menuString);

    classicMiniSprite.SetPosition(HELP_LEFT_TITLE, 245 + yTrans);
    app->Draw(classicMiniSprite);

    menuString->SetText(HELP_CLASSIC1[language]);
    menuString->SetPosition(HELP_LEFT_TEXT, 275 + yTrans);
    app->Draw(*menuString);

    menuString->SetText(HELP_CLASSIC2[language]);
    menuString->SetPosition(HELP_LEFT_TEXT, 305 + yTrans);
    app->Draw(*menuString);

    singleTile.SetSubRect(sf::IntRect(5 * TILE_W, 0, 6 * TILE_W, TILE_H));
    singleTile.SetPosition(48,350 + yTrans);
    app->Draw(singleTile);
    singleTile.SetSubRect(sf::IntRect(6 * TILE_W, 0, 7 * TILE_W, TILE_H));
    singleTile.SetPosition(HELP_LEFT_ICON,360 + yTrans);
    app->Draw(singleTile);

    singleTile.SetSubRect(sf::IntRect(8 * TILE_W, 0, 9 * TILE_W, TILE_H));
    singleTile.SetPosition(HELP_LEFT_ICON,400 + yTrans);
    app->Draw(singleTile);

    singleTile.SetSubRect(sf::IntRect(9 * TILE_W, 0, 10 * TILE_W, TILE_H));
    singleTile.SetPosition(HELP_LEFT_ICON,440 + yTrans);
    app->Draw(singleTile);

    singleTile.SetSubRect(sf::IntRect(10 * TILE_W, 0, 11 * TILE_W, TILE_H));
    singleTile.SetPosition(HELP_LEFT_ICON,480 + yTrans);
    app->Draw(singleTile);

    menuString->SetText(HELP_BONUS_COLOR[language]);
    menuString->SetPosition(HELP_LEFT_ICON_TEXT, 365 + yTrans);
    app->Draw(*menuString);

    menuString->SetText(HELP_BONUS_BOMB[language]);
    menuString->SetPosition(HELP_LEFT_ICON_TEXT, 405 + yTrans);
    app->Draw(*menuString);

    menuString->SetText(HELP_BONUS_ARROW[language]);
    menuString->SetPosition(HELP_LEFT_ICON_TEXT, 445 + yTrans);
    app->Draw(*menuString);

    menuString->SetText(HELP_BONUS_SKULL[language]);
    menuString->SetPosition(HELP_LEFT_ICON_TEXT, 485 + yTrans);
    app->Draw(*menuString);

    menuString->SetSize(HELP_SUBTITLE_SIZE);
    menuString->SetColor(sf::Color(255, 255, 255, 128 + 127 * cos(5 * clock.GetElapsedTime())));
    menuString->SetText(CLICK_TO_CONTINUE[language]);
    menuString->SetPosition((SCREEN_W - menuString->GetRect().GetWidth()) / 2, 525);
    app->Draw(*menuString);

    app->Display();
}

void GfxEngine::getKeys(GameInput* gameInput)
{
    // message processing loop
    xRand = 4.0 * (rand() / ((double)RAND_MAX)) - 2;
    yRand = 4.0 * (rand() / ((double)RAND_MAX)) - 2;

    // Process events
    sf::Event Event;
    while (app->GetEvent(Event))
    {
        // Close window : exit
        if (Event.Type == sf::Event::Closed)
        {
                gameInput->setQuitKey(true);
        }

        // Touche 'echap' appuyée
        if (Event.Type == sf::Event::KeyPressed) {
            if (Event.Key.Code == sf::Key::Escape)
            {
                gameInput->setEscKey(true);
            }
            else if (Event.Key.Code == sf::Key::Return)
            {
                gameInput->setEnterKey(true);
            }
            else if (Event.Key.Code == sf::Key::Back)
            {
                gameInput->setRemoveKey(true);
            }
            else if (Event.Key.Code >= sf::Key::A && Event.Key.Code <= sf::Key::Z)
            {
                gameInput->setMiscKey(true);
                gameInput->setCharKey('A' + (Event.Key.Code - sf::Key::A));
            }
            else if (Event.Key.Code >= sf::Key::Num0 && Event.Key.Code <= sf::Key::Num9)
            {
                gameInput->setMiscKey(true);
                gameInput->setCharKey('0' + (Event.Key.Code - sf::Key::Num0));
            }
            else if (Event.Key.Code == sf::Key::Space)
            {
                gameInput->setMiscKey(true);
                gameInput->setCharKey('_');
            }
         }

         if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::F10)) {
            sf::Image Screen = app->Capture();
            Screen.SaveToFile("screenshot.jpg");
        }


         if (Event.Type == sf::Event::MouseButtonPressed) {
            if (Event.MouseButton.Button == sf::Mouse::Left)
                gameInput->setMouseLeftClick(true);
            else if (Event.MouseButton.Button == sf::Mouse::Right)
                gameInput->setMouseRightClick(true);
            gameInput->setXMouse(oldXMouse);
            gameInput->setYMouse(oldYMouse);
         }

        if (Event.Type == sf::Event::MouseMoved) {
            gameInput->setMouseMotion(true);
            sf::Vector2f vec=app->ConvertCoords(Event.MouseMove.X, Event.MouseMove.Y);
            oldXMouse=(int)(vec.x);
            oldYMouse=(int)(vec.y);
            gameInput->setXMouse(oldXMouse);
            gameInput->setYMouse(oldYMouse);
         }
    }
}

void GfxEngine::updateTime()
{
    if (delay > 0.0f)
    {
        delay -= app->GetFrameTime();
    }
    if (bestScoresDelay > 0.0f)
    {
        bestScoresDelay -= app->GetFrameTime();
    }
    updateEffects();
}

void GfxEngine::updateEffects()
{
    EM->Animate(app->GetFrameTime());
}

void GfxEngine::animateUp()
{
    upAnimation=true;
    delay=upDelay;
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
                sprite.SetImage(tiles);
                sprite.SetCenter(TILE_W / 2, TILE_H / 2);
                int mTile=gameModel->getGameGrid(i, j) - 1;
                sprite.SetSubRect(sf::IntRect(mTile * TILE_W, 0, mTile * TILE_W + TILE_W, TILE_H));
                //sprite.SetSubRect(sf::IntRect(0,0,80,0));
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
                sprite.SetImage(tiles);
                sprite.SetCenter(TILE_W/2, TILE_H/2);
                sprite.SetPosition(-100, -100);
                int mTile=gameModel->getGameGrid(i, j)-1;
                sprite.SetSubRect(sf::IntRect(mTile * TILE_W, 0, mTile * TILE_W + TILE_W, TILE_H));
                //sprite.SetSubRect(sf::IntRect(0,0,80,0));
                sprite.SetColor(sf::Color(255, 255, 255, 128));
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
    sprite.SetImage(boomImage);
    sprite.SetCenter(TILE_W / 2, TILE_H / 2);
    sprite.SetPosition(xPos, yPos);
    sprite.SetScale(5.0f, 5.0f);
    sprite.SetColor(sf::Color(255, 255, 255, 164));
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
                GameEntity* entity=new GameEntityLightning(OFFSET_X + i * TILE_W, OFFSET_Y, GameEntity::typeSprite);
                entity->lifetime=0.6f;
                sf::Sprite sprite;
                sprite.SetImage(lightningVImage);
                sprite.SetColor(sf::Color(255, 255, 255, 128));
                entity->sprite=sprite;
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
                GameEntity* entity=new GameEntityLightning(OFFSET_X, OFFSET_Y + j * TILE_H, GameEntity::typeSprite);
                entity->lifetime=0.6f;
                sf::Sprite sprite;
                sprite.SetImage(lightningHImage);
                sprite.SetColor(sf::Color(255, 255, 255, 128));
                entity->sprite=sprite;
                EM->Add(entity);
        }
    }

    GameEntity* entity=new GameEntityWhiteScreen(OFFSET_X, OFFSET_Y, GameEntity::typeSprite);
    entity->lifetime=0.3f;
    sf::Sprite sprite;
    sprite.SetImage(whiteScreenImage);
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

    sf::String myString;
    myString.SetText(scoreStream.str());
    myString.SetFont(font);
    myString.SetSize(25);
    myString.SetPosition(entity->x, entity->y);;
    entity->string=myString;
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
    sf::String myString;
    myString.SetText(scoreStream.str());
    myString.SetFont(font);
    myString.SetSize(40);
    myString.SetPosition(800,600);
    entity->string=myString;
    EM->Add(entity);
}

void GfxEngine::playSound(int n)
{
    if (!soundEnabled) return;
    sound[n].Play();
}

void GfxEngine::playMusic(int n)
{
    if (!musicEnabled) return;

    stopMusic();
    music.SetLoop(true);

    switch(n)
    {
        case musicIntro:
        {
            if (!music.OpenFromFile(musicIntroFile))
            {
                std::cout << "Error loading " << musicIntroFile << std::endl;
            }
            break;
        }
        case musicGame:
        {
            if (!music.OpenFromFile(musicGameFile))
            {
                std::cout << "Error loading " << musicGameFile << std::endl;
            }
            break;
        }
        case musicClassic:
        {
            if (!music.OpenFromFile(musicClassicFile))
            {
                std::cout << "Error loading " << musicClassicFile << std::endl;
            }
            break;
        }
        case musicGameOver:
        {
            if (!music.OpenFromFile(musicGameOverFile))
            {
                std::cout << "Error loading " << musicGameOverFile << std::endl;
            }
            break;
        }
    }

    music.Play();
}

void GfxEngine::stopMusic()
{
    music.Stop();
}

void GfxEngine::initScores()
{


    sf::Color color1(240, 200, 90, 255);
    sf::Color color2(0, 200, 200, 255);
    for (int i=0; i < 10; i++)
    {
        //delete (textPos[i]);
        textPos[i]=new sf::String(PODIUM_STRING[i], font, STRING_SIZE_HISCORES);
        textPos[i]->SetPosition(425, 142 + i * 35);
        textPos[i]->SetColor(i % 2 == 0 ? color1 : color2);

        //delete (textNames[i]);
        textNames[i]=new sf::String("bob", font, STRING_SIZE_HISCORES);
        textNames[i]->SetPosition(470, 142 + i * 35);
        textNames[i]->SetColor(i % 2 == 0 ? color1 : color2);

        // delete (textScores[i]);
        textScores[i]=new sf::String("bob", font, STRING_SIZE_HISCORES);
        textScores[i]->SetPosition(680, 142 + i * 35);
        textScores[i]->SetColor(i % 2 == 0 ? color1 : color2);
    }
}


void GfxEngine::updateScores(GameScores* gameScores, int gameType)
{
    switch (gameType)
    {
        case GameModel::gameTypeNormal: modeScoreSprite.SetImage(modeNormalHiscores); break;
        case GameModel::gameTypeMotion: modeScoreSprite.SetImage(modeMotionHiscores); break;
        case GameModel::gameTypeClassic: modeScoreSprite.SetImage(modeClassicHiscores); break;
    }
    modeScoreSprite.SetPosition(597 - modeScoreSprite.GetSubRect().GetWidth() / 2, 58);

    for (int i=0; i < 10; i++)
    {
        switch (gameType)
        {
            case GameModel::gameTypeNormal:
            {
            textNames[i]->SetText(gameScores->getNormalScore(i).name);

            std::ostringstream scoreStream;

            scoreStream << gameScores->getNormalScore(i).score;
            textScores[i]->SetText(scoreStream.str());

            break;
            }

            case GameModel::gameTypeMotion:
            {
            textNames[i]->SetText(gameScores->getMotionScore(i).name);

            std::ostringstream scoreStream;

            scoreStream << gameScores->getMotionScore(i).score;
            textScores[i]->SetText(scoreStream.str());

            break;
            }

            case GameModel::gameTypeClassic:
            {
            textNames[i]->SetText(gameScores->getClassicScore(i).name);

            std::ostringstream scoreStream;

            scoreStream << gameScores->getClassicScore(i).score;
            textScores[i]->SetText(scoreStream.str());

            break;
            }
        }
    }
}

void GfxEngine::closeApp()
{
    app->Close();
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
        sound[i].SetVolume((float)soundVolume);
}

void GfxEngine::setMusicVolume(int musicVolume)
{
    this->musicVolume=musicVolume;
    music.SetVolume((float)musicVolume);
}
