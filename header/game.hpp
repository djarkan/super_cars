#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "header/player.hpp"
#include <assetcontainer/assetcontainer.hpp>
#include "header/assetkey.hpp"

class Game {
    public:
                                                                Game(sf::RenderWindow& );
        void                                                    launch();
        void                                                    loadTexture();




    private:
        sf::RenderWindow&                                        m_window;
        std::vector<Player>                                     playerList;
        AssetContainer<textures::ID, sf::Texture>               m_textureContaigner;
};

#endif
