#include "header/office.hpp"

#include <SFML/Window/Mouse.hpp>

#include "header/inputs.hpp"

Office::Office(sf::RenderWindow& window, Player& player, BottomPanel& bottomPanel, const unsigned int language) :   m_window{window}, m_bottomPanel{bottomPanel}, m_language{language}, m_firstTimeIn{true},
                                                                                                                    m_door(sf::FloatRect(0, 0, 87, 240)), m_font(16, 16), m_inputs(window, player.getJoystickID())
{
    loadTextures();
    m_font.setTexture(m_textureContaigner.getAsset(textures::ID::supercarsfont));
    m_text.setFont(&m_font);

    setBackground();
    switch(m_language) {
        case 0 :
            m_languageJson.loadJsonFile("language/english/office.json");
            break;
        case 1:
            m_languageJson.loadJsonFile("language/usa/office.json");
            break;
        case 2 :
            m_languageJson.loadJsonFile("language/french/office.json");
            break;
        default :
            break;
    }
}


void Office::loadTextures()
{
    m_textureContaigner.loadAsset(textures::ID::office, "graphics/office/office.png");
    m_textureContaigner.loadAsset(textures::ID::darkgarage, "graphics/bitmap_fonts/super_cars_16x16.png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
}

void Office::setBackground()
{
    m_background.setTexture(m_textureContaigner.getAsset(textures::ID::office));
    m_background.setTextureRect(sf::IntRect(0, 0, 640, 360));
    m_background.setPosition(0, 0);
}

void Office::buy()                                ///// 16,252   16, 268   16,284   16,300   16,316   16,332
{
    bool inOffice{true};                        /////////// mettre les inputs
    while(inOffice) {
        m_window.clear();
        m_window.draw(m_background);
        m_window.draw(m_bottomPanel);
        m_window.display();
       // if(m_door.isPressed(m_command.coords)) { inOffice = false; }
    }
}
