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

class Player
{
    public:
        float dx, dy;
        sf::Sprite sprite;
        sf::FloatRect rect;
        bool onGround;
        float currentFrame;
        int hp;
        bool life;
        sf::SoundBuffer buf;
        sf::Sound key;

        Player(const sf::Texture &image);
        void update(float time);
        void CollisionX();
        void CollisionY();
};
