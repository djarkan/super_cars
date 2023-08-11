#include <header/inputs.hpp>

Inputs::Inputs(sf::RenderWindow& theWindow, const unsigned int joystickID): m_window{theWindow}, m_commandQueue(), m_currentJoystickButton{0}, m_joystickID{joystickID}
{

}

void Inputs::readInput()
{
    Command command;
    command.coords = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
    sf::Event event;
     while(m_window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed :
                    m_window.close();
                    break;
                case sf::Event::LostFocus :
                    command.action = CommandType::lostFocus;
                    break;
                case sf::Event::GainedFocus :
                    command.action = CommandType::gainFocus;
                    break;
                case sf::Event::KeyPressed :
                    command.action = CommandType::keyboardPressed;
                    if(event.key.code == sf::Keyboard::Key::Space) { command.letter = ' '; }
                    break;
                case sf::Event::KeyReleased :
                    command.action = CommandType::keyboardReleased;
                    if(event.key.code == sf::Keyboard::Key::Escape) { command.letter = 27; }
                    break;
                case sf::Event::JoystickButtonPressed :
                    if(event.joystickButton.joystickId == m_joystickID) {
                        command.coords = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
                        command.action = CommandType::joystiskButtonPressed;
                        command.joystickButtonID = event.joystickButton.button;
                        m_currentJoystickButton = event.joystickButton.button;
                    }
                    break;
                case sf::Event::JoystickButtonReleased :
                    if(event.joystickButton.joystickId == m_joystickID) {
                        command.coords = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
                        command.action = CommandType::joystiskButtonReleased;
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Button::Left){
                        command.coords = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
                        command.action = CommandType::mouseLeftButtonPressed;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if(event.mouseButton.button == sf::Mouse::Button::Left){
                        command.coords = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
                        command.action = CommandType::mouseLeftButtonReleased;
                    }
                    break;
                default:
                    break;
            }
            m_commandQueue.push(command);
            clearCommand(command);
        }
        checkLastJoystickButtonUsed(command);
        checkAxis(command);
}

Command Inputs::getInput()
{
    if(!m_commandQueue.isEmpty()) { return m_commandQueue.pop(); }
    else {
        Command command;
        command.action = CommandType::noAction;
        return command;
    }
}

bool Inputs::isCommandAvailable()
{
    return !m_commandQueue.isEmpty();
}

void Inputs::checkAxis(Command& command)
{
    if(sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::X) < -50 || sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::X) > 50) {
            command.offsetX = sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::X);
            command.action = CommandType::joystiskMoved;
        }
        if(sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::Y) < -50 || sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::Y) > 50) {
            command.offsetY = sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::Y);
            command.action = CommandType::joystiskMoved;
        }
        if(sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::U) < -50 || sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::U) > 50) {
            command.offsetX = sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::U);
            command.action = CommandType::joystiskMoved;
        }
        if(sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::R) < -50 || sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::R) > 50) {
            command.offsetY = sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::R);
            command.action = CommandType::joystiskMoved;
        }

        if(sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::PovX) < -50 || sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::PovX) > 50) {
            command.offsetX = sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::PovX);
            command.action = CommandType::joystiskMoved;
        }

        if(sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::PovY) < -50 || sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::PovY) > 50) {
            command.offsetY = sf::Joystick::getAxisPosition(m_joystickID, sf::Joystick::Axis::PovY);
            command.action = CommandType::joystiskMoved;
        }
        m_commandQueue.push(command);
        clearCommand(command);
}

void Inputs::checkLastJoystickButtonUsed(Command& command)
{
    if (sf::Joystick::isButtonPressed(m_joystickID, m_currentJoystickButton) && command.action != CommandType::joystiskButtonPressed) {
        command.action = CommandType::joystiskButtonPushed;
        command.joystickButtonID = m_currentJoystickButton;
        m_commandQueue.push(command);
        clearCommand(command);
    }
}

void Inputs::clearCommand(Command& command)
{
    command.action = CommandType::noAction;
    command.coords = sf::Vector2f(0, 0);
    command.joystickButtonID = 99;
    command.letter = 0;
    command.offsetX = 0;
    command.offsetY = 0;
}
