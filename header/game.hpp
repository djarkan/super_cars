#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <array>
#include <vector>
#include "header/player.hpp"
#include <assetcontainer/assetcontainer.hpp>
#include "header/assetkey.hpp"
#include "header/inputs.hpp"
#include "button/button/button.hpp"
#include "button/simpleclickbox/simpleclickbox.hpp"
#include "button/complexClickBox/complexClickBox.hpp"
#include "jsonfile/jsonfile.hpp"

class Game {
    public:
                                                                        Game(sf::RenderWindow& window, Player& player, unsigned int language);
        void                                                            launch();
        void                                                            loadTextures();
        std::string                                                     enterPseudo();
        void                                                            loading(const sf::Int32 delay);
        bool                                                            setHandCursor();


    private:
        sf::RenderWindow&                                               m_window;
        mylib::AssetContainer<textures::ID, sf::Texture>                m_textureContaigner;
        std::string                                                     m_pseudo;
        unsigned int                                                    m_gameLevel;
        unsigned int                                                    m_language;
        std::array<bool, 9>                                             m_raceListState;
        std::array<std::unique_ptr<mylib::SimpleClickBox>, 2>           m_simpleClickBoxes;
        std::array<std::unique_ptr<mylib::ComplexClickBox>, 3>          m_ComplexClickBoxes;
        std::array<std::unique_ptr<mylib::Button>, 9>                   m_races;
        Player&                                                         m_player;
        Inputs                                                          m_inputs;
        Command                                                         m_command;
        mylib::JsonFile                                                 m_languageJson;
        sf::Cursor                                                      m_handCursor;
        sf::Image                                                       m_handCursorImage;

        void                                                            drawRacePannel();
        void                                                            loadClickBoxes();
        void                                                            loadButtons();
        void                                                            loadSimpleClickBoxes();
        void                                                            loadComplexClickBoxes();
        int                                                             checkButtons();
        void                                                            race(int whatRace);


};

#endif
