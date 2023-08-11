#include "header/player.hpp"
#include <SFML/Window/Mouse.hpp>

Player::Player() : m_car()
{
    setJoystickID();
}

Player::Player(const bool human) : m_human{human}, m_money{5000} , m_car()
{
    setJoystickID();
}

Player::Player(const bool human, const std::string pseudo) : m_human{human}, m_name{pseudo}, m_money{5000} , m_car()
{
    setJoystickID();
}

void Player::setGameDifficulty()
{
    if(m_name == "ODIE") {
        m_gameDifficulty = LevelDifficulty::medium;
        setCarType(Car::Type::Vaug_Interceptor2);
        m_car.m_shape = sf::FloatRect(0, 0, 52, 38);
        m_money = 20000;
    }
    else {
        if(m_name == "BIGC") {
            m_gameDifficulty = LevelDifficulty::hard;
            setCarType(Car::Type::Retron_Parsec_Turbo5);
            m_car.m_shape = sf::FloatRect(0, 0, 56, 38);
            m_money = 50000;
        }
        else {
            m_gameDifficulty = LevelDifficulty::normal;
            setCarType(Car::Type::Taraco_Neoroder);
            m_car.m_shape = sf::FloatRect(0, 0, 48, 34);
            if(m_name == "RICH") { m_money = 500000; }
            else { m_money = 5000; }
        }
    }
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

unsigned int Player::getJoystickID()
{
    return m_joystickID;
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

sf::Vector2f& Player::getCarCenter()
{
    return m_car.getCenter();
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

void Player::setCarType(Car::Type type)
{
    m_car.setType(type);
}

void Player::setMoney(unsigned int money)
{
    m_money = money;
}

void Player::setCarShape(sf::FloatRect& shape)
{
    m_car.setShape(shape);
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

void Player::setCarCenter(sf::Vector2f& coords)
{
    m_car.setCenter(coords);
}

void Player::setCarElevation(int elevation)
{
    m_car.setElevation(elevation);
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
