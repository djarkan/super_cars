#include "header/car.hpp"

Car::Car()
{

}

Car::Car(type type) :  m_type{type}
{

}

Car::Car(float x, float y, float w, float h, float angle, float maxSpeed, bool humanPlayer)
: m_x{x}, m_y{y}, m_w{w}, m_h{h},m_angle{angle}, m_maxSpeed{maxSpeed}, m_humanPlayer{humanPlayer}
{
    m_carCenter.x = (m_x + m_x + m_w) / 2;
    m_carCenter.y = (m_y + m_y + m_h) / 2;
    m_acceleration = 1;
}

void Car::updateCarLimits()
{
    m_carLimits[0].x = m_carCenter.x - (m_w / 2);
    m_carLimits[0].y = m_carCenter.y - (m_h / 2);
    m_carLimits[1].x = m_carCenter.x + (m_w / 2);
    m_carLimits[1].y = m_carCenter.y - (m_h / 2);
    m_carLimits[2].x = m_carCenter.x - (m_w / 2);
    m_carLimits[2].y = m_carCenter.y + (m_h / 2);
    m_carLimits[3].x = m_carCenter.x + (m_w / 2);
    m_carLimits[3].y = m_carCenter.y + (m_h / 2);
    m_carLimits[4].x = m_carCenter.x ;
    m_carLimits[4].y = m_carCenter.y + (m_h / 2);
    m_carLimits[5].x = m_carCenter.x ;
    m_carLimits[5].y = m_carCenter.y - (m_h / 2);
    m_carLimits[6].x = m_carCenter.x - (m_w / 2);
    m_carLimits[6].y = m_carCenter.y;
    m_carLimits[7].x = m_carCenter.x + (m_w / 2);
    m_carLimits[7].y = m_carCenter.y;
    m_carLimits[8].x = m_carCenter.x - (m_w / 4);
    m_carLimits[8].y = m_carCenter.y - (m_h / 2);
    m_carLimits[9].x = m_carCenter.x + (m_w / 4);
    m_carLimits[9].y = m_carCenter.y - (m_h / 2);
    m_carLimits[10].x = m_carCenter.x - (m_w / 4);
    m_carLimits[10].y = m_carCenter.y + (m_h / 2);
    m_carLimits[11].x = m_carCenter.x + (m_w / 4);
    m_carLimits[11].y = m_carCenter.y + (m_h / 2);
    float sinusAngle = sinf(m_angle);
    float cosinusAngle = cosf(m_angle);
    for (auto& point : m_carLimits) {
        point = rotateCarCorners(point, m_carCenter, cosinusAngle, sinusAngle);
    }
}

sf::Vector2f Car::rotateCarCorners(const sf::Vector2f& corner, const sf::Vector2f& center,const float cosinusAngle, const float sinusAngle)
{
    sf::Vector2f newCoord;
    newCoord.x = ((corner.x - center.x) * static_cast<float>(cosinusAngle)) - ((corner.y - center.y) * static_cast<float>(sinusAngle)) + center.x;
    newCoord.y = ((corner.x - center.x) * static_cast<float>(sinusAngle)) + ((corner.y - center.y) * static_cast<float>(cosinusAngle)) + center.y;
    return newCoord;
}

float Car::getX() const
{
    return m_x;
}

float Car::getY() const
{
    return m_y;
}
sf::Vector2f Car::getCarLimit( unsigned int cornerNumber) const
{
    return m_carLimits[cornerNumber];
}

sf::Vector2f Car::getCarCenter() const
{
    return m_carCenter;
}

float Car::getAngle() const
{
    return m_angle;
}
/*
void car::move()
{
    if(m_speed + m_acceleration < m_maxSpeed) { m_speed += m_acceleration; }
    m_carCenter.x += (cosf(m_angle * static_cast<float>(M_PI) / 180.0f) * m_speed);
    m_carCenter.y -= -1 * (sinf(m_angle * static_cast<float>(M_PI) / 180.0f) * m_speed);
    updateCarLimits();
}
*/
void Car::move()
{
    if(m_speed + m_acceleration < m_maxSpeed) { m_speed += m_acceleration; }
    m_carCenter.x -= cosf(m_angle) * m_speed;    // radians
    m_carCenter.y += sinf(m_angle) * m_speed;    // radians
    updateCarLimits();
}

void Car::findWayToTarget(const sf::Vector2f& target)
{
    float beta = m_angle - atan2f(target.x - m_carCenter.x, -target.y + m_carCenter.y);
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
    if ((m_carCenter.x - target.x) * (m_carCenter.x - target.x) + (m_carCenter.y - target.y) * (m_carCenter.y - target.y) < 25*25)
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
;
void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
//    sf::Transform states;
	states.transform *= getTransform();
	draw(target, states);
}
