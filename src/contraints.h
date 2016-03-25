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

#ifndef CONTRAINTS_H_INCLUDED
#define CONTRAINTS_H_INCLUDED

#include <string>

const std::string APP_NAME = "FreeTumble";
const std::string APP_VERSION = "V1.0";

#ifdef DATADIR
const std::string DATA_DIR = DATADIR;
#else
const std::string DATA_DIR = "data/media";
#endif

const int SCREEN_W = 800;
const int SCREEN_H = 600;

const int OFFSET_X = 20;
const int OFFSET_Y = 50;

const int GRID_W = 15;
const int GRID_H = 12;

const int NB_SPRITES = 11;

const int TILE_W = 40;
const int TILE_H = 40;
const int DANGER_W = 30;
const int DANGER_H = 30;

const int MENU_NORMAL_X = 74;
const int MENU_NORMAL_Y = 151;
const int MENU_MOTION_X = 74;
const int MENU_MOTION_Y = 250;
const int MENU_CLASSIC_X = 74;
const int MENU_CLASSIC_Y = 346;
const int MENU_ITEM_W = 260;
const int MENU_ITEM_H = 65;

const int MENU_HELP_X=430;
const int MENU_HELP_Y=530;
const int MENU_OPTIONS_X=517;
const int MENU_OPTIONS_Y=530;
const int MENU_CREDITS_X=604;
const int MENU_CREDITS_Y=530;
const int MENU_QUIT_X=685;
const int MENU_QUIT_Y=530;
const int MENU_ICON_W=63;
const int MENU_ICON_H=51;

const int STRING_SIZE_SCORE = 24;
const int STRING_SIZE_DESCRIPTION = 20;
const int STRING_SIZE_HISCORES = 21;

const int stonesLevel = 120;

const float upDelay = 0.25f;
const float gameOverAnimDelay = 2.0f;
const float bestScoresDelay = 1.0f;
//const float gameClickAnimDelay = 0.35f;

const int SCORE_NAME_MAX = 12;

const float delayForLevel[]={ 7.0f, 5.5f, 4.0f, 3.0f, 2.7f, 2.3f, 1.9f, 1.6f, 1.3f, 0.9f};
//const float delayForLevel[]={ 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f};
const int bonusScore[]={ 1500, 1000, 500, 350, 250, 150 };

const int GAMEMENU_BUTTON_X0=662;
const int GAMEMENU_BUTTON_Y0=529;
const int GAMEMENU_BUTTON_W=96;
const int GAMEMENU_BUTTON_H=35;
const int GAMEMENU_BUTTON_XF=GAMEMENU_BUTTON_X0 + GAMEMENU_BUTTON_W;
const int GAMEMENU_BUTTON_YF=GAMEMENU_BUTTON_Y0 + GAMEMENU_BUTTON_H;

const int CANCEL_BUTTON_X0=680;
const int CANCEL_BUTTON_Y0=400;
const int CANCEL_BUTTON_W=64;
const int CANCEL_BUTTON_H=64;
const int CANCEL_BUTTON_XF=CANCEL_BUTTON_X0 + CANCEL_BUTTON_W;
const int CANCEL_BUTTON_YF=CANCEL_BUTTON_Y0 + CANCEL_BUTTON_H;

const int SCORESMENU_BUTTONL_Y=470;

const int NB_SOUNDS=7;

const int NB_SKINS=4;
const std::wstring WSKINS[NB_SKINS]={ L"Blue", L"Green", L"Rosa", L"Brown"};
const std::string PODIUM_STRING[10]={"1.", "2.", "3.", "4.", "5.", "6.", "7.", "8.", "9.", "10."};

// menu related
const int CHOICES_MAX=11;
const int MENU_SIZE_MAX=8;

const int MENU_TOP_X0=0;
const int MENU_TOP_Y0=0;
const int MENU_TOP_W=500;
const int MENU_TOP_H=65;

const int MENU_SPACE_H=30;

const int MENU_BUTTON_X0=0;
const int MENU_BUTTON_Y0=79;
const int MENU_BUTTON_W=500;
const int MENU_BUTTON_H=64 + 10;

const int MENU_BUTTON_OVER_X0=0;
const int MENU_BUTTON_OVER_Y0=156;
const int MENU_BUTTON_OVER_W=500;
const int MENU_BUTTON_OVER_H=64 + 10;

const int MENU_BUTTONL_X0=0;
const int MENU_BUTTONL_Y0=318;
const int MENU_BUTTONL_W=250;
const int MENU_BUTTONL_H=65;

const int MENU_BUTTONR_X0=250;
const int MENU_BUTTONR_Y0=318;
const int MENU_BUTTONR_W=250;
const int MENU_BUTTONR_H=65;

const int MENU_BUTTONL_OVER_X0=0;
const int MENU_BUTTONL_OVER_Y0=395;
const int MENU_BUTTONL_OVER_W=250;
const int MENU_BUTTONL_OVER_H=65;

const int MENU_BUTTONR_OVER_X0=250;
const int MENU_BUTTONR_OVER_Y0=395;
const int MENU_BUTTONR_OVER_W=250;
const int MENU_BUTTONR_OVER_H=65;

const int MENU_SCORE1_X0=0;
const int MENU_SCORE1_Y0=235;
const int MENU_SCORE1_W=500;
const int MENU_SCORE1_H=35;

const int MENU_SCORE2_X0=0;
const int MENU_SCORE2_Y0=275;
const int MENU_SCORE2_W=500;
const int MENU_SCORE2_H=35;

const int MENU_BOTTOM_X0=0;
const int MENU_BOTTOM_Y0=460;
const int MENU_BOTTOM_W=500;
const int MENU_BOTTOM_H=40;

const int MENU_W=MENU_TOP_W;
const int MENU_SCORE_H=MENU_SCORE1_H;
const int MENU_POSX=(SCREEN_W - (MENU_TOP_X0 + MENU_TOP_W)) / 2;

const int STRING_SIZE_MENU_TITLE=34;
const int STRING_SIZE_MENU_ITEM=28;

const int HELP_TITLE_SIZE=29;
const int HELP_SUBTITLE_SIZE=24;
const int HELP_TEXT_SIZE=17;
const int HELP_SMALLTEXT_SIZE=15;

const int HELP_TITLE_POSITION=31;
const int HELP_LEFT_TITLE=65;
const int HELP_LEFT_TEXT=50;
const int HELP_LEFT_ICON=55;
const int HELP_LEFT_ICON_TEXT=95;

const std::string musicIntroFile="music/bart-contemplation.ogg";
const std::string musicGameFile="music/bart-contemplation2.ogg";
const std::string musicClassicFile="music/rezoner-puzzle1a.ogg";
const std::string musicGameOverFile="music/cynicmusic-crystalcave.ogg";

const int CONSOLE_STRING_SIZE=15;
const int CONSOLE_X=40;
const int CONSOLE_Y0=485;
const int CONSOLE_LINE_H=25;

const float CONSOLE_LINE_DELAY=1.5f;
const float CONSOLE_NEXT_LINE_DELAY=3.5f;

enum skinsEnum
{
    skinBlue,
    skinGreen,
    skinRosa,
    skinBrown
};

enum tileSetsEnum
{
    tsPastel,
    tsGems,
    tsMarble,
};

inline std::string getDataFile(const std::string fileName) {
    return DATA_DIR + "/" + fileName;
};

#endif // CONTRAINTS_H_INCLUDED
