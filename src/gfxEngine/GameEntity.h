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

#ifndef __GAMEENTITY
#define __GAMEENTITY

#include <SFML/Graphics.hpp>

#include "MyTools.h"
#include "EntityManager.h"

class EntityManager;

/** Objects to be displayed during the game.
  * A basic physics has been implemented.
  * The EntityManager will destroy the GameEntity when dying. */
class GameEntity
{
public:
    enum entityTypes
    {
        typeSprite = 0,
        typeText
    };

    struct bboxStruct
    {
        float x, y, width, height;
    } bbox;

    struct effectStruct
    {
        float x, y;
        int type;
        bool actif;
    } effect;

    GameEntity();
    GameEntity(float m_x, float m_y, int m_type);
    //GameEntity(float m_x = 0.0f, float m_y = 0.0f, m_type = TypePart02 );
    virtual void Animate(float delay);
    float Fade();

    // position (geometry)
    float x, y;
    float angle;
    float spin;
    // visibility
    bool visible;
    // life_time
    float age, lifetime;
    bool dying;
    // physics
    Vector2D velocity;
    Vector2D acceleration;
    Vector2D gravity;
    float viscosity;
    // management
    int type;
    EntityManager* EM;

    sf::Sprite sprite;
    sf::Text text;
};

////////////////////////////////////////////////////////////////////////////////////////////

class GameEntityBoom : public GameEntity {
public:
    GameEntityBoom(float m_x, float m_y, int m_type);
    virtual void Animate(float delay);
};

class GameEntityLightning : public GameEntity {
public:
    GameEntityLightning(float m_x, float m_y, int m_type);
    virtual void Animate(float delay);
    void SetTexture(sf::Texture &texture);
private:
  int width;
  int height;
};

class GameEntityFlyingText : public GameEntity {
public:
    GameEntityFlyingText(float m_x, float m_y, int m_type);
    virtual void Animate(float delay);
};

class GameEntityWhiteScreen : public GameEntity {
public:
    GameEntityWhiteScreen(float m_x, float m_y, int m_type);
    virtual void Animate(float delay);
};

#endif
