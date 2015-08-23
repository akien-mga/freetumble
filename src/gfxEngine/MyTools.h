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

#ifndef __MYTOOLS
#define __MYTOOLS

#include <stdlib.h>
#include <math.h>

/** Vector utility, used in physics
 ** Default constructor creates a random Vector */
class Vector2D
{
public:
    float x, y;
    Vector2D()
    {
        float v = (float)rand() / RAND_MAX;
        v *= 6.283f;
        x=cosf(v); y=sinf(v);
    }

    Vector2D(float m_x, float m_y) { x=m_x; y=m_y; }
};

#endif
