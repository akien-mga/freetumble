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

#include "GameScores.h"
#include "GameModel.h"

#include <fstream>

GameScores::GameScores(std::string _userDir)
{
    currentScore=-1;
    userDir=_userDir;
    loadScores();
}

void GameScores::loadScores()
{
    if (!loadScores(userDir + "scores.dat"))
        reset();
}

void GameScores::reset()
{
    loadScores(getDataFile("reset_scores.dat"));
    saveScores();
}

bool GameScores::loadScores(std::string filePath)
{
    ifstream f(filePath.c_str());
    if (!f.is_open()) return false;

    int n;
    int i;
    char c[SCORE_NAME_MAX + 1];

    // normal
    f>>c;
    for (i=0; i < 10; i++)
    {
        f>>c;
        normalScores[i].name=c;
        f>>n;
        normalScores[i].score=n;
    }

    // motion
    f>>c;
    for (i=0; i < 10; i++)
    {
        f>>c;
        motionScores[i].name=c;
        f>>n;
        motionScores[i].score=n;
    }

    // classic
    f>>c;
    for (i=0; i < 10; i++)
    {
        f>>c;
        classicScores[i].name=c;
        f>>n;
        classicScores[i].score=n;
    }

    f.close();
    return true;
}

void GameScores::saveScores()
{
    std::string filePath = userDir + "scores.dat";
    ofstream f(filePath.c_str());

    int i;
    // normal
    f << "normal" << endl;
    for (i=0; i < 10; i++)
    {
        f << normalScores[i].name << " " << normalScores[i].score << endl;
    }

    // motion
    f << "motion" << endl;
    for (i=0; i < 10; i++)
    {
        f << motionScores[i].name << " " << motionScores[i].score << endl;
    }

    // classic
    f << "classic" << endl;
    for (i=0; i < 10; i++)
    {
        f << classicScores[i].name << " " << classicScores[i].score << endl;
    }

    f.close();
}

GameScores::scoresStruct GameScores::getScore(int type, int n)
{
    switch (type)
    {
        case (GameModel::gameTypeNormal): return normalScores[n]; break;
        case (GameModel::gameTypeMotion): return motionScores[n]; break;
        case (GameModel::gameTypeClassic):return classicScores[n]; break;
        default: return normalScores[n];
    }
}

void GameScores::addScore(int type, int score, std::string name)
{
    switch (type)
    {
        case (GameModel::gameTypeNormal): currentScore=addScoreNormal(score, name); break;
        case (GameModel::gameTypeMotion): currentScore=addScoreMotion(score, name) + 10; break;
        case (GameModel::gameTypeClassic): currentScore=addScoreClassic(score, name) + 20; break;
    }
    saveScores();
}

int GameScores::addScoreNormal(int score, std::string name)
{
    int i=0;
    for (i=0; i < 10; i++)
    {
        if (score > normalScores[i].score) break;
    }
    for (int j=9; j > i; j--)
    {
        normalScores[j].name=normalScores[j - 1].name;
        normalScores[j].score=normalScores[j - 1].score;
    }
    normalScores[i].name=name;
    normalScores[i].score=score;

    return i;
}

int GameScores::addScoreMotion(int score, std::string name)
{
    int i=0;
    for (i=0; i < 10; i++)
    {
        if (score > motionScores[i].score) break;
    }
    for (int j=9; j > i; j--)
    {
        motionScores[j].name=motionScores[j - 1].name;
        motionScores[j].score=motionScores[j - 1].score;
    }
    motionScores[i].name=name;
    motionScores[i].score=score;

    return i;
}

int GameScores::addScoreClassic(int score, std::string name)
{
    int i=0;
    for (i=0; i < 10; i++)
    {
        if (score > classicScores[i].score) break;
    }
    for (int j=9; j > i; j--)
    {
        classicScores[j].name=classicScores[j - 1].name;
        classicScores[j].score=classicScores[j - 1].score;
    }
    classicScores[i].name=name;
    classicScores[i].score=score;

    return i;
}
