#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio.hpp>
#include "player.hpp"

//using namespace game_parameters;
class Entity 
{
    public:
        std::string str_texture;
        sf::Texture texture;
        sf::Sprite sprite;
        sf::IntRect sizeRectSprite;
        sf::FloatRect rect; //rectangle (left, top, height, width) set the absolute position Sprite ;
        float dx, dy; //speed;
        float speed;
        float currentFrame;
        int hp;
        bool life;

        Entity(std::string &string_texture, sf::IntRect intRect, sf::FloatRect floatRect, float speed = 0.1f, int hitPoints = 1, bool life = true) : str_texture(string_texture), sizeRectSprite(intRect), rect(floatRect), speed(speed), hp(hitPoints), life(life)
    {
        dx = 0.0f;
        dy = 0.0f;
        currentFrame = 0.0f;
        if(!texture.loadFromFile(str_texture))
        {
            std::cerr << "texture.loadFromFile(" << string_texture << ") - error" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sizeRectSprite);
    };
        virtual void update(float time) = 0;

};

class Player : public Entity
{
    public:
        enum {left, right, up, down, jump, stop} state;
        bool onGround;
        sf::SoundBuffer buf_jump;
        sf::SoundBuffer buf_key;
        sf::SoundBuffer buf_eat;
        sf::Sound sound_key;
        sf::Sound sound_jump;
        sf::Sound sound_eat;

        Player(std::string &str_texture, sf::IntRect intRect, sf::FloatRect rect, float speed, int player_hp);
        void control();
        void update(float time);
        void CollisionX();
        void CollisionY();
};
