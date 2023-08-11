#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>

#include "header/command.hpp"
#include "header/commandqueue.hpp"
#include "header/player.hpp"

class Inputs {
    public:
                                            Inputs(sf::RenderWindow& m_window, const unsigned int joystickID);
        void                                readInput();                                                    // push m_commandQueue
        Command                             getInput();                                                     // pop m_commandQueue
        bool                                isCommandAvailable();
        void                                clearCommand(Command& command);

    private:
        sf::RenderWindow&                   m_window;
        Queue                               m_commandQueue;
        unsigned int                        m_currentJoystickButton;
        const unsigned int                  m_joystickID;

        void                                checkAxis(Command& command);
        void                                checkLastJoystickButtonUsed(Command& command);

};
#endif


