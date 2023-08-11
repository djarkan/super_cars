#ifndef OFFICE_HPP
#define OFFICE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "header/assetkey.hpp"
#include "assetcontainer/assetcontainer.hpp"
#include "button/simpleClickBox/simpleClickBox.hpp"
#include "text/bitmaptext/bitmapText.hpp"
#include "header/bottompanel.hpp"
#include "header/player.hpp"
#include "jsonfile/jsonfile.hpp"
#include "header/inputs.hpp"

class Office {
    public:
                                                                Office(sf::RenderWindow& window, Player& player, BottomPanel& bottomPanel, const unsigned int language);
        void                                                    buy();


    private:
        sf::RenderWindow&                                       m_window;
        BottomPanel&                                            m_bottomPanel;
        unsigned int                                            m_language;
        bool                                                    m_firstTimeIn;
        mylib::JsonFile                                         m_languageJson;
        mylib::AssetContainer<textures::ID, sf::Texture>        m_textureContaigner;
        sf::Sprite                                              m_background;
        mylib::SimpleClickBox                                   m_door;
        mylib::BitmapFont                                       m_font;
        mylib::BitmapText                                       m_text;
        Inputs                                                  m_inputs;
        Command                                                 m_command;

        void                                                    loadTextures();
        void                                                    setBackground();
};

#endif
