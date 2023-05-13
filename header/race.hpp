#ifndef    __RACE_HPP__
#define    __RACE_HPP__

#include <SFML/Graphics/RenderWindow.hpp>
#include <assetcontainer/assetcontainer.hpp>
#include "header/assetkey.hpp"
#include "header/track.hpp"
#include <iostream>

#include <jsonfile/jsonfile.hpp>

class Race {
    public:
                                                                Race(sf::RenderWindow& window);
                void                                            loadTextures(int trackNb);
                void                                            setPlayerScreen(int playerId, const sf::Vector2f& coord);
                bool                                            racing();

    private:
                sf::RenderWindow&                               m_window;
                struct PlayerScreen {
                    sf::View                                    topView;
                    sf::View                                    bottomView;
                };
                AssetContainer<textures::ID, sf::Texture>       m_textureContaigner;
                std::vector<PlayerScreen>                       m_playersScreen;
                std::vector<sf::Vector2f>                       m_coordPlayerScreen;


};
#endif
