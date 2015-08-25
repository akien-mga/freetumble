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

#include "GameEntity.h"
#include <stdlib.h>

GameEntity::GameEntity()
{
    GameEntity(0.0f, 0.0f, 2);
}

GameEntity::GameEntity(float m_x, float m_y, int m_type)
{
    x = m_x;
    y = m_y;
    bbox.x = 0.0f; bbox.y = 0.0f;
    bbox.width = 0.0f; bbox.height = 0.0f;
    age = 0;
    lifetime = -1;
    visible = true;
    dying = false;
    viscosity = 0.99f;

    gravity.y = 0.0f;
    velocity.x = 0.0f;
    velocity.y = 0.0f;

    angle = 0.0f;
    spin = 0.0f;

    type = m_type;

    effect.actif = false;
}

void GameEntity::Animate(float delay)
{
    age += delay;

    //velocity.x *= viscosity;
    //velocity.y *= viscosity;

    //printf("%f += %f * %f \n", velocity.y, gravity.y, delay);

    velocity.y += gravity.y * delay;

    x += velocity.x * delay;
    y += velocity.y * delay;

    angle += spin * delay;

    if (lifetime > 0)
    {
        if (age>=lifetime) dying = true;
    }

    sprite.setPosition(x, y);
    sprite.setRotation(angle);
}

float GameEntity::Fade()
{
    if (lifetime > 0)
    {
        float f= 1.0f - (float)age/lifetime;
        if (f < 0.0f) f=0.0f;
        if (f > 1.0f) f=1.0f;
        return f;
    }
    else return 1.0f;
}


GameEntityBoom::GameEntityBoom(float m_x, float m_y, int m_type) : GameEntity(m_x, m_y, m_type){

}

void GameEntityBoom::Animate(float delay) {
    GameEntity::Animate(delay);

    float fade=Fade();

    if (fade>0.9f) {
        fade=(1.0f-fade) * 10;
    } else  {
        fade = fade*10/9;
    }


    sprite.setScale(5.0f*fade, 5.0f*fade);
}


GameEntityLightning::GameEntityLightning(float m_x, float m_y, int m_type) : GameEntity(m_x, m_y, m_type){
  width = 0;
  height = 0;
}

void GameEntityLightning::SetTexture(sf::Texture &texture){
  sprite.setTexture(texture);
  sprite.setColor(sf::Color(255, 255, 255, 128));

  width = sprite.getTexture()->getSize().x;
  height = sprite.getTexture()->getSize().y;
}

void GameEntityLightning::Animate(float delay) {
    GameEntity::Animate(delay);

    float fade=Fade();

    if (fade>0.9f) {
        fade=(1.0f-fade) * 10;
    } else  {
        fade = fade*10/9;
    }
    if (fade < 0.0f) fade = 0.0f;
    if (fade > 1.0f) fade = 1.0f;

    sprite.setColor(sf::Color(255, 255, 255, 255 * fade));

    bool flipX = rand()%2 == 0;
    bool flipY = rand()%2 == 0;

    sprite.setTextureRect(sf::IntRect(flipX ? width : 0, flipY ? height : 0,
                                      flipX ? -width : width, flipY ? -height : height));
}



GameEntityFlyingText::GameEntityFlyingText(float m_x, float m_y, int m_type) : GameEntity(m_x, m_y, m_type){

}

void GameEntityFlyingText::Animate(float delay) {
    GameEntity::Animate(delay);

    text.setPosition(x, y);

    float fade=Fade();
    text.setColor(sf::Color(128 * fade, 255 * fade, 255 , 255 * fade));
    //float fade=Fade();

    //sprite.SetColor(sf::Color(255, 255, 255, 255 * fade));

}

GameEntityWhiteScreen::GameEntityWhiteScreen(float m_x, float m_y, int m_type) : GameEntity(m_x, m_y, m_type){

}

void GameEntityWhiteScreen::Animate(float delay) {
    GameEntity::Animate(delay);

    float fade=Fade();

    sprite.setColor(sf::Color(255, 255, 255, 255 * fade));
}
