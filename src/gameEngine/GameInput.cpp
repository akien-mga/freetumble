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

#include "GameInput.h"

GameInput::GameInput()
{
    init();
}

void GameInput::init()
{
    quitKey=false;
    escKey=false;
    enterKey=false;
    removeKey=false;
    miscKey=false;
    mouseLeftClick=false;
    mouseRightClick=false;
    mouseMotion=false;
    xMouse=0;
    yMouse=0;
    charKey='\0';
}
