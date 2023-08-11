#ifndef INTRO_HPP
#define INTRO_HPP

#include <array>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include "assetcontainer/assetcontainer.hpp"
#include "header/assetkey.hpp"
#include "triple/triple.hpp"
#include "jsonfile/jsonfile.hpp"
#include "header/inputs.hpp"

class Intro
{
    public :
                                                                Intro();
        void                                                    loadAssets();
        void                                                    launchIntro();
        void                                                    launchGame();

    private :

        mylib::AssetContainer<textures::ID, sf::Texture>        m_textureContaigner;
        mylib::JsonFile                                         m_jsonConfig;
        sf::RenderWindow                                        m_window;
        std::array<std::pair<std::string, int32_t>, 9>          m_bestLapsTimes;
        std::array<mylib::Triple<std::string, int, int>, 6>     m_bestWinnersList;
        unsigned int                                            m_language;
        Player                                                  m_player;
        Inputs                                                  m_inputs;

        void                                                    gremlinsAndMagneticFields();
        void                                                    superCars();
        bool                                                    guyAndGirl();
        bool                                                    ronAndNancy();
        bool                                                    creditsOriginal();
        bool                                                    creditsAdditionnal();
        void                                                    loading(const sf::Int32 delay);
        void                                                    buildRonAndNancySpeech(std::array<std::string, 21>& speechArray);
        bool                                                    isIntroSkipped();
};
#endif // PLAYER_HPP
