#include <iostream>
#include "game_parameters.hpp"
#include "player.hpp"
#include "enemy.hpp"

using namespace game_parameters;

void Enemy::set(sf::Texture &image, int x, int y)
{
    sprite.setTexture(image);
    rect = sf::FloatRect(x, y, 32, 32);

    dx = -0.05f;
    currentFrame = 0;
    life = true;
}

void Enemy::update (float time)
{
    rect.left += dx * time;
    Collision();

    currentFrame += time * 0.005;
    if (currentFrame > 6)
        currentFrame -= 6;
    sprite.setTextureRect(sf::IntRect(32 * static_cast<int>(currentFrame), 0, 32, 32));
    //if (!life)
        //sprite.setTextureRect(sf::IntRect(58, 0, 32, 32));

    sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
}

void Enemy::Collision()
{
    for (int i{static_cast<int>(rect.top / 32)}; i < (rect.top + rect.height) / 32; ++i)
    {
        for (int j{static_cast<int>(rect.left / 32)}; j < (rect.left + rect.width) / 32; ++j)
        {
            if (tileMap[i][j] == 'B')
            {
                if (dx > 0)
                {
                    rect.left = j * 32 - rect.width;
                    dx = -0.05f;
                }
                else
                {
                    rect.left = j * 32 + 32;
                    dx = 0.05f;
                }
            }
        }
    }
}
