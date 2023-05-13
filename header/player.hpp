#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "header/car.hpp"

class Player
{
    public :
        enum levelDifficulty                            {normal = 0, medium, hard};
                                                        Player();
                                                        Player(const bool human,const int ID, const std::string pseudo, const sf::Texture&);

        bool                                            human;
        unsigned int                                    ID;
        unsigned int                                    joystickID;
        struct PlayserCursor {
            sf::Vector2f                                coord;
            sf::Sprite                                  sprite;
        };
        PlayserCursor                                   playerCursor;
        levelDifficulty                                 gameDifficulty;               ///////////////////////  repercuter sur les autres en muti
        std::string                                     pseudo;
        int                                             money;
        Car                                             car;

    private :
        void                                            setGameDifficulty(const std::string pseudo);
        void                                            setPlayerMoney(const std::string pseudo);
        void                                            setJoystickID(const int ID);
        void                                            setPlayerOwnMouseCoord();
        void                                            setPlayerCursor(const sf::Texture& cursorTexture);
};
#endif // PLAYER_HPP
