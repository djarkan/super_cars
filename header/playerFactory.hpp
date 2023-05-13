#ifndef PLAYERFACTORY_HPP
#define PLAYERFACTORY_HPP

#include "header/player.hpp"
#include <vector>

class PlayerFactory
{
    public :
                                                                PlayerFactory();
        void                                                    addPlayer(std::string);

    private :
        int                                                     freeID;
        std::vector<Player>                                          playerList;

};
#endif // PLAYER_HPP
