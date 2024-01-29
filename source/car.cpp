#include "header/car.hpp"

Car::Car()
{

}

Car::Car(bool human) : m_human{human}, m_body{1}, m_engine{1}, m_tyres{1}, m_fuel{1}, m_frontMissile{false}, m_rearMissile{false},
  m_highSpeedKit{false}, m_turboCharger{false}, m_spinAssist{false}, m_sideArmour{false},
  m_powerSteering{false}, m_type{Car::Type::Taraco_Neoroder}, m_speed{0}, m_speedLimiter{1}, m_frame{0}
{
    if(human) { m_color = 1; }
    else { m_color = 0; }
}

void Car::updateCarLimits()   // 4 corners + 2 points for front lights  +-2 for car shadow
{
    sf::Vector2f coords{getPosition()};
    m_carLimits[0].x = coords.x - m_center.x + 2;                     // top left
    m_carLimits[0].y = coords.y - m_center.y + 2;
    m_carLimits[1].x = coords.x + m_center.x - 2;                     // top right
    m_carLimits[1].y = coords.y - m_center.y + 2;
    m_carLimits[2].x = coords.x - m_center.x + 2;                     // bottom left
    m_carLimits[2].y = coords.y + m_center.y - 2;
    m_carLimits[3].x = coords.x + m_center.x - 2;                     // bottom right
    m_carLimits[3].y = coords.y + m_center.y - 2;
    m_carLimits[4].x = coords.x - m_center.x + 2;                     // <- left light
    m_carLimits[4].y = coords.y + m_center.y - 8;
    m_carLimits[5].x = coords.x - m_center.x + 2;                     // <- right light
    m_carLimits[5].y = coords.y - m_center.y + 8;

    float sinusAngle = sinf(m_angle);
    float cosinusAngle = cosf(m_angle);
    for (auto& point : m_carLimits) {
// point = rotateCarCorners(point, m_center, cosinusAngle, sinusAngle);
        point.x = ((point.x - m_center.x) * static_cast<float>(cosinusAngle)) - ((point.y - m_center.y) * static_cast<float>(sinusAngle)) + m_center.x;
        point.y = ((point.x - m_center.x) * static_cast<float>(sinusAngle)) + ((point.y - m_center.y) * static_cast<float>(cosinusAngle)) + m_center.y;
    }
}
/*
sf::Vector2f Car::rotateCarCorners(const sf::Vector2f& corner, const sf::Vector2f& center,const float cosinusAngle, const float sinusAngle)
{
    sf::Vector2f newCoord;
    newCoord.x = ((corner.x - center.x) * static_cast<float>(cosinusAngle)) - ((corner.y - center.y) * static_cast<float>(sinusAngle)) + center.x;
    newCoord.y = ((corner.x - center.x) * static_cast<float>(sinusAngle)) + ((corner.y - center.y) * static_cast<float>(cosinusAngle)) + center.y;
    return newCoord;
}
*/
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
    else { m_speed = m_maxSpeed; }
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
    setAngle(m_angle - 0.261f);
    if(m_frame == 0) { m_frame = 23; }
    else { --m_frame; }
    // update des coins
}

void Car::turnRight()
{
   setAngle(m_angle + 0.261f);
   if(m_frame == 23) { m_frame = 0; }
    else { ++m_frame; }
    // update des coins
}

void Car::accelerate()
{
    if(m_speed < 3.f)
        m_speed += 0.05f;
}

void Car::decelerate()
{
    if(m_speed > 0.f)
        m_speed -= 0.05f;
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

float Car::getSpeedLimiter() const
{
    return m_speedLimiter;
}

sf::Vector2f Car::getCenter() const
{
    return m_center;
}

sf::FloatRect Car::getShape() const
{
    return m_shape;
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
    return m_highSpeedKit;
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

unsigned int Car::getColor()
{
    return m_color;
}

void Car::setType(Type type)
{
    m_type = type;
}

void Car::setShape(sf::FloatRect& rect)
{
    m_shape = rect;
    updateCarLimits();
}

void Car::setCenter(float centerX, float centerY)
{
    m_center.x = centerX;
    m_center.y = centerY;
    setOrigin(centerX, centerY);
}

void Car::setCenter(sf::Vector2f& coords)
{
    m_center.x = coords.x;
    m_center.y = coords.y;
    setOrigin(coords.x, coords.y);
}

void  Car::setAngle(float angle)
{
    m_angle = angle;
    if(m_angle > 6.2838) { m_angle -= 6.2838; }
    if(m_angle < 0) { m_angle += 6.2838; }
    setRotation(m_angle  / 0.0174);
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

void Car::setSpeedLimiter(float speedLimiter)
{
    m_speedLimiter = speedLimiter;
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
    m_highSpeedKit = highSpeed;
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

void Car::setColor(unsigned int color)
{
    m_color = color;
    m_frame = 0;
}

void Car::setTexture(sf::Texture* carTexture)
{
    m_carTexture = carTexture;
}

void Car::setVertices()      /////////////////////////// set les vertices par rapport au model et la couleur
{
    m_vertices.setPrimitiveType(sf::Quads);
    sf::FloatRect carTextureCoords;
    switch(m_type) {
        case Type::Taraco_Neoroder :
        case Type::Taraco_Neoroder1 :
            m_shape = sf::FloatRect(0, 0, 49, 35);
            carTextureCoords = sf::FloatRect(0, 0, 49, 35);
            break;
        case Type::Vaug_Interceptor2 :
        case Type::Vaug_Interceptor3 :
            m_shape = sf::FloatRect(0, 0, 53, 39);
            carTextureCoords = sf::FloatRect(0, 35, 53, 39);
            break;
        case Type::Retron_Parsec_Turbo5 :
        case Type::Retron_Parsec_Turbo6 :
        case Type::Retron_Parsec_Turbo8 :
            m_shape = sf::FloatRect(0, 0, 57, 39);
            carTextureCoords = sf::FloatRect(0, 74, 57, 39);
            break;
        default :
            break;
    }
    carTextureCoords.top += m_color * 113;
    float width = carTextureCoords.width;
    float heigth = carTextureCoords.height;
    float LeftX = carTextureCoords.left;
    float topY = carTextureCoords.top;
    float RightX = LeftX + width;
    float bottomY = topY + heigth;
    sf::Vertex vertex;
    m_vertices.resize(0);
    for(auto i = 0; i < 24; ++i) {
        vertex.position = sf::Vector2f(0,0);
        vertex.texCoords = sf::Vector2f(LeftX + (i * width), topY);
        m_vertices.append(vertex);
        vertex.position = sf::Vector2f(width,0);
        vertex.texCoords = sf::Vector2f(RightX + (i * width), topY);
        m_vertices.append(vertex);
        vertex.position = sf::Vector2f(width,heigth);
        vertex.texCoords = sf::Vector2f(RightX + (i * width), bottomY);
        m_vertices.append(vertex);
        vertex.position = sf::Vector2f(0,heigth);
        vertex.texCoords = sf::Vector2f(LeftX + (i * width), bottomY);
        m_vertices.append(vertex);
    }
}

void Car::setStartFrame()
{
    if(m_angle < 1) { m_frame = 0; }
    else {
        if(m_angle >4.7123) { m_frame = 18; }
        else {
            if(m_angle > 3.1415) { m_frame = 12; }
            else {
                 if(m_angle > 1.5707) { m_frame = 6; }
            }
        }
    }
    setVertices();
}

void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = m_carTexture;
    target.draw(&m_vertices[m_frame * 4], 4, sf::Quads, states);
}
