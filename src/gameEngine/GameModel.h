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

#ifndef GAMEMODEL_H_INCLUDED
#define GAMEMODEL_H_INCLUDED

#include "../contraints.h"

class GameModel {

public:
    // Constructor
    GameModel();

    // Accessors
    int getGameGrid(int x, int y) { return gameGrid[x][y]; }
    bool getZoneGrid(int x, int y) { return zoneGrid[x][y]; }
    int getNextRow(int x) { return nextRow[x]; }
    int getTx() { return tx; }
    int getTy() { return ty; }
    bool getOverZone() { return overZone; }
    int getScore() { return score; }
    int getHighScore() { return highScore; }
    int getClickValue() { return clickValue; }
    int getLevel() { return level; }
    int getNbStones() { return nbStones; }
    bool getGameOver() { return gameOver; }
    int getGameType() { return gameType; }
    float getPercentDelay() { return delay/rowDelay; }
    bool getDanger(int n) { return danger[n]; }
    bool getRowUp() { bool res=rowUp; rowUp=false; return res; }
    int getClassicBonus() { return classicBonus; }

    int getRemoveType() { return removeType; }
    enum removeTypeEnum {   removeTypeNormal,
                            removeTypeBomb,
                            removeTypeArrows,
                            removeTypeColor };

    bool getCanCancel() { return canCancel; }

    // Mutators
    void setTx(int tx) { this->tx=tx; }
    void setTy(int ty) { this->ty=ty; }
    void setClickValue(int clickValue) { this->clickValue=clickValue; }
    void setHighScore(int highScore) { this->highScore=highScore; }
    void setOverZone(bool overZone) { this->overZone=overZone; }

    void newGame(int gameType);
    void initGameGrid();
    void randomizeNextRow();
    void randomizeWholeGameGrid();
    void randomizeBottomGameGrid();
    void emptyZoneGrid();
    void emptyDanger();
    void findDanger();

    int findZone(int x, int y);
    int findZoneRecursive(int x, int y);
    int findZoneColor(int x, int y);
    int findZoneArrows(int x, int y);
    int findZoneBomb(int x, int y);

    bool hasHoles(int col);
    bool hasHoles();
    void updateColumn(int col);
    void updateGrid();
    void removeZone();
    void resetClickValue();
    void addRow();
    bool canAddRow();

    void updateTime(float dt);

    void saveState();
    void restoreState();

    enum gameTypeEnum {     gameTypeNormal=0,
                            gameTypeMotion,
                            gameTypeClassic };

    enum tileEnum { tileEmpty=0,
                    tileOne,
                    tileTwo,
                    tileThree,
                    tileFour,
                    tileOneRemove,
                    tileTwoRemove,
                    tileThreeRemove,
                    tileFourRemove,
                    tileBomb,
                    tileArrows,
                    tileUnremovable };


private:
    int gameGrid[GRID_W][GRID_H]; // main game grid
    bool zoneGrid[GRID_W][GRID_H]; // to save same color zone
    int nextRow[GRID_W];

    int tx; // current tile x position (mouse over) - tx==-1 when not on grid
    int ty; // current tile y position (mouse over)

    bool noMoreZone();

    bool overZone;

    int score;
    int highScore;
    int clickValue;
    int nbStones;
    int level;



    int gameType;

    bool gameOver;

    float delay;
    float rowDelay;

    bool danger[GRID_W];

    bool rowUp;
    int removeType;

    int countStones();
    void checkBonus();
    int classicBonus;

    bool canCancel;
    bool isCancelled;
    int oldGameGrid[GRID_W][GRID_H];
    int oldNextRow[GRID_W];
    int newNextRow[GRID_W];
    bool oldDanger[GRID_W];
    int oldScore;
    int oldHighScore;
    int oldNbStones;
    int oldLevel;
};

#endif // CGAMEMODEL_H_INCLUDED
