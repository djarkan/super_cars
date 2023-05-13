#include <header/inputs.hpp>

Inputs::Inputs(sf::RenderWindow& theWindow): m_window{theWindow}
{

}

Inputs::Command Inputs::readSelectionInput()
{
    Input input;
    while(m_window.pollEvent(m_event)) {
        switch (m_event.type) {
            case sf::Event::Closed:
                m_window.close();
                break;
                case sf::Event::MouseButtonPressed :
                case sf::Event::JoystickButtonPressed :
                    return Command::selection;
                    break;
                default:
                    break;
            }
    }
    return Command::noSelection;
}

bool Inputs::isCommandAvailable()
{
    if(commandBuffer.size() > 0) { return true; }
    else { return false; }
}

void Inputs::readNotRacingInput()
{
    Input input;
     while(m_window.pollEvent(m_event)) {
            switch (m_event.type) {
                case sf::Event::Closed :
                    m_window.close();
                    break;
                case sf::Event::JoystickMoved:
                    if(m_event.joystickMove.axis == sf::Joystick::Axis::X) {
                        input.command = Command::moveCursor;
                        if(m_event.joystickMove.joystickId == 0) {
                            sf::Vector2i coord(sf::Mouse::getPosition(m_window));
                            input.offset.x = m_event.joystickMove.position + static_cast<float>(coord.x);
                        }
                        else {
                            input.offset.x = m_event.joystickMove.position;
                        }
                        input.joystckNumber = m_event.joystickMove.joystickId;
                        commandBuffer.push_back(input);
                    }
                    else {
                        if(m_event.joystickMove.axis == sf::Joystick::Y) {
                            input.command = Command::moveCursor;
                            if(m_event.joystickMove.joystickId == 0) {
                                sf::Vector2f coord(sf::Mouse::getPosition(m_window));
                                input.offset.y = m_event.joystickMove.position + static_cast<float>(coord.x);
                            }
                        }
                        else {
                            input.offset.x = m_event.joystickMove.position;
                        }
                        input.joystckNumber = m_event.joystickMove.joystickId;
                        commandBuffer.push_back(input);
                    }
                    commandBuffer.push_back(input);
                    break;
                case sf::Event::JoystickButtonPressed :
                    break;
                case sf::Event::MouseButtonPressed:
                    if(m_event.mouseButton.button == sf::Mouse::Button::Left){
                        input.command = Command::selection;
                        input.joystckNumber = 33;                                                           // 33 because event can t know player ID 0 joystick ID so no 33th button so event for player ID 0
                    }
                    break;
                case sf::Event::MouseMoved:
                    input.command = Command::moveCursor;
                    sf::Mouse::setPosition(sf::Vector2i(m_event.mouseMove.x, m_event.mouseMove.y));
                    input.offset.x = 0;
                    input.offset.y = 0;
                    input.joystckNumber = 33;                                                           // 33 because event can t know player ID 0 joystick ID so no 33th button so event for player ID 0
                    break;
                default:
                    break;
            }
        }
}
/*
void Inputs::readRaceInput()
{
    Input input;
     while(m_window->pollEvent(m_event)) {
            switch (m_event.type) {
                case sf::Event::Closed:
                    m_window->close();
                    break;
                case sf::Event::KeyPressed:
                    switch (m_event.key.code)
                    {
                        case sf::Keyboard::Space:
                            input.command = Command::pause;
                            break;
                        case sf::Keyboard::Escape:
                            input.command = Command::reset;
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::JoystickMoved:
                    if(m_event.joystickMove.axis == sf::Joystick::Axis::PovX && m_event.joystickMove.position == 100 && m_event.joystickMove.joystickId == m_joystickID) {
                        m_repeatLimitaionTimer.startTimer();
                        return gameControl::shiftRight;
                    }
                    if(m_event.joystickMove.axis == sf::Joystick::Axis::PovX && m_event.joystickMove.position == -100 && m_event.joystickMove.joystickId == m_joystickID) {
                        m_repeatLimitaionTimer.startTimer();
                        return gameControl::shiftLeft;
                    }
                    if(m_event.joystickMove.axis == sf::Joystick::Axis::PovY && m_event.joystickMove.position == 100 && m_event.joystickMove.joystickId == m_joystickID) {
                        m_repeatLimitaionTimer.startTimer();
                        return gameControl::hardDrop;
                    }
                    if(m_event.joystickMove.axis == sf::Joystick::Axis::PovY && m_event.joystickMove.position == -100 && m_event.joystickMove.joystickId == m_joystickID) {
                        m_repeatLimitaionTimer.startTimer();
                        return gameControl::softDrop;
                    }
                    break;
                case sf::Event::JoystickButtonPressed:
                default:
                    if(sf::Joystick::isButtonPressed(sf::Mouse::Left)){ // à voir si maintient bouton joy emet toujours event joypressed
                        return gameControl::leftMouseButtonHeld;
                    }
                    else{
                        return gameControl::none;
                    }
                    break;
            }
        }
}

Input Inputs::getInput(const int joystickNumber)
{

}
*/
