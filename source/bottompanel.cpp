#include "header/bottompanel.hpp"
#include "header/car.hpp"

#include <iostream>

BottomPanel::BottomPanel(Player& player, sf::RenderWindow& window, unsigned int language) : m_player{player}, m_window{window}, m_language{language}, m_font(16,16)
{
    loadTextures();
    spriteConfiguration();
    switch(language) {
        case 0 :
            m_languageJson.loadJsonFile("language/english/bottompanel.json");
            break;
        case 1:
            m_languageJson.loadJsonFile("language/usa/bottompanel.json");
            break;
        case 2 :
            m_languageJson.loadJsonFile("language/french/bottompanel.json");
            break;
        default :
            break;
    }
    textConfiguration();
}

 void BottomPanel::loadTextures()
 {
    m_textureContaigner.loadAsset(textures::ID::supercarsfont, "graphics/bitmap_fonts/super_cars_16x16.png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
 }

void BottomPanel::spriteConfiguration()
{
    for(auto i = 0; i < 5; ++i) {
        sf::Sprite sprite;
        sprite.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
        sprite.setTextureRect(sf::IntRect(0, 150, 46, 10));
        m_sprites[i] = std::make_unique<sf::Sprite>(sprite);
    }
    m_sprites[0]->setTextureRect(sf::IntRect(0, 1940, 640, 40));
    m_sprites[0]->setPosition(0, 360);
    m_sprites[1]->setPosition(384, 384);
    m_sprites[2]->setPosition(448, 384);
    m_sprites[3]->setPosition(512, 384);
    m_sprites[4]->setPosition(576, 384);
    updateUsury();
}

void BottomPanel::textConfiguration()
{

    m_font.setTexture(m_textureContaigner.getAsset(textures::ID::supercarsfont));
    m_Text.setFont(&m_font);
    std::string text = m_languageJson.m_Root["bottomScreen"]["text"].asString();
    updateMoney(text);
    updateCarModel(text);
    m_Text.setText(text);
    m_Text.setPosition(16, 364);
}

void BottomPanel::updateMoney(std::string& text)
{
    std::string price;
    if(m_language < 2) {  price = m_languageJson.m_Root["bottomScreen"]["currency"].asString() + std::to_string(m_player.getMoney()); }
    else { price = std::to_string(m_player.getMoney()) + m_languageJson.m_Root["bottomScreen"]["currency"].asString(); }
    text.replace(39, 7, price);
}

void BottomPanel::updateCarModel(std::string& text)
{
    std::string model = m_languageJson.m_Root["bottomScreen"]["carModel"][m_player.getCarType()].asString();
    text.replace(47, 14, model);
}

void BottomPanel::updateUsury()
{
    m_sprites[1]->setTextureRect(sf::IntRect(0, 150, 46 * m_player.getCarEngineState(), 10));
    m_sprites[2]->setTextureRect(sf::IntRect(0, 150, 46 * m_player.getCarBodyState(), 10));
    m_sprites[3]->setTextureRect(sf::IntRect(0, 150, 46 * m_player.getCarFuelState(), 10));
    m_sprites[4]->setTextureRect(sf::IntRect(0, 150, 46 * m_player.getCarTyresState(), 10));
}

void BottomPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(auto& elem : m_sprites) {
        target.draw(*elem, states);
    }
    target.draw(m_Text, states);
}
