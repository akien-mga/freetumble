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

#include "EntityManager.h"
#include <stdlib.h>

EntityManager::EntityManager()
{
	InitIterator();
	mItems = 0;
}

void EntityManager::Add(GameEntity* g)
{
	g->EM = this;
	EL.push_front(g);
	mItems++;
}


void EntityManager::InitIterator()
{
	mIterator = EL.begin ();
}

GameEntity* EntityManager::NextItem()
{
	if (mIterator == EL.end ()) return NULL;

	GameEntity *e = *mIterator;
	mIterator++;
	return e;
}

void EntityManager::Animate (float delay)
{
	if (!IsEmpty())
	{
		EntityList::iterator it;
		EntityList::iterator oldit = EL.begin ();
		//bool isBegin = false;
		for (it = EL.begin (); it != EL.end () && !IsEmpty();)
		{
			GameEntity *e = *it;
			oldit = it;
			it++;
			//e->Animate(delay, map);
			if (e->dying)
			{
				//printf("trying...");
				EL.erase(oldit);
				//printf("OK\n");
				delete e;
				mItems--;
			} // endif
			else
				e->Animate(delay);
		} // end for
	}
}

void EntityManager::Clean ()
{
	if (!IsEmpty())
	{
		EntityList::iterator it;
		EntityList::iterator oldit = EL.begin ();
		//bool isBegin = false;
		//EL.clear();
		for (it = EL.begin (); it != EL.end ();)
		{
			GameEntity *e = *it;
			oldit = it;
			it++;

			EL.erase(oldit);

			delete e;
		} // end for*/
		mItems = 0;
	}
}
