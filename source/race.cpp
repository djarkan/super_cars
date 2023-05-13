#include "header/race.hpp"

#include <string>
#include <SFML/Graphics/Sprite.hpp>
Race::Race(sf::RenderWindow& window) : m_window(window), m_textureContaigner()
{

}

void Race::loadTextures(int trackNb)
{
    m_textureContaigner.loadAsset(textures::ID::cars, "graphics/cars/cars.png");
    m_textureContaigner.loadAsset(textures::ID::track, "graphics/tracks/races/track" + std::to_string(trackNb) + ".png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
    m_textureContaigner.loadAsset(textures::ID::shadows, "graphics/tracks/shadows/track" + std::to_string(trackNb) + " shadows.png");
    m_textureContaigner.loadAsset(textures::ID::roadUp, "graphics/tracks/roads/track" + std::to_string(trackNb) + " up.png");
    m_textureContaigner.loadAsset(textures::ID::roadDown, "graphics/tracks/roads/track" + std::to_string(trackNb) + " down.png");
}

void Race::setPlayerScreen(int playerId, const sf::Vector2f& spawnCoord)
{
    PlayerScreen playerScreen;
    m_playersScreen.push_back(playerScreen);
    m_playersScreen[playerId].topView.setSize(sf::Vector2f(512.f, 350.f));
    m_playersScreen[playerId].topView.setCenter(spawnCoord.x, spawnCoord.y);
    m_playersScreen[playerId].topView.setViewport(sf::FloatRect(0.1f, 0.f, 0.8f, 0.885f));
    m_playersScreen[playerId].bottomView.setSize(sf::Vector2f(512.f, 40.f));
    m_playersScreen[playerId].bottomView.setCenter(256, 20);
    m_playersScreen[playerId].bottomView.setViewport(sf::FloatRect(0.1f, 0.9f, 0.8f, 0.1f));
}

bool Race::racing()
{
    Track track(1);
    sf::Vector2f coord(590.f, 270.f);
    setPlayerScreen(0, coord);
    sf::Sprite top;
    top.setPosition(0, 0);
    top.setTexture(m_textureContaigner.getAsset(textures::ID::track));
    sf::Sprite bottom;
    bottom.setPosition(0, 0);
    bottom.setTextureRect(sf::IntRect(0, 0, 512, 40));
    bottom.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
    sf::Sprite oneCar;
    oneCar.setTexture(m_textureContaigner.getAsset(textures::ID::cars));
    oneCar.setOrigin(24,17);
    oneCar.setTextureRect(sf::IntRect(0, 0, 48, 34));
    sf::Sprite shadows;
    shadows.setTexture(m_textureContaigner.getAsset(textures::ID::shadows));
    shadows.setPosition(0, 0);

    while(1) {
        m_window.clear();
    m_window.setView(m_playersScreen[0].topView);
        m_window.draw(top);

     //   oneCar.setRotation(90.f);
        oneCar.setPosition(coord);
        m_window.draw(oneCar);

        m_window.draw(shadows);

    m_window.setView(m_playersScreen[0].bottomView);
        m_window.draw(bottom);
        float x{256};
        sf::Sprite jauge;
        jauge.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
        jauge.setTextureRect(sf::IntRect(0, 150, 46, 10));
        for(auto i = 0; i < 4; ++i) {
            jauge.setPosition(x, 24);
            m_window.draw(jauge);
            x += 64;
        }
        m_window.display();
    }
    return true;
}
