#include <SFML/Audio/Sound.hpp>
#include <iostream>
#include "game_parameters.hpp"
#include "player.hpp"

using namespace game_parameters;

Player::Player(const sf::Texture &image)
            : dx(0.0f), dy(0.0f), sprite(image), rect(sf::FloatRect(2 * 32, ground, 64, 115)),
             onGround(true), currentFrame(0.0f), hp(3), life(true)
{
    buf.loadFromFile("key.ogg");
    sf::Sound temp(buf);
    key = temp;
}
void Player::update(float time)
{
    rect.left += dx * time;
    CollisionX();
    if (!onGround)
        dy += 0.0005 * time;
    rect.top += dy * time;
    onGround = false;
    CollisionY();

    currentFrame += 0.005 * time;

    if (currentFrame > 4)
        currentFrame -= 4;
    if (dx > 0)
        sprite.setTextureRect(sf::IntRect(192 * static_cast<int>(currentFrame) + 64, 45, 64, 115));
    if (dx < 0)
        sprite.setTextureRect(sf::IntRect(192 * static_cast<int>(currentFrame) + 64, 235, 64, 115));

    sprite.setPosition((rect.left - offsetX), (rect.top - offsetY)); // position player coord;
    dx = 0.0f;
}

void Player::CollisionX()
{
    for (int i{static_cast<int>(rect.top) / 32}; i < (rect.top + rect.height) / 32; ++i)
    {
        for (int j{static_cast<int>(rect.left) / 32}; j < (rect.left + rect.width) / 32; ++j)
        {
            if (tileMap[i][j] == 'B')
            {
                if (dx > 0)
                {
                    rect.left = j * 32 - rect.width; // rect.width == 64  2-tile.
                    //dx = 0.0f;
                }
                if (dx < 0)
                {
                    rect.left = j * 32 + 32;
                    //dx = 0.0f;
                }
            }
            else if (tileMap[i][j] == 'A')
            {
                key.play();
                tileMap[i][j] = ' ';
                life = false; // GameOver
            }
        }
    }
}
void Player::CollisionY()
{
    for (int i{static_cast<int>(rect.top) / 32}; i < (rect.top + rect.height) / 32; ++i)
    {
        for (int j{static_cast<int>(rect.left) / 32}; j < (rect.left + rect.width) / 32; ++j)
        {
            if (tileMap[i][j] == 'B')
            {
                if (dy > 0)
                {
                    rect.top = i * 32 - rect.height;
                    dy = 0.0f;
                    onGround = true;
                }
                if (dy < 0)
                {
                    rect.top = i * 32 + 32;
                    dy = 0.0f;
                }
            }
            else if (tileMap[i][j] == 'A')
            {
                key.play();
                tileMap[i][j] = ' ';
            }
        }
    }
}
