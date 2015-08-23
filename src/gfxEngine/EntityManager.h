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

#ifndef __ENTITYMANAGER
#define __ENTITYMANAGER

#include <list>
class GameEntity;
#include "GameEntity.h"

class EntityManager
{

public:
    EntityManager();
    ~EntityManager() { Clean(); };
    void InitIterator();
    void Add(GameEntity* g);
    GameEntity* NextItem();
    void Animate (float delay);

    bool IsEmpty() { return mItems == 0; };
    int Count() { return mItems; }
    void Clean();

    void SaveIterator() { mSavedIterator = mIterator; }
    void RestoreIterator() { mIterator = mSavedIterator; }


private:
    typedef std::list<GameEntity*> EntityList;
    EntityList EL;
    EntityList::iterator mIterator, mSavedIterator;
    int mItems;
    float oldTime;
};

#endif
