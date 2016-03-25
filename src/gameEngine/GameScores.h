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

#ifndef GAMESCORES_H_INCLUDED
#define GAMESCORES_H_INCLUDED

#include <string>

using namespace std ;

/** Used to load, store and save High-Scores. */
class GameScores {

public:
    GameScores(std::string _userDir);

    std::string userDir;

    struct scoresStruct {
        string name;
        int score;
        int level;
        int nbStones;
    };

    bool loadScores(std::string filePath);
    void loadScores();
    void saveScores();

    int getCurrentScore() { return currentScore; }
    void resetCurrentScore() { currentScore=-1; }

    scoresStruct getNormalScore(int n) { return normalScores[n]; }
    scoresStruct getMotionScore(int n) { return motionScores[n]; }
    scoresStruct getClassicScore(int n) { return classicScores[n]; }

    scoresStruct getScore(int type, int n);
    void addScore(int type, int score, std::string name);

    void reset();

private:
    scoresStruct normalScores[10];
    scoresStruct motionScores[10];
    scoresStruct classicScores[10];

    int addScoreNormal(int score, string name);
    int addScoreMotion(int score, string name);
    int addScoreClassic(int score, string name);

    int currentScore;
};

#endif
