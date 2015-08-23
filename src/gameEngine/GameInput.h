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

#ifndef CGAMEINPUT_H_INCLUDED
#define CGAMEINPUT_H_INCLUDED

/** storage input class
 ** exchange object between game engine and media engine */
class GameInput
{

public:
    GameInput();

    // init all the bool to "false"
    void init();

    bool getQuitKey() { return quitKey; }   // close window
    bool getEscKey() { return escKey; }
    bool getEnterKey() { return enterKey; }
    bool getRemoveKey() { return removeKey; }   // back space
    bool getMiscKey() { return miscKey; }       // all the other key - will be store in charKey
    bool getMouseMotion() { return mouseMotion; }
    bool getMouseLeftClick() { return mouseLeftClick; }
    bool getMouseRightClick() { return mouseRightClick; }
    int getXMouse() { return xMouse; }
    int getYMouse() { return yMouse; }
    char getCharKey() { return charKey; }

    void setQuitKey(bool quitKey) { this->quitKey=quitKey; }
    void setEscKey(bool escKey) { this->escKey=escKey; }
    void setEnterKey(bool enterKey) { this->enterKey=enterKey; }
    void setRemoveKey(bool removeKey) { this->removeKey=removeKey; }
    void setMiscKey(bool miscKey) { this->miscKey=miscKey; }
    void setMouseMotion(bool mouseMotion) { this->mouseMotion=mouseMotion; }
    void setMouseLeftClick(bool mouseLeftClick) { this->mouseLeftClick=mouseLeftClick; }
    void setMouseRightClick(bool mouseRightClick) { this->mouseRightClick=mouseRightClick; }
    void setXMouse(int xMouse) { this->xMouse=xMouse; }
    void setYMouse(int yMouse) { this->yMouse=yMouse; }
    void setCharKey(char charKey) { this->charKey=charKey; }

private:
    bool quitKey;
    bool escKey;
    bool enterKey;
    bool removeKey;
    bool miscKey;
    bool mouseMotion;
    bool mouseLeftClick;
    bool mouseRightClick;

    char charKey;

    int xMouse; // position of the mouse
    int yMouse;
};

#endif // CGAMEINPUT_H_INCLUDED
