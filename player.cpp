#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include "game_parameters.hpp"
#include "player.hpp"

using namespace game_parameters;

Player::Player(std::string &str_texture, sf::IntRect intRect, sf::FloatRect rect, float speed = 0.1f, int player_hp = 1) : Entity(str_texture, intRect, rect, speed, player_hp) 
         //   dx(0.0f), dy(0.0f), sprite(image), rect(sf::FloatRect(2 * 32, ground, 64, 115)),
          //   onGround(true), currentFrame(0.0f), hp(3), life(true)
{
    onGround = true;
    offsetX = rect.left - MAP_WIDTH / 2;
    offsetY = rect.top - MAP_HEIGHT / 2;
    sprite.setPosition((rect.left - offsetX), (rect.top - offsetY)); // position player coord;
                                                                           
    buf_key.loadFromFile("key.ogg");
    buf_jump.loadFromFile("jump_player.ogg");
    sound_key.setBuffer(buf_key);
    sound_jump.setBuffer(buf_jump);
}
void Player::control()
{ 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        dx = -speed; // speed
        state = left;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        dx = speed; // speed
        state = right;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (onGround)
        {
            dy = -0.4f; // speed jump
            state = jump;
            onGround = false;
            sound_jump.play();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
       state = down;
       sprite.scale(1.0001f, 1.0001f);
    }
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
    if (dx != 0 || state == down)
    {
        currentFrame += 0.005 * time;
        sizeRectSprite.left = 64 + (192 * static_cast<int>(currentFrame));
        if (currentFrame > 4)
            currentFrame -= 4;
        if (state == right)
        {
            sizeRectSprite.top = 45;
            sprite.setTextureRect(sizeRectSprite);
        }
        if (state == left)
        {
            sizeRectSprite.top = 235;
            sprite.setTextureRect(sizeRectSprite);
        }
        state = stop;
        sprite.setPosition((rect.left - offsetX), (rect.top - offsetY)); // position player coord;
        dx = 0.0f;
    }

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
                sound_key.play();
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
                sound_key.play();
                tileMap[i][j] = ' ';
            }
        }
    }
}
