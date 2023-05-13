#include "header/player.hpp"
#include <SFML/Window/Mouse.hpp>

Player::Player()
{

}

Player::Player(const bool human, const int ID, const std::string pseudo, const sf::Texture& cursorTexture) : human{human}, ID{ID}, pseudo{pseudo}
   //     Player(const bool human, const int ID, const std::string pseudo, const sf::Texture& );
{
    setGameDifficulty(pseudo);
    setPlayerMoney(pseudo);
    setJoystickID(ID);
    setPlayerCursor(cursorTexture);
}

void Player::setGameDifficulty(const std::string pseudo)
{
    if(pseudo == "ODIE") {
        gameDifficulty = levelDifficulty::medium;
    }
    else {
        if(pseudo == "BIGC") {
            gameDifficulty = levelDifficulty::hard;
        }
        else {
            gameDifficulty = levelDifficulty::normal;
        }
    }
}

void Player::setPlayerMoney(const std::string pseudo)
{
    if(pseudo == "ODIE") {
        money = 30000;
    }
    else {
        if(pseudo == "BIGC") {
            money = 50000;
        }
        else {
            if(pseudo == "RICH") {
                money = 500000;
            }
            else {
                money = 10000;
            }
        }
    }
}

void Player::setJoystickID(const int ID)
{
    sf::Joystick joystick;
    unsigned int pluggedJoystickNumber{0};
    for(auto i = 0; i < sf::Joystick::Count; ++i)
    if(joystick.isConnected(i)) {
        ++pluggedJoystickNumber;
    }
    if(pluggedJoystickNumber > 1) {
        std::cout << "Player " << ID << ", press fire to assign your joystick."<< std::endl;
        bool JoystickButtonPressed{false};
        while(!JoystickButtonPressed){
            joystick.update();
            for(auto id = 0; id < sf::Joystick::Count; ++id ) {
                if(joystick.isButtonPressed(id, 0)) {
                    JoystickButtonPressed = true;
                    joystickID = id;
    std::cout << "joystick " << joystickID << " assigned to player " << ID << "." << std::endl;
                }
            }
        }
    }
    else {
        joystickID = 0;
    }
}

void Player::setPlayerCursor(const sf::Texture& cursorTexture)
{
    playerCursor.sprite.setTexture(cursorTexture);
    playerCursor.sprite.setTextureRect(sf::IntRect(0, 260, 32, 32));
    playerCursor.coord.x = 200;
    playerCursor.coord.y = 200;
    switch(ID) {
        case 0 :
            sf::Mouse::setPosition(sf::Vector2i(200, 200));
            break;
        case 1 :
            playerCursor.coord.x = 200 + 600;
            playerCursor.coord.y = 200;
            break;
        case 2 :
            playerCursor.coord.x = 200;
            playerCursor.coord.y = 200 + 400;
            break;
        case 3 :
            playerCursor.coord.x = 200 + 600;
            playerCursor.coord.y = 200 + 400;
            break;
        default :
            break;
    }

}
