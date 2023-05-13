#include <header/game.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <header/car.hpp>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "assetcontainer/assetcontainer.hpp"
#include "header/race.hpp"
#include "header/track.hpp"
#include <string>

Game::Game(sf::RenderWindow& window): m_window{window}
{
    loadTexture();
}

void Game::launch()  // json langage et si json de config reçoit les tableaux scores classement nb races/money et liste meilleurs temps/pseudo par ref non const
{

    std::string trackNB{"1"};
    Race race(m_window);
    race.loadTextures(std::stoi(trackNB));
    Player player(1, 0, "DJAR", m_textureContaigner.getAsset(textures::ID::spritesheet));
    if(!race.racing()) {m_window.close(); }




    sf::Event event;
    sf::CircleShape circle(1.f);
    Car car(150, 250, 70, 40, 0, 100, 1);
    sf::Vector2f coord(car.getX(), car.getY());
    while (m_window.isOpen()) {
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
        }
        m_window.clear();
      //  car.turnRight();
        car.move();
        for(auto i = 0; i < 12; ++i) {
            circle.setPosition(car.getCarLimit(i));
            if(i == 0) { circle.setFillColor(sf::Color::Blue); }
            else { circle.setFillColor(sf::Color::Green); }
            m_window.draw(circle);
            circle.setPosition(car.getCarCenter());
            circle.setFillColor(sf::Color::Red);
            m_window.draw(circle);
        }
        sf::sleep(sf::milliseconds(100));
        m_window.display();
    }
}

void Game::loadTexture()
{
    m_textureContaigner.loadAsset(textures::ID::game, "graphics/game/game.png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
}
