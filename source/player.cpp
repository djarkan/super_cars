#include "header/player.hpp"

#include <SFML/Window/Mouse.hpp>

Player::Player()
{

}

Player::Player(const bool human) : m_human{human}, m_money{5000} , m_car(human)
{
    if(m_human) { setJoystickID(); }
}

Player::Player(const bool human, const std::string pseudo) : m_human{human}, m_name{pseudo}, m_money{5000} , m_car(human)
{
    if(m_human) { setJoystickID(); }
}

void Player::setGameDifficulty()
{
    if(m_name == "ODIE") {
        m_gameDifficulty = LevelDifficulty::medium;
        setCarType(Car::Type::Vaug_Interceptor2);
        m_money = 20000;
    }
    else {
        if(m_name == "BIGC") {
            m_gameDifficulty = LevelDifficulty::hard;
            setCarType(Car::Type::Retron_Parsec_Turbo5);
            m_money = 50000;
        }
        else {
            m_gameDifficulty = LevelDifficulty::normal;
            setCarType(Car::Type::Taraco_Neoroder);
            if(m_name == "RICH") { m_money = 500000; }
            else { m_money = 5000; }
        }
    }
}

void Player::levelupGameDifficulty()
{
    if(m_gameDifficulty == LevelDifficulty::normal) { m_gameDifficulty = LevelDifficulty::medium; }
    if(m_gameDifficulty == LevelDifficulty::medium) { m_gameDifficulty = LevelDifficulty::hard; }
}

void Player::setJoystickID()
{
    unsigned int pluggedJoystickNumber{0};
    for(auto i = 0; i < sf::Joystick::Count; ++i)
    if(sf::Joystick::isConnected(i)) { ++pluggedJoystickNumber; }
    if(pluggedJoystickNumber > 1) {                                                                 // A REFAIRE EN BITMAP !!!!!!!!!!!!!!!!!!!!!!!
        std::cout <<"Press fire to assign your joystick."<< std::endl;                              // PRENDRE DANS FICHIER TRADUC !!!!
        bool JoystickButtonPressed{false};
        while(!JoystickButtonPressed){
     sf::Joystick::update();
            for(auto id = 0; id < sf::Joystick::Count; ++id ) {
                if(sf::Joystick::isButtonPressed(id, 0)) {
                    JoystickButtonPressed = true;
                    m_joystickID = id;
                }
            }
        }
    }
    else {
        m_joystickID = 0;
    }
}

void Player::setName(const std::string& name)
{
    m_name = name;
}

unsigned int Player::getMoney()
{
    return m_money;
}

std::string Player::getName()
{
    return m_name;
}

unsigned int Player::getJoystickID()
{
    return m_joystickID;
}

unsigned int Player::getGameDifficulty()
{
    return m_gameDifficulty;
}

Car::Type Player::getCarType() const
{
    return m_car.getType();
}

float Player::getCarAngle() const
{
    return m_car.getAngle();
}

float Player::getCarSpeed() const
{
    return m_car.getSpeed();
}

float Player::getCarMaxSpeed() const
{
    return m_car.getMaxSpeed();
}

float Player::getCarAcceleration() const
{
    return m_car.getAcceleration();
}

float Player::getCarSpeedLimiter() const
{
   return m_car.getSpeedLimiter();
}

sf::Vector2f Player::getCarPosition()
{
    return m_car.getPosition();
}

sf::Vector2f Player::getCarCenter()
{
    return m_car.getCenter();
}

sf::FloatRect Player::getCarShape() const
{
    return m_car.getShape();
}

int Player::getCarElevation() const
{
    return m_car.getElevation();
}

float Player::getCarBodyState() const
{
    return m_car.getBodyState();
}

float Player::getCarEngineState() const
{
    return m_car.getEngineState();
}

float Player::getCarTyresState() const
{
    return m_car.getTyresState();
}

float Player::getCarFuelState() const
{
    return m_car.getFuelState();
}

bool Player::getCarIsFrontMissileEquiped() const
{
    return m_car.getIsFrontMissileEquiped();
}

bool Player::getCarIsRearMissileEquiped() const
{
    return m_car.getIsRearMissileEquiped();
}

bool Player::getCarIsHighSpeedKitEquiped() const
{
    return m_car.getIsHighSpeedKitEquiped();
}

bool Player::getCarIsTurboChargerKitEquiped() const
{
    return m_car.getIsTurboChargerKitEquiped();
}

bool Player::getCarIsSpinAssistKitEquiped() const
{
    return m_car.getIsSpinAssistKitEquiped();
}

bool Player::getCarIsRetroKitEquiped() const
{
    return m_car.getIsRetrorKitEquiped();
}

bool  Player::getCarIsSideArmourKitEquiped() const
{
    return m_car.getIsSideArmourKitEquiped();
}

bool Player::getCarIsPowerSteeringKitEquiped() const
{
    return m_car.getIsPowerSteeringKitEquiped();
}

unsigned int Player::getCarColor()
{
    return m_car.getColor();
}

const Car& Player::getCar() const
{
    return m_car;
}

void Player::setCarType(Car::Type type)
{
    m_car.setType(type);
}

void Player::setMoney(unsigned int money)
{
    m_money = money;
}

void Player::setCarShape(sf::FloatRect rect)
{
    m_car.setShape(rect);
}

void Player::setCarCenter(float centerX, float centerY)
{
    m_car.setCenter(centerX, centerY);
}

void Player::setCarCenter(sf::Vector2f& coords)
{
    m_car.setCenter(coords);
}

void  Player::setCarAngle(float angle)
{
    m_car.setAngle(angle);
}

void Player::setCarSpeed(float speed)
{
    m_car.setSpeed(speed);
}

void Player::setCarMaxSpeed(float maxSpeed)
{
    m_car.setMaxSpeed(maxSpeed);
}

void Player::setCarAcceleration(float acceleration)
{
    m_car.setAcceleration(acceleration);
}

void Player::setCarElevation(int elevation)
{
    m_car.setElevation(elevation);
}

void Player::setCarSpeedLimiter(float speedLimiter)
{
    m_car.setSpeedLimiter(speedLimiter);
}

void Player::setCarBodyState(float body)
{
    m_car.setBodyState(body);
}

void Player::setCarEngineState(float engine)
{
    m_car.setEngineState(engine);
}

void Player::setCarTyresState(float tyres)
{
    m_car.setTyresState(tyres);
}

void Player::setCarFuelState(float fuel)
{
    m_car.setFuelState(fuel);
}

void Player::setCarFrontMissile(bool frontMissile)
{
    m_car.setIsFrontMissileEquiped(frontMissile);
}

void Player::setCarRearMissile(bool rearMissile)
{
    m_car.setIsRearMissileEquiped(rearMissile);
}

void Player::setCarHighSpeedKit(bool highSpeed)
{
    m_car.setIsHighSpeedKitEquiped(highSpeed);
}

void Player::setCarTurboChargerKit(bool turboCharger)
{
    m_car.setIsTurboChargerKitEquiped(turboCharger);
}

void Player::setCarSpinAssistKit(bool spinAssist)
{
    m_car.setIsSpinAssistKitEquiped(spinAssist);
}

void Player::setCarRetroKit(bool retro)
{
     m_car.setIsRetroKitEquiped(retro);
}

void  Player::setCarSideArmourKit(bool sideArmour)
{
    m_car.setIsSideArmourKitEquiped(sideArmour);
}

void Player::setCarPowerSteeringKit(bool powerSteering)
{
    m_car.setIsPowerSteeringKitEquiped(powerSteering);
}

void Player::setCarColor(unsigned int color)
{
    m_car.setColor(color);
}

void Player::setTexture(sf::Texture* carTexture)
{
    m_car.setTexture(carTexture);
}

void Player::setCarPosition(const sf::Vector2f& position)
{
    m_car.setPosition(position);
}

void Player::setCarStartFrame()
{
    m_car.setStartFrame();
}

void Player::moveCar()
{
    m_car.move();
}

void Player::turnCarLeft()
{
    m_car.turnLeft();
}

void Player::turnCarRight()
{
    m_car.turnRight();
}

void Player::accelerate()
{
    m_car.accelerate();
}

void Player::decelerate()
{
    m_car.decelerate();
}
