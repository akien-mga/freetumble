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

#include "GameModel.h"

#include <stdlib.h>
#include "../contraints.h"

int getNormalValue(int n)
{
  int result=0;
  int base=9;
  for (int i=1; i < n; i++) {
      result += i + base;
  }
  return result;
}

int getBonusValue(int n)
{
  int result=0;
  int base=4;
  for (int i=1; i < n; i++) {
      result += i + base;
  }
  return result;
}

GameModel::GameModel()
{
    highScore=0;
}

void GameModel::emptyZoneGrid()
{
    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            zoneGrid[i][j]= false;
        }
    }
}

void GameModel::emptyDanger()
{
    for (int i=0; i < GRID_W; i++)
    {
        danger[i]=false;
    }
}

void GameModel::findDanger()
{
    for (int i=0; i < GRID_W; i++)
    {
        danger[i]=(gameGrid[i][0] > 0);
    }
}

void GameModel::newGame(int gameType)
{
    this->gameType=gameType;
    tx=-1;
    ty=-1;
    score=0;
    clickValue=0;
    nbStones=0;
    level=1;
    overZone=false;
    gameOver=false;
    emptyZoneGrid();
    oldTime=0.0f;
    delay=0.0f;
    rowDelay=delayForLevel[0];
    rowUp=false;
    canCancel=false;
    isCancelled=false;

    switch (gameType)
    {
        case gameTypeClassic:
        {
            randomizeWholeGameGrid();
            break;
        }
        case gameTypeNormal:
        case gameTypeMotion:
        {
            initGameGrid();
            randomizeBottomGameGrid();
            randomizeNextRow();
            break;
        }
    }

    if (noMoreZone())
    {
        if (gameType==gameTypeMotion)
        {
            if (rowDelay>delay + 0.5f) rowDelay=delay + 0.5f;
        }
        else // gameTypeNormal
        {
            if (canAddRow())
            {
                addRow();
            }
            else
            {
                gameOver=true;
                emptyDanger();
            }
        }
    }

    emptyDanger();
}

void GameModel::initGameGrid()
{
    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            gameGrid[i][j]= 0;
        }
    }
}

void GameModel::randomizeNextRow()
{
    int i;

    if (isCancelled)
    {
        for (i=0; i < GRID_W; i++)
        {
            nextRow[i]=newNextRow[i];
        }
        isCancelled=false;
        return;
    }

    int bonusOffset=tileTwoRemove;
    int bonusRand=5;
    int normalOffset=tileTwo;
    int normalRand=3;

    if (level > 9)
    {
        bonusOffset=tileOneRemove;
        bonusRand=7;
        normalOffset=tileOne;
        normalRand=4;
    }
    else if (level > 5)
    {
        bonusOffset=tileTwoRemove;
        bonusRand=6;
        normalOffset=tileTwo;
        normalRand=3;
    }

    for (i=0; i < GRID_W; i++)
    {
        int dice=1000 * (rand() / ((double)RAND_MAX));
        if (dice < 12)
        {
            nextRow[i]= bonusOffset + bonusRand * (rand() / ((double)RAND_MAX));
        }
        else
        {
            nextRow[i]= normalOffset + normalRand * (rand() / ((double)RAND_MAX));
        }
    }
}

void GameModel::randomizeWholeGameGrid()
{
    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            gameGrid[i][j]= 1 + 4 * (rand() / ((double)RAND_MAX));
        }
    }
}

void GameModel::randomizeBottomGameGrid()
{
    for (int i=0; i < GRID_W; i++)
    {
        int randomNumber = 1 + 4 * (rand() / ((double)RAND_MAX));
        for (int j=GRID_H-1; j >= GRID_H - randomNumber; j--)
        {
            gameGrid[i][j]= 2 + 3 * (rand() / ((double)RAND_MAX));
        }
    }
}

int GameModel::findZoneRecursive(int x, int y)
{
    zoneGrid[x][y]=true;
    int n=1;
    if ( x > 0 && zoneGrid[x - 1][y] == false && gameGrid[x][y] == gameGrid[x - 1][y])
    {
        n += findZone(x - 1, y);
    }
    if ( x < (GRID_W - 1) && zoneGrid[x + 1][y] == false && gameGrid[x][y] == gameGrid[x + 1][y])
    {
        n += findZone(x + 1, y);
    }
    if ( y > 0 && zoneGrid[x][y - 1] == false && gameGrid[x][y] == gameGrid[x][y - 1])
    {
        n += findZone(x, y - 1);
    }
    if ( y < GRID_H - 1 && zoneGrid[x][y + 1] == false && gameGrid[x][y] == gameGrid[x][y + 1])
    {
        n += findZone(x, y + 1);
    }
    clickValue=getNormalValue(n);
    return n;
}

int GameModel::findZoneColor(int x, int y)
{
    removeType=removeTypeColor;

    zoneGrid[x][y]=true;
    int n=1;

    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            if (gameGrid[i][j]==gameGrid[x][y] - 4 || gameGrid[i][j] == gameGrid[x][y])
            {
                zoneGrid[i][j]=true;
                n++;
            }
        }
    }

    clickValue=getBonusValue(n);
    return n;
}

int GameModel::findZoneArrows(int x, int y)
{
    zoneGrid[x][y]=true;
    removeType=removeTypeArrows;
    int n=1;

    for (int i=0; i<GRID_W; i++)
    {
        if (gameGrid[i][y] > 0 && !zoneGrid[i][y])
        {
            if (gameGrid[i][y] == tileArrows)
            {
                n += findZoneArrows(i, y);
            }
            else
            {
                zoneGrid[i][y]=true;
                n++;
            }
        }
    }
    for (int j=0; j<GRID_H; j++)
    {
        if (gameGrid[x][j] > 0 && !zoneGrid[x][j])
        {
            if (gameGrid[x][j] == tileArrows)
            {
                n += findZoneArrows(x, j);
            }
            else
            {
                zoneGrid[x][j]=true;
                n++;
            }
        }
    }

    clickValue=getBonusValue(n);
    return n;
}

int GameModel::findZoneBomb(int x, int y)
{
    zoneGrid[x][y]=true;
    removeType=removeTypeBomb;
    int n=1;

    for (int i=x-2; i<=x+2; i++) {
        for (int j=y-2; j<=y+2; j++) {
            if ( !zoneGrid[i][j]
                && i >= 0 && i < GRID_W && j >= 0 && j < GRID_H     // out of grid test
                && !((i == x-2 || i == x+2) && (j == y-2 || j == y+2))  // no corner
                && gameGrid[i][j]>0) {
                if (gameGrid[i][j] == tileBomb) // recursive bombs
                {
                    n+=findZoneBomb(i, j);
                }
                zoneGrid[i][j]=true;
                n++;
            }
        }
    }

    clickValue=getBonusValue(n);
    return n;
}

int GameModel::findZone(int x, int y)
{
    removeType=removeTypeNormal;
    if (gameGrid[x][y] >= tileOne && gameGrid[x][y] <= tileFour)
    {
        return findZoneRecursive(x, y);
    }
    if (gameGrid[x][y] >= tileOneRemove && gameGrid[x][y] <= tileFourRemove)
    {
        return findZoneColor(x, y);
    }
    if (gameGrid[x][y] == tileArrows)
    {
        return findZoneArrows(x, y);
    }
    if (gameGrid[x][y] == tileBomb)
    {
        return findZoneBomb(x, y);
    }

    return 0;
}

bool GameModel::hasHoles(int col)
{
    bool hole=false;
    for (int i=GRID_H-1; i >= 0; i--)
    {
        if (!hole && gameGrid[col][i] == 0)
        {
            hole=true;
        }
        else if (hole && gameGrid[col][i] > 0)
        {
            return true;
        }
    }
    return false;
}

bool GameModel::hasHoles()
{
    bool hole = false;
    for (int i=GRID_W-1; i >= 0; i--)
    {
        if (!hole && gameGrid[i][GRID_H-1] == 0)
        {
            hole = true;
        }
        else if (hole && gameGrid[i][GRID_H-1] > 0)
        {
            return true;
        }
    }
    return false;
}

void GameModel::updateColumn(int col)
{
    while (hasHoles(col))
    {
        for (int i=GRID_H-1; i > 0; i--)
        {
            if (gameGrid[col][i] == 0)
            {
                for (int j=i; j > 0; j--)
                {
                    gameGrid[col][j] = gameGrid[col][j-1];
                }
                gameGrid[col][0]=0;
            }
        }
    }
}

void GameModel::updateGrid()
{
    for (int i=0; i < GRID_W; i++)
    {
        updateColumn(i);
    }
    if (gameType != gameTypeClassic) return;
    while (hasHoles()) {
        for (int i=GRID_W-1; i > 0; i--)
        {
            if (gameGrid[i][GRID_H-1] == 0)
            {
                for (int j=i; j >= 0; j--)
                {
                    for (int k=0; k < GRID_H; k++)
                    {
                        if (j==0)
                        {
                            gameGrid[j][k]=0;
                        }
                        else
                        {
                            gameGrid[j][k]=gameGrid[j-1][k];
                        }
                    }
                }
            }
        }
    }

}

void GameModel::removeZone()
{
    saveState();
    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            if (zoneGrid[i][j]) {
                gameGrid[i][j]=0;
                nbStones++;
            }
        }
    }
    updateGrid();

    level=1 + nbStones / stonesLevel;
    if (level > 10) level=10;

    emptyZoneGrid();
    score += clickValue;
    clickValue=0;
    if (score > highScore)
    {
        highScore=score;
    }

    switch (gameType)
    {
        case gameTypeClassic:
        {
            if (noMoreZone())
            {
                checkBonus();
                gameOver=true;
            }
            break;
        }
        case gameTypeNormal:
        {
            if (canAddRow())
            {
                addRow();
            }
            else
            {
                gameOver=true;
                emptyDanger();
            }
            break;
        }
        case gameTypeMotion:
        {
            if (noMoreZone())
            {
                if (rowDelay > delay + 0.5f)
                {
                    rowDelay=delay + 0.5f;
                }
            }
            findDanger();
            break;
        }
    }
}

bool GameModel::noMoreZone()
{
    if (gameType == gameTypeClassic)
    {
        for (int i=0; i < GRID_W; i++)
        {
            for (int j=0; j < GRID_H; j++)
            {
                if (gameGrid[i][j] > 0) {
                    if (i > 0 && gameGrid[i][j] == gameGrid[i-1][j]) return false;
                    if (i < GRID_W-1 && gameGrid[i][j] == gameGrid[i+1][j]) return false;
                    if (j > 0 && gameGrid[i][j] == gameGrid[i][j-1]) return false;
                    if (j < GRID_H-1 && gameGrid[i][j] == gameGrid[i][j+1]) return false;
                }
            }
        }
    }
    else
    {
        for (int i=0; i < GRID_W; i++)
        {
            for (int j=0; j < GRID_H; j++)
            {
                int n=0;
                if (gameGrid[i][j] > 0)
                {
                    if (gameGrid[i][j] >= tileOneRemove) return false;
                    if (i > 0 && gameGrid[i][j] == gameGrid[i-1][j]) n++;
                    if (i < GRID_W-1 && gameGrid[i][j] == gameGrid[i+1][j]) n++;
                    if (j > 0 && gameGrid[i][j] == gameGrid[i][j - 1]) n++;
                    if (j < GRID_H-1 && gameGrid[i][j] == gameGrid[i][j + 1]) n++;
                    if (n > 1) return false;
                }
            }
        }
    }
    return true;
}

void GameModel::resetClickValue() {
    clickValue=0;
}

void GameModel::addRow()
{
    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H-1; j++)
        {
            gameGrid[i][j]=gameGrid[i][j + 1];
        }
        gameGrid[i][GRID_H - 1]=nextRow[i];
    }
    randomizeNextRow();

    if (noMoreZone())
    {
        if (gameType==gameTypeMotion)
        {
            if (rowDelay>delay + 0.5f) rowDelay=delay + 0.5f;
        }
        else // gameTypeNormal
        {
            if (canAddRow())
            {
                addRow();
            }
            else
            {
                gameOver=true;
                emptyDanger();
            }
        }
    }


    findDanger();
    rowUp=true;
}

bool GameModel::canAddRow()
{
    for (int i=0; i < GRID_W; i++)
    {
        if (gameGrid[i][0] > 0)
        {
            return false;
        }
    }
    return true;
}

void GameModel::updateTime(float newTime)
{
    delay+=newTime - oldTime;
    oldTime=newTime;
    if (delay > rowDelay)
    {
        delay-=rowDelay;
        if (canAddRow())
        {
            rowDelay=delayForLevel[level - 1];
            addRow();
        }
        else
        {
            gameOver=true;
        }
    }
}

int GameModel::countStones()
{
    int n=0;
    for (int i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            if (gameGrid[i][j] != tileEmpty) n++;
        }
    }
    return n;
}

void GameModel::checkBonus()
{
    classicBonus=-1;
    int count=countStones();
    if (count < 6)
    {
        score += bonusScore[count];
        classicBonus=count;
        if (score > highScore)
        {
            highScore=score;
        }
    }
}

void GameModel::saveState()
{
    canCancel=true;
    int i;
    for (i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            oldGameGrid[i][j]=gameGrid[i][j];
        }
    }
    for (i=0; i < GRID_W; i++)
    {
        oldNextRow[i]=nextRow[i];
        oldDanger[i]=danger[i];
    }

    oldScore=score;
    oldHighScore=highScore;
    oldNbStones=nbStones;
    oldLevel=level;
}

void GameModel::restoreState()
{
    if (!canCancel || gameType==gameTypeMotion) return;

    int i;
    for (i=0; i < GRID_W; i++)
    {
        for (int j=0; j < GRID_H; j++)
        {
            gameGrid[i][j]=oldGameGrid[i][j];
        }
    }

    score=oldScore;
    highScore=oldHighScore;
    nbStones=oldNbStones;
    level=oldLevel;

    canCancel=false;

    if (gameType==gameTypeClassic) return;

    isCancelled=true;

    for (i=0; i < GRID_W; i++)
    {
        newNextRow[i]=nextRow[i];
        nextRow[i]=oldNextRow[i];
        danger[i]=oldDanger[i];
    }
}
