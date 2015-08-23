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

#ifndef CGFXENGINE_INCLUDED
#define CGFXENGINE_INCLUDED

#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "EntityManager.h"
#include "../contraints.h"
#include "../languages.h"
#include "../gameEngine/GameModel.h"
#include "../gameEngine/GameInput.h"
#include "../gameEngine/GameScores.h"

#include "../gameEngine/Menu.h"

/** sfml gfx, music and event engine */
class GfxEngine
{

public:
    GfxEngine();
    ~GfxEngine();

    bool getStatusOK() { return statusOK; }
    float getGameTime() { return clock.GetElapsedTime(); }
    float getFrameTime() { return app->GetFrameTime(); }
    bool getSoundEnabled() { return soundEnabled; }
    bool getMusicEnabled() { return musicEnabled; }
    bool getEffectsEnabled() { return effectsEnabled; }
    int getLanguage() { return language; }
    int getSoundVolume() { return soundVolume; }
    int getMusicVolume() { return musicVolume; }
    int getSkin() { return skin; }
    bool getMouseOnMenuButton() { return mouseOnMenuButton; }
    bool getMouseOnLeftButton() { return mouseOnLeftButton; }
    bool getMouseOnRightButton() { return mouseOnRightButton; }
    bool getMouseOnCancelButton() { return mouseOnCancelButton; }
    int getIntroMouseOver() { return introMouseOver; }

    void setSoundEnabled(bool soundEnabled) { this->soundEnabled=soundEnabled; }
    void setMusicEnabled(bool musicEnabled) { this->musicEnabled=musicEnabled; }
    void setEffectsEnabled(bool effectsEnabled) { this->effectsEnabled=effectsEnabled; }
    void setLanguage(int language);
    void setSoundVolume(int soundVolume);
    void setMusicVolume(int musicVolume);
    void setSkin(int skin);
    void setMouseOnMenuButton(bool mouseOnMenuButton) { this->mouseOnMenuButton=mouseOnMenuButton; }
    void setMouseOnLeftButton(bool mouseOnLeftButton) { this->mouseOnLeftButton=mouseOnLeftButton; }
    void setMouseOnRightButton(bool mouseOnRightButton) { this->mouseOnRightButton=mouseOnRightButton; }
    void setMouseOnCancelButton(bool mouseOnCancelButton) { this->mouseOnCancelButton=mouseOnCancelButton; }
    void setBestScoresDelay(float bestScoresDelay) { this->bestScoresDelay=bestScoresDelay; }
    void setIntroMouseOver(int introMouseOver) { this->introMouseOver=introMouseOver; }

    // rendering methodes
    void renderIntro(int choice, Menu* myMenu);
    void renderConsole();
    void renderGameOver(GameModel* gameModel, GameScores* gameScores);
    void renderGameOverAnim(GameModel* gameModel);
    void renderGame(GameModel* gameModel, Menu* myMenu);
    void renderGameClicking(GameModel* gameModel);
    void renderGrid(GameModel* gameModel);
    void renderHelper(GameModel* gameModel);
    void renderModeIcone(int mode);
    void renderNextRow(GameModel* gameModel);
    void renderScores(GameModel* gameModel);
    void renderDanger(GameModel* gameModel);
    void renderEffects();
    void renderBestScores();
    void renderGameOverBestScores(GameScores* gameScore, int gameType);
    void renderEnterName(GameModel* gameModel, string name);
    void renderMenu(Menu* myMenu);
    void renderCredits();
    void renderHelp();

    void getKeys(GameInput* gameInput);
    void updateTime();
    void updateEffects();
    void animateUp();

    void createGameOverEffect(GameModel* gameModel);
    void createRemoveEffect(GameModel* gameModel);
    void createBombEffect(int x, int y);
    void createLightningEffect(GameModel* gameModel);
    void createScoreTextEffect(int score);
    void createClassicBonusEffect(int bonus);

    void playSound(int n);
    void playMusic(int n);
    void stopMusic();

    void initScores();
    void updateScores(GameScores* gameScores, int gameType);

    int getMenuMouseOver(int x, int y, Menu* myMenu);

    void closeApp();

    enum soundsEnum
    {
        soundBrick,
        soundExplode,
        soundThunder,
        soundColor,
        soundDanger,
        soundMenuOver,
        soundMenuClick
    };

    enum musicsEnum
    {
        musicIntro,
        musicGame,
        musicClassic,
        musicGameOver
    };

    enum mouseOverENum
    {
        mouseOverNothing,
        mouseOverNormal,
        mouseOverMotion,
        mouseOverClassic,
        mouseOverHelp,
        mouseOverCredits,
        mouseOverOptions,
        mouseOverQuit
    };

private:

    sf::RenderWindow* app;

    sf::Image bg; // primary surface
    sf::Image helpBg;
    sf::Image creditsBg;
    sf::Image intro;
    sf::Image gameOver;
    sf::Image levelImage;
    sf::Image tiles; // game tiles
    sf::Image nextTiles;
    sf::Image effectsImage;
    sf::Image holeImage;
    sf::Image modeNormal;
    sf::Image modeMotion;
    sf::Image modeClassic;
    sf::Image modeNormalOver;
    sf::Image modeMotionOver;
    sf::Image modeClassicOver;
    sf::Image modeNormalMini;
    sf::Image modeMotionMini;
    sf::Image modeClassicMini;
    sf::Image modeNormalHiscores;
    sf::Image modeMotionHiscores;
    sf::Image modeClassicHiscores;
    sf::Image modeIcones;
    sf::Image dangerImage;
    sf::Image boomImage;
    sf::Image lightningHImage;
    sf::Image lightningVImage;

    sf::Image sfmlLogoImage;

    sf::Image blackFootImage;
    sf::Image whiteScreenImage;

    sf::Image blackScreen;
    sf::Image menuImage;
    sf::Image menuButtonImage;
    sf::Image cancelButtonImage;

    sf::Image buttonHelpImage;
    sf::Image buttonCreditsImage;
    sf::Image buttonOptionsImage;
    sf::Image buttonQuitImage;

    sf::Font font;

    sf::String* textScore;
    sf::String* textHighScore;
    sf::String* textValue;
    sf::String* textLevel;

    sf::String* textNames[10];
    sf::String* textScores[10];
    sf::String* textPos[10];

    sf::String* textScoreName;
    sf::String* textMode;

    sf::String* normalDescription[2];
    sf::String* motionDescription[2];
    sf::String* classicDescription[2];

    sf::String* menuString;

    sf::Sprite bgSprite;
    sf::Sprite helpBgSprite;
    sf::Sprite creditsBgSprite;
    sf::Sprite introSprite;
    sf::Sprite gameOverSprite;
    sf::Sprite levelSprite;
    sf::Sprite tile[GRID_W][GRID_H];
    sf::Sprite singleTile;
    sf::Sprite nextRowTile[GRID_W];
    sf::Sprite effectsSprite;
    sf::Sprite holeSprite;
    sf::Sprite normalSprite;
    sf::Sprite motionSprite;
    sf::Sprite classicSprite;
    sf::Sprite normalMiniSprite;
    sf::Sprite motionMiniSprite;
    sf::Sprite classicMiniSprite;
    sf::Sprite modeSprite;
    sf::Sprite dangerSprite[GRID_W];
    sf::Sprite modeScoreSprite;
    sf::Sprite blackFootSprite;

    sf::Sprite menuButtonSprite;
    sf::Sprite cancelButtonSprite;
    sf::Sprite blackScreenSprite;
    sf::Sprite menuSpriteTop;
    sf::Sprite menuSpriteBottom;
    sf::Sprite menuSpriteButton;
    sf::Sprite menuSpriteButtonL;
    sf::Sprite menuSpriteButtonR;
    sf::Sprite menuSpriteButtonOver;
    sf::Sprite menuSpriteButtonLOver;
    sf::Sprite menuSpriteButtonROver;
    sf::Sprite menuSpriteHiScore1;
    sf::Sprite menuSpriteHiScore2;
    sf::Sprite menuSpriteSpace;

    sf::Sprite buttonHelpSprite;
    sf::Sprite buttonCreditsSprite;
    sf::Sprite buttonOptionsSprite;
    sf::Sprite buttonQuitSprite;

    sf::Sprite sfmlLogoSprite;

    sf::Sprite miniButtonSprite;

    sf::SoundBuffer brickBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::SoundBuffer thunderBuffer;
    sf::SoundBuffer colorBuffer;
    sf::SoundBuffer dangerBuffer;
    sf::SoundBuffer menuOverBuffer;
    sf::SoundBuffer menuClickBuffer;

    sf::Sound sound[7];

    sf::Music music;

    bool statusOK;

    void initialize();
    void initWindow();
    void loadPictures();
    void loadSkin();
    void loadTileSet();
    void loadSounds();

    void renderTile(int x, int y, int tile, float xTrans=0.0f, float yTrans=0.0f);

    int oldXMouse;
    int oldYMouse;

    float xRand;
    float yRand;

    sf::Clock clock;

    float delay;
    float bestScoresDelay;
    float consoleDelay;
    int consoleIndex;
    bool upAnimation;

    EntityManager* EM;

    bool soundEnabled;
    bool musicEnabled;
    bool effectsEnabled;
    int language;
    int soundVolume;
    int musicVolume;
    int skin;
    int tileSet;

    bool mouseOnMenuButton;
    bool mouseOnLeftButton;
    bool mouseOnRightButton;
    bool mouseOnCancelButton;

    int introMouseOver;
};

#endif // CGFXENGINE_INCLUDED
