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
    //sf::RectangleShape rectangle;
    //rectangle.setSize(sf::Vector2f(32, 32));
    // texture_player.loadFromFile("knight.jpg"); // start x = 435, y = 45, width = 137, height = 150; run: x = 435 y = 190;
        //float currentFrame{};
        //sf::Sprite sprite;
        //sprite.setTexture(texture_player);
        //sprite.setTextureRect(sf::IntRect(0, 20, 190, 130));
        //sprite.setPosition(50, 50);
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
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH, MAP_HEIGHT), "2D-Game!");

    sf::Texture texture_player, texture_heart, texture_background;
    texture_player.loadFromFile("star_wars1.png"); // start x = 0 y = 20, width = 190, height = 130; 
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


    Player player(texture_player);
    player.sprite.setTextureRect(sf::IntRect(64, 45, 64, 115));

    int numEnemy = 7;
    Enemy* enemy = new Enemy[numEnemy ];
    sf::Texture texture_pig;
    texture_pig.loadFromFile("run_pig_32x32.png");
    std::srand(std::time(0));
    for (int i{}; i < numEnemy; ++i)
    {
        int rd = rand() % 30;
        enemy[i].set(texture_pig, (2 * rd + 2) * 32, (ROW - 2 - rd) * 32 ); 
        enemy[i].sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    }
    //enemy.sprite.scale(2.f, 2.f);

    sf::Sprite sprite_heart;
    sprite_heart.setTexture(texture_heart);
    sf::FloatRect rect_heart(sf::FloatRect(0, 0, 97, 25));
    //sprite_heart.setTextureRect(sf::IntRect(0, 0, 97, 25));

    sf::Sprite sprite_background;
    sprite_background.setTexture(texture_background);



    sf::SoundBuffer buffer;
    buffer.loadFromFile("fart_11.ogg");
    sf::Sound jump(buffer);

    sf::Music music;
    music.openFromFile("music_fon.ogg");
    music.play();
    music.setLoop(true);
    music.setVolume(50.f);

    sf::Clock clock, time_music;
    sf::Clock clock_hp;
    clock_hp.restart();
    sf::Time time_hp;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        //std::cout << "time1 = " << time << std::endl;


        time = time / 600; // control speed person;
    
        //std::cout << "time2 = " << time << std::endl;


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            player.dx = -0.1f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            player.dx = 0.1f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if (player.onGround)
            {
                player.dy = -0.4f; // speed jump
                player.onGround = false;
                jump.play();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
        }


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
    
