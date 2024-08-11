#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "enemy.hpp"


class Enemy 
{
    public:
        float dx, dy;
        sf::FloatRect rect;
        sf::Sprite sprite;
        float currentFrame;
        bool life;

        void set(sf::Texture &image, int x, int y);
        void update (float time);
        void Collision();
};
#endif

