#include "header/car.hpp"

Car::Car() : m_body{0.79}, m_engine{0.17}, m_tyres{0.63}, m_fuel{0.27}, m_frontMissile{false}, m_rearMissile{false},
  m_highSpeed{false}, m_turboCharger{false}, m_spinAssist{false}, m_sideArmour{false},
  m_powerSteering{false}, m_type{Car::Type::Taraco_Neoroder}
{

}

void Car::updateCarLimits()
{
    m_center.x = m_shape.left + (m_shape.width / 2);
    m_center.y = m_shape.top + (m_shape.height / 2);

    m_carLimits[0].x = m_center.x - (m_shape.width / 2);
    m_carLimits[0].y = m_center.y - (m_shape.height / 2);
    m_carLimits[1].x = m_center.x + (m_shape.width / 2);
    m_carLimits[1].y = m_center.y - (m_shape.height / 2);
    m_carLimits[2].x = m_center.x - (m_shape.width / 2);
    m_carLimits[2].y = m_center.y + (m_shape.height / 2);
    m_carLimits[3].x = m_center.x + (m_shape.width / 2);
    m_carLimits[3].y = m_center.y + (m_shape.height / 2);
    m_carLimits[4].x = m_center.x ;
    m_carLimits[4].y = m_center.y + (m_shape.height / 2);
    m_carLimits[5].x = m_center.x ;
    m_carLimits[5].y = m_center.y - (m_shape.height / 2);
    m_carLimits[6].x = m_center.x - (m_shape.width / 2);
    m_carLimits[6].y = m_center.y;
    m_carLimits[7].x = m_center.x + (m_shape.width / 2);
    m_carLimits[7].y = m_center.y;
    m_carLimits[8].x = m_center.x - (m_shape.width / 4);
    m_carLimits[8].y = m_center.y - (m_shape.height / 2);
    m_carLimits[9].x = m_center.x + (m_shape.width / 4);
    m_carLimits[9].y = m_center.y - (m_shape.height / 2);
    m_carLimits[10].x = m_center.x - (m_shape.width / 4);
    m_carLimits[10].y = m_center.y + (m_shape.height / 2);
    m_carLimits[11].x = m_center.x + (m_shape.width / 4);
    m_carLimits[11].y = m_center.y + (m_shape.height / 2);
    float sinusAngle = sinf(m_angle);
    float cosinusAngle = cosf(m_angle);
    for (auto& point : m_carLimits) {
        point = rotateCarCorners(point, m_center, cosinusAngle, sinusAngle);
    }
}

sf::Vector2f Car::rotateCarCorners(const sf::Vector2f& corner, const sf::Vector2f& center,const float cosinusAngle, const float sinusAngle)
{
    sf::Vector2f newCoord;
    newCoord.x = ((corner.x - center.x) * static_cast<float>(cosinusAngle)) - ((corner.y - center.y) * static_cast<float>(sinusAngle)) + center.x;
    newCoord.y = ((corner.x - center.x) * static_cast<float>(sinusAngle)) + ((corner.y - center.y) * static_cast<float>(cosinusAngle)) + center.y;
    return newCoord;
}

sf::Vector2f Car::getCarLimit( unsigned int cornerNumber) const
{
    return m_carLimits[cornerNumber];
}

/*
void car::move()
{
    if(m_speed + m_acceleration < m_maxSpeed) { m_speed += m_acceleration; }
    m_center.x += (cosf(m_angle * static_cast<float>(M_PI) / 180.0f) * m_speed);
    m_center.y -= -1 * (sinf(m_angle * static_cast<float>(M_PI) / 180.0f) * m_speed);
    updateCarLimits();
}
*/
void Car::move()
{
    if(m_speed + m_acceleration < m_maxSpeed) { m_speed += m_acceleration; }
    m_center.x -= cosf(m_angle) * m_speed;    // radians
    m_center.y += sinf(m_angle) * m_speed;    // radians
    updateCarLimits();
}

void Car::findWayToTarget(const sf::Vector2f& target)
{
    float beta = m_angle - atan2f(target.x - m_center.x, -target.y + m_center.y);
    if(fabsf(beta) > 0.2f) {
        if (sinf(beta) < 0) {
            m_angle += 0.01f;
        }
        else {
            m_angle -= 0.01f;
        }
    }
    else {
        if (sinf(beta) < 0) {
            m_angle += 0.1f;
        }
        else {
            m_angle -= 0.1f;
        }
    }
}

bool Car::isInTargetArea(const sf::Vector2f& target)
{
    if ((m_center.x - target.x) * (m_center.x - target.x) + (m_center.y - target.y) * (m_center.y - target.y) < 25*25)
        return true;    //  à a faire si true dans world  ----------     n = (n + 1) % num;
    else
        return false;
}

void Car::turnLeft()
{
    m_angle -= 2.f;
}

void Car::turnRight()
{
    m_angle += 6.f;
}

void Car::speedUp()
{
    if(m_speed < 3.f)
        m_speed += 0.05f;
}

void Car::speedDown()
{
    if(m_speed > 0.f)
        m_speed -= 0.05f;
}

void Car::setTheCar()
{
   // prendre x, y, angle dans json pour chaque circuit
}

void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
//    sf::Transform states;
	states.transform *= getTransform();
	draw(target, states);
}

Car::Type Car::getType() const
{
    return m_type;
}

float Car::getAngle() const
{
    return m_angle;
}

float Car::getSpeed() const
{
    return m_speed;
}

float Car::getMaxSpeed() const
{
    return m_maxSpeed;
}

float Car::getAcceleration() const
{
    return m_acceleration;
}

sf::Vector2f& Car::getCenter()
{
    return m_center;
}

int Car::getElevation() const
{
    return m_elevation;
}

float Car::getBodyState() const
{
    return m_body;
}

float Car::getEngineState() const
{
    return m_engine;
}

float Car::getTyresState() const
{
    return m_tyres;
}

float Car::getFuelState() const
{
    return m_fuel;
}

bool Car::getIsFrontMissileEquiped() const
{
    return m_frontMissile;
}

bool Car::getIsRearMissileEquiped() const
{
    return m_rearMissile;
}

bool Car::getIsHighSpeedKitEquiped() const
{
    return m_highSpeed;
}

bool Car::getIsTurboChargerKitEquiped() const
{
    return m_turboCharger;
}

bool Car::getIsRetrorKitEquiped() const
{
    return m_retro;
}

bool Car::getIsSpinAssistKitEquiped() const
{
    return m_spinAssist;
}

bool  Car::getIsSideArmourKitEquiped() const
{
    return m_sideArmour;
}

bool Car::getIsPowerSteeringKitEquiped() const
{
    return m_powerSteering;
}

void Car::setType(Type type)
{
    m_type = type;
}

void Car::setShape(sf::FloatRect& shape)
{
    m_shape = shape;
}

void  Car::setAngle(float angle)
{
    m_angle = angle;
}

void Car::setSpeed(float speed)
{
    m_speed = speed;
}

void Car::setMaxSpeed(float maxSpeed)
{
    m_maxSpeed = maxSpeed;
}

void Car::setAcceleration(float acceleration)
{
    m_acceleration = acceleration;
}

void Car::setCenter(sf::Vector2f& coords)
{
    m_center = coords;
}

void Car::setElevation(int elevation)
{
    m_elevation = elevation;
}

void Car::setBodyState(float body)
{
    m_body = body;
}

void Car::setEngineState(float engine)
{
    m_engine = engine;
}

void Car::setTyresState(float tyres)
{
    m_tyres = tyres;
}

void Car::setFuelState(float fuel)
{
    m_fuel = fuel;
}

void Car::setIsFrontMissileEquiped(bool frontMissile)
{
    m_frontMissile = frontMissile;
}

void Car::setIsRearMissileEquiped(bool rearMissile)
{
    m_rearMissile = rearMissile;
}

void Car::setIsHighSpeedKitEquiped(bool highSpeed)
{
    m_highSpeed = highSpeed;
}

void Car::setIsTurboChargerKitEquiped(bool turboCharger)
{
    m_turboCharger = turboCharger;
}

void Car::setIsRetroKitEquiped(bool retro)
{
    m_retro = retro;
}

void Car::setIsSpinAssistKitEquiped(bool spinAssist)
{
    m_spinAssist = spinAssist;
}

void  Car::setIsSideArmourKitEquiped(bool sideArmour)
{
    m_sideArmour = sideArmour;
}

void Car::setIsPowerSteeringKitEquiped(bool powerSteering)
{
    m_powerSteering = powerSteering;
}

