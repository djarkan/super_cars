#ifndef CAR_HPP
#define CAR_HPP

#include <cmath>
#include <array>
#include <SFML/System/Vector2.hpp>
#include <iostream>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


class Car : public sf::Transformable, public sf::Drawable, sf::NonCopyable
{
	public:
	    enum class type {Taraco_Neoroder, Taraco_Neoroder1, Vaug_Interceptor2, Vaug_Interceptor3, Retron_Parsec_Turbo5, Retron_Parsec_Turbo6, Retron_Parsec_Turbo8};

	    Car();
	    Car(type type);
        Car(float x, float y, float w, float h, float angle,  float maxSpeed, bool humanPlayer);

        float                                       getX() const;
        float                                       getY() const;
        float                                       getAngle() const;
        sf::Vector2f                                getCarLimit(unsigned int cornerNumber) const;
        sf::Vector2f                                getCarCenter() const;
        void                                        move();
        void                                        findWayToTarget(const sf::Vector2f& target);
        bool                                        isInTargetArea(const sf::Vector2f& target);
        void                                        turnLeft();
        void                                        turnRight();
        void                                        speedUp();
        void                                        speedDown();
        void                                        setTheCar();
        virtual void			                    draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
	    type m_type;
        float m_x, m_y, m_w, m_h;
        float m_angle;
        float m_speed, m_maxSpeed, m_acceleration;
        bool m_humanPlayer;
        std::array<sf::Vector2f, 12> m_carLimits;                                                               // corners for collisions and tracks limit detection
        sf::Vector2f m_carCenter;
        int elevation;                                                                                          // altitude to know if car is on or under the bridges
        int m_body;
        int m_engine;
        float m_tyres;
        int m_fuel;
        bool m_frontMissile{false};                                                                             // you can only buy one per race.
        bool m_rearMissile{false};                                                                              // you can only buy one per race.
        bool m_highSpeedKit{false};                                                                             // Increases your maximum speed - Invaluable on the straights!
        bool m_turboCharger{false};                                                                             // Use this to increase your acceleration rate. This option is particularly useful when running into patches of mud which slow your car down.
        bool m_spinAssist{false};                                                                               // Use this for faster braking.
        bool m_sideArmour{false};                                                                               // Use this to send any other vehicle into an uncontrollable spin simply by ramming it.
        bool m_powerSteering{false};                                                                            // Essential for decreasing your turning circle and allowing for faster corner-taking.


        sf::Vector2f rotateCarCorners(const sf::Vector2f& corner, const sf::Vector2f& center, const float cosinusAngle, const float sinusAngle) ;
        void updateCarLimits();
};

#endif
