#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>


class Inputs {
    public:
         enum class                         Command {turnLeft = 0, handBrakeTurnLeft, turnRight, handBrakeTurnRight, acceleration, selection, noSelection, frontMissile, rearMissile, moveCursor, pause, reset};
                                            Inputs(sf::RenderWindow& theWindow);
        struct Input{
            sf::Vector2f                    offset;                                                                     // offset from current position
            Command                         command;                                                                    // what
            int                             joystckNumber;                                                              // who
        };

        void                                readRaceInput();
        void                                readNotRacingInput();
        Command                             readSelectionInput();
        Input                               getInput(int joystickNumber);
        bool                                isCommandAvailable();

    private:
        sf::RenderWindow&                   m_window;
        sf::Event                           m_event;
        std::vector<Input>                  commandBuffer;
};
#endif


