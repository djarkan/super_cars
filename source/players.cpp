#include "header/players.hpp"

#include <SFML/Graphics/Texture.hpp>

#include "text/bitmapfont/bitmapFont.hpp"
#include "text/bitmaptext/bitmapText.hpp"
#include "timer/timer.hpp"
#include "header/command.hpp"
#include "header/commandqueue.hpp"
#include "header/inputs.hpp"

Players::Players(sf::RenderWindow& window) : m_window{window}, m_humanPlayer(true, "RICH")
{
    mylib::JsonFile configJson("config.json");
    m_language = configJson.m_Root["language"].asInt();
    switch(m_language) {
        case 0 :
            m_languageJson.loadJsonFile("language/english/player.json");
            break;
        case 1:
            m_languageJson.loadJsonFile("language/usa/player.json");
            break;
        case 2 :
            m_languageJson.loadJsonFile("language/french/player.json");
            break;
        default :
            break;
    }
    if(!m_carsTexture.loadFromFile("graphics/cars/cars.png")) { std::cout << m_languageJson.m_Root["error"].asString() << std::endl; }
}

std::string Players::enterPseudo()
{
    std::string pseudo = "RICH";  m_humanPlayer.getName();
    sf::Sprite background;
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("graphics/intro/enter_name.png");
    background.setTexture(backgroundTexture);
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    sf::Sprite clearedGlyph;
    clearedGlyph.setTexture(backgroundTexture);
    clearedGlyph.setTextureRect(sf::IntRect(250, 335, 32, 32));
    mylib::BitmapFont font;
    sf::Texture glyphTexture;
    font.loadTexture("graphics/bitmap_fonts/golden_font_32x32.png");
    font.setSize(32, 32);
    mylib::BitmapText bitmapText;
    bitmapText.setFont(&font);
    mylib::Timer flashingLetter(100);
    unsigned int currentLetter{0};
    flashingLetter.startTimer();
    bool clearGlyph{false};
    mylib::Timer commandTimer(200);
    commandTimer.startTimer();
    Command command;
    Inputs inputs(m_window);
    while(command.action != CommandType::joystiskButtonPressed && command.action != CommandType::mouseLeftButtonPressed) {
        if(commandTimer.isTimeElapsed()) {
            inputs.readInput(m_humanPlayer.getJoystickID());
            commandTimer.restartTimer();
        }
        if(inputs.isCommandAvailable()) { command = inputs.getInput(); }
        m_window.clear();
        m_window.draw(background);
        bitmapText.setText(m_languageJson.m_Root["pseudo"][0].asString());
        sf::FloatRect bounds = bitmapText.getLocalBounds();
        bitmapText.setPosition(320.f - (bounds.width / 2.f), 242.f);
        m_window.draw(bitmapText);
        bitmapText.setText(m_languageJson.m_Root["pseudo"][1].asString());
        bounds = bitmapText.getLocalBounds();
        bitmapText.setPosition(320.f - (bounds.width / 2.f), 278.f);
        m_window.draw(bitmapText);
        bitmapText.setText(pseudo);
        bitmapText.setPosition(257, 335);
        m_window.draw(bitmapText);
        if(command.action == CommandType::joystiskMoved) {
            if(command.offsetX > 75) {
                currentLetter++;
                if(currentLetter > 3) { currentLetter = 0; }
            }
            if(command.offsetX < -75) {
                if(currentLetter == 0) { currentLetter = 3; }
                else { currentLetter--; }
            }
            if(command.offsetY < -75) {
                if(pseudo[currentLetter] == 'Z') { pseudo[currentLetter] = 'A';}
                else { pseudo[currentLetter] = static_cast<char>(pseudo[currentLetter] + 1); }
                bitmapText.setText(pseudo);
            }
            if(command.offsetY > 75) {
                if(pseudo[currentLetter] == 'A') { pseudo[currentLetter] = 'Z';}
                else { pseudo[currentLetter] = static_cast<char>(pseudo[currentLetter] - 1); }
                bitmapText.setText(pseudo);
            }
        inputs.clearCommand(command);
        }
        if(clearGlyph) {
            clearedGlyph.setPosition(257.f + (static_cast<float>(currentLetter) * 32.f), 335);
            m_window.draw(clearedGlyph);
        }
        if(flashingLetter.isTimeElapsed()) {
            if(clearGlyph) { clearGlyph = false; }
            else { clearGlyph = true; }
            flashingLetter.restartTimer();
        }
        m_window.display();
    }
    m_window.draw(bitmapText);
    m_window.display();
    return pseudo;
}

Player& Players::getHumanPlayer()
{
    return m_humanPlayer;
}

unsigned int Players::getHumanJoystick()
{
    return m_humanPlayer.getJoystickID();
}

unsigned int Players::getGameDifficulty()
{
    return m_humanPlayer.getGameDifficulty();
}

unsigned int Players::getRacerNumber()
{
    return m_players.size();
}

sf::Vector2f Players::getHumanCarPosition()
{
    return m_humanPlayer.getCarPosition();
}

const Car& Players::getPlayerCar(unsigned int index)
{
    return m_players[index]->getCar();
}

void Players::setHumanName(bool won)
{
    m_humanPlayer.setName(enterPseudo());
    if(!won) { m_humanPlayer.setGameDifficulty(); }
}

void Players::setCarPosition(sf::Vector2f& coords, unsigned int index)
{
    m_players[index]->setCarPosition(coords);
}

void  Players::setCarAngle(double angle, unsigned int index)
{
    m_players[index]->setCarAngle(angle);
}

void Players::setCarStartFrame(unsigned int index)
{
    m_players[index]->setCarStartFrame();
}

void Players::buildPlayers(unsigned int completedRaces, unsigned int lastRaceRanking)
{
    mylib::JsonFile playersFileConfig("data/race/racers.json");
    mylib::JsonFile carsFileConfig("data/cars/cars.json");
    unsigned int AIracerNumber = playersFileConfig.m_Root["races"][completedRaces]["names"].size();
    unsigned int totalRacers{AIracerNumber + 1};
    unsigned int humanStartPlace{lastRaceRanking - 1};
    unsigned int AIindex{0};

    for(unsigned int index = 0; index < totalRacers; ++index) {
        Player* ptr = new Player;
        if(index == humanStartPlace) {

            if(m_humanPlayer.getCarIsHighSpeedKitEquiped()) { m_humanPlayer.setCarMaxSpeed(carsFileConfig.m_Root["model"][m_humanPlayer.getCarType()]["maxSpeedKit"].asFloat()); }
            else { m_humanPlayer.setCarMaxSpeed(carsFileConfig.m_Root["model"][m_humanPlayer.getCarType()]["maxSpeed"].asFloat()); }
            ptr = &m_humanPlayer;
            m_players.emplace(m_players.begin(), ptr);
        }
        else {
            ptr->setName(playersFileConfig.m_Root["races"][completedRaces]["names"][AIindex].asString());
            ptr->setCarType(static_cast<Car::Type>(playersFileConfig.m_Root["races"][completedRaces]["cartype"][AIindex].asInt()));
            ptr->setCarSpeedLimiter(playersFileConfig.m_Root["races"][completedRaces]["carspeedlimiter"][AIindex].asFloat());
            ptr->setCarMaxSpeed(carsFileConfig.m_Root["model"][ptr->getCarType()]["maxSpeed"].asFloat());
            ptr->setCarColor(0);
            m_players.emplace(m_players.begin(), ptr);
            ++AIindex;
        }
    }
    for(auto& elem : m_players) {
        elem->setTexture(&m_carsTexture);
        elem->setCarCenter(carsFileConfig.m_Root["model"][elem->getCarType()]["centerx"].asFloat(), carsFileConfig.m_Root["model"][elem->getCarType()]["centery"].asFloat());
        elem->setCarShape(sf::FloatRect(0, 0, carsFileConfig.m_Root["model"][elem->getCarType()]["width"].asFloat(), carsFileConfig.m_Root["model"][elem->getCarType()]["heigth"].asFloat()));

    }
}

void Players::levelupGameDifficulty()
{
    m_humanPlayer.levelupGameDifficulty();
}

void Players::moveCars()
{
   // for(auto& elem : m_players) {
      //  elem->setCarAngle( elem->getCarAngle() + 0.087);
        m_humanPlayer.turnCarRight();
std::cout << "car angle is : " << m_humanPlayer.getCarAngle() << " rad" << std::endl;
  //  }
}

/*
float Players::getCarAngle(Player& player) const
{
    return player.getCarAngle();
}

float Players::getCarSpeed(Player& player) const
{
    return player.getCarSpeed();
}

float Players::getCarMaxSpeed(Player& player) const
{
    return player.getCarMaxSpeed();
}

float Players::getCarAcceleration(Player& player) const
{
    return player.getCarAcceleration();
}

sf::Vector2f& Players::getCarCenter(Player& player)
{
    return player.getCarCenter();
}

int Players::getCarElevation(Player& player) const
{
    return player.getCarElevation();
}

float Players::getCarBodyState(Player& player) const
{
    return player.getCarBodyState();
}

float Players::getCarEngineState(Player& player) const
{
    return player.getCarEngineState();
}

float Players::getCarTyresState(Player& player) const
{
    return player.getCarTyresState();
}

float Players::getCarFuelState(Player& player) const
{
    return player.getCarFuelState();
}

bool Players::getCarIsFrontMissileEquiped(Player& player) const
{
    return player.getCarIsFrontMissileEquiped();
}

bool Players::getCarIsRearMissileEquiped(Player& player) const
{
    return player.getCarIsRearMissileEquiped();
}

bool Players::getCarIsHighSpeedKitEquiped(Player& player) const
{
    return player.getCarIsHighSpeedKitEquiped();
}

bool Players::getCarIsTurboChargerKitEquiped(Player& player) const
{
    return player.getCarIsTurboChargerKitEquiped();
}

bool Players::getCarIsRetroKitEquiped(Player& player) const
{
    return player.getCarIsRetroKitEquiped();
}

bool Players::getCarIsSpinAssistKitEquiped(Player& player) const
{
    return player.getCarIsSpinAssistKitEquiped();
}

bool Players::getCarIsSideArmourKitEquiped(Player& player) const
{
    return player.getCarIsSideArmourKitEquiped();
}

bool Players::getCarIsPowerSteeringKitEquiped(Player& player) const
{
    return player.getCarIsPowerSteeringKitEquiped();
}

void Players::setCarSpeed(float speed, Player& player)
{
    player.setCarSpeed(speed);
}

void  Players::setCarMaxSpeed(float maxSpeed, Player& player)
{
    player.setCarMaxSpeed(maxSpeed);
}

void Players::setCarAcceleration(float acceleration, Player& player)
{
    player.setCarAcceleration(acceleration);
}

void Players::setCarElevation(int elevation, Player& player)
{
    player.setCarElevation(elevation);
}

void Players::setCarBodyState(float body, Player& player)
{
    player.setCarBodyState(body);
}

void Players::setCarEngineState(float engine, Player& player)
{
    player.setCarEngineState(engine);
}

void Players::setCarTyresState(float tyres, Player& player)
{
    player.setCarTyresState(tyres);
}

void Players::setCarFuelState(float fuel, Player& player)
{
    player.setCarFuelState(fuel);
}

void Players::setCarFrontMissile(bool frontMissile, Player& player)
{
    player.setCarFrontMissile(frontMissile);
}

void Players::setCarRearMissile(bool rearMissile, Player& player)
{
    player.setCarRearMissile(rearMissile);
}

void Players::setCarHighSpeedKit(bool highSpeed, Player& player)
{
    player.setCarHighSpeedKit(highSpeed);

}

void Players::setCarRetroKit(bool retro, Player& player)
{
    player.setCarRetroKit(retro);
}

void Players::setCarTurboChargerKit(bool turboCharger, Player& player)
{
    player.setCarTurboChargerKit(turboCharger);
}

void Players::setCarSpinAssistKit(bool spinAssist, Player& player)
{
    player.setCarSpinAssistKit(spinAssist);
}

void Players::setCarSideArmourKit(bool sideArmour, Player& player)
{
    player.setCarSideArmourKit(sideArmour);
}

void Players::setCarPowerSteeringKit(bool powerSteering, Player& player)
{
    player.setCarPowerSteeringKit(powerSteering);
}


*/

