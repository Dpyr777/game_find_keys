#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
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
#include "game_parameters.hpp"
#include "player.hpp"
#include "enemy.hpp"

using namespace game_parameters;

float game_parameters::offsetX{}, game_parameters::offsetY{};
const float game_parameters::MAP_WIDTH{800};
const float game_parameters::MAP_HEIGHT{600};
int game_parameters::ROW{};
int game_parameters::COL{};
int game_parameters::ground{34 * 32};
std::vector<std::string> game_parameters::tileMap;

int main()
{
    std::string nameFile{"tile_map.txt"};
    std::ifstream fin(nameFile);
    if (!fin.is_open())
    {
        std::cerr << "NO FILE: " << nameFile << std::endl;
    }

    for (std::size_t i{}; ; ++i)
    {
        ++ROW;
        std::string str;
        std::getline(fin, str);
        if (fin.eof())
        {
            --ROW;
            break;
        }
        tileMap.push_back(str);
    }

    for (std::size_t i{}; i < tileMap[0].size(); ++i)
    {
        ++COL;
    }
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH, MAP_HEIGHT), "Find_key");

    sf::Texture texture_heart, texture_background;
    texture_heart.loadFromFile("heart_32x32.png");
    texture_background.loadFromFile("background.jpg");

    //---------------------- Tile MAP -------------------------:
    sf::Texture texture_tile;
    texture_tile.loadFromFile("tile_set.png");

    sf::Sprite sprite_tile;
    sprite_tile.setTexture(texture_tile);
    sprite_tile.setTextureRect(sf::IntRect(0, 0, 32, 32));

    sf::Sprite sprite_key;
    sprite_key.setTexture(texture_tile);
    sprite_key.setTextureRect(sf::IntRect(460, 639, 50, 75)); 
    sprite_key.scale(0.5f, 0.5f);


    //---------------------- Tile MAP -------------------------;


    std::string str_texture{"star_wars1.png"};
    float player_speed = 0.1f;
    int player_hp = 3;
    Player player(str_texture, sf::IntRect(64,  45, 64, 115), sf::FloatRect(2 * 32, ground, 64, 115), player_speed, player_hp);

    int numEnemy = 7;
    Enemy* enemy = new Enemy[numEnemy];
    sf::Texture texture_pig;
    texture_pig.loadFromFile("run_pig_32x32.png");
    std::srand(std::time(0));
    for (int i{}; i < numEnemy; ++i)
    {
        int rd = rand() % 30;
        enemy[i].set(texture_pig, (2 * rd + 2) * 32, (ROW - 2 - rd) * 32 ); 
        enemy[i].sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    }
    enemy[0].sprite.scale(2.f, 2.f);

    sf::Sprite sprite_heart;
    sprite_heart.setTexture(texture_heart);

    sf::Sprite sprite_background;
    sprite_background.setTexture(texture_background);

    sf::Music music;
    music.openFromFile("music_fon.ogg");
    music.play();
    music.setLoop(true);
    music.setVolume(50.f);

    sf::Clock clock, time_music;
    sf::Clock clock_hp;
    clock_hp.restart();
    sf::Time time_hp;
    //player.sprite.setPosition((player.rect.left - offsetX), (player.rect.top - offsetY)); // position player coord;
    player.sprite.scale(0.5, 0.5);


    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 600; // control speed person;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        player.control();

        window.clear();
        player.update(time);
        window.draw(sprite_background);
        for (int i{}; i < numEnemy; ++i)
        {
            if (enemy[i].life == false) // enemy dead
                continue;
            enemy[i].update(time);
        
            if (player.rect.intersects(enemy[i].rect))
            {
                time_hp = clock_hp.getElapsedTime();
                if (player.dy > 0)
                {
                    enemy[i].dx = 0;
                    player.dy = -0.4;
                    enemy[i].life = false;
                }
                else if (time_hp > sf::seconds(2.f) && player.hp > 0) 
                {
                    --player.hp;
                    clock_hp.restart();
                    if (player.hp == 0)
                    {
                        player.sprite.setColor(sf::Color::Red);
                        return 0;
                    }
                }
            }
        }
        //if (player.rect.left > MAP_WIDTH / 2 && player.rect.left < (COL * 32 - MAP_WIDTH / 2))
            offsetX = player.rect.left - MAP_WIDTH / 2;
        //if (player.rect.top > MAP_HEIGHT / 2 && player.rect.top < (ROW * 32 - 150))
            offsetY = player.rect.top - MAP_HEIGHT / 2;

        player.life = false;
        for (int i{}; i < ROW; ++i)
        {
            for (int j{}; j < COL; ++j)
            {
                if (tileMap[i][j] == ' ')
                    continue;
                if (tileMap[i][j] == 'A')
                {
                    player.life = true;
                    sprite_key.setPosition(j * 32 - offsetX, i * 32 - offsetY);
                    window.draw(sprite_key);
                }
                else
                {
                    sprite_tile.setPosition(j * 32 - offsetX, i * 32 - offsetY);
                    window.draw(sprite_tile);
                }
            }
        }
        if (player.life == false)
        {
            return 0; // GameOver
        }

        if (player.hp < 3 ) // hit points are less than the starting ones
        {
            sprite_heart.setTextureRect(sf::IntRect(0, 0, 33 * player.hp, 25));

        }
        sprite_heart.setPosition(player.rect.left - offsetX - MAP_WIDTH / 2 + 32, player.rect.top - offsetY - MAP_HEIGHT / 2 + 32);
        window.draw(sprite_heart);
        window.draw(player.sprite);
        for (int i{}; i < numEnemy; ++i)
        {
            if (enemy[i].life == true)
                window.draw(enemy[i].sprite);
        }
        window.display();
    }

    return 0;
}
    
