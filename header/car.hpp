#ifndef CAR_HPP
#define CAR_HPP

#include <cmath>
#include <array>
#include <SFML/System/Vector2.hpp>
#include <iostream>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


class Car : public sf::Transformable, public sf::Drawable, public sf::NonCopyable
{
	public:
	    enum                                        Type {Taraco_Neoroder = 0, Taraco_Neoroder1, Vaug_Interceptor2, Vaug_Interceptor3, Retron_Parsec_Turbo5, Retron_Parsec_Turbo6, Retron_Parsec_Turbo8};
Car();
                                                    Car(bool human);
        sf::Vector2f                                getCarLimit(unsigned int cornerNumber) const;
        Type                                        getType() const;
        sf::FloatRect                               getShape() const;
        float                                       getAngle() const;
        float                                       getSpeed() const;
        float                                       getMaxSpeed() const;
        float                                       getAcceleration() const;
        float                                       getSpeedLimiter() const;
        sf::Vector2f                                getCenter() const;
        int                                         getElevation() const;
        float                                       getBodyState() const;
        float                                       getEngineState() const;
        float                                       getTyresState() const;
        float                                       getFuelState() const;
        bool                                        getIsFrontMissileEquiped() const;
        bool                                        getIsRearMissileEquiped() const;
        bool                                        getIsHighSpeedKitEquiped() const;
        bool                                        getIsTurboChargerKitEquiped() const;
        bool                                        getIsRetrorKitEquiped() const;
        bool                                        getIsSpinAssistKitEquiped() const;
        bool                                        getIsSideArmourKitEquiped() const;
        bool                                        getIsPowerSteeringKitEquiped() const;
        unsigned int                                getColor();

        void                                        setType(Type type);
        void                                        setShape(sf::FloatRect& rect);
        void                                        setCenter(float centerX, float centerY);
        void                                        setCenter(sf::Vector2f& coords);
        void                                        setAngle(float angle);
        void                                        setSpeed(float speed);
        void                                        setMaxSpeed(float maxSpeed);
        void                                        setAcceleration(float acceleration);
        void                                        setSpeedLimiter(float speedLimiter);
        void                                        setElevation(int elevation);
        void                                        setBodyState(float body);
        void                                        setEngineState(float engine);
        void                                        setTyresState(float tyres);
        void                                        setFuelState(float fuel);
        void                                        setIsFrontMissileEquiped(bool frontMissile);
        void                                        setIsRearMissileEquiped(bool rearMissile);
        void                                        setIsHighSpeedKitEquiped(bool highSpeed);
        void                                        setIsRetroKitEquiped(bool retro);
        void                                        setIsTurboChargerKitEquiped(bool turboCharger);
        void                                        setIsSpinAssistKitEquiped(bool spinAssist);
        void                                        setIsSideArmourKitEquiped(bool sideArmour);
        void                                        setIsPowerSteeringKitEquiped(bool powerSteering);
        void                                        setColor(unsigned int color);
        void                                        setTexture(sf::Texture* carTexture);
        void                                        setVertices();
        void                                        setStartFrame();

        void                                        move();
        void                                        turnLeft();
        void                                        turnRight();
        void                                        accelerate();
        void                                        decelerate();
        void                                        findWayToTarget(const sf::Vector2f& target);
        bool                                        isInTargetArea(const sf::Vector2f& target);

	private:
	    bool                                        m_human;                            // car driven by human or AI
        float                                       m_body;                             // % body health
        float                                       m_engine;                           // % engine usury
        float                                       m_tyres;                            // % tyres usury
        float                                       m_fuel;                             // % fuel tank level
        sf::Vector2f                                m_center;
        bool                                        m_frontMissile;                     // you can only buy one per race.
        bool                                        m_rearMissile;                      // you can only buy one per race.
        bool                                        m_highSpeedKit;                     // Increases your maximum speed - Invaluable on the straights!
        bool                                        m_turboCharger;                     // Use this to increase your acceleration rate. This option is particularly useful when running into patches of mud which slow your car down.
        bool                                        m_spinAssist;                       // Spin Assist will always point your car in the right direction.
        bool                                        m_sideArmour;                       // Use this to send any other vehicle into an uncontrollable spin simply by ramming it.
        bool                                        m_powerSteering;                    // Essential for decreasing your turning circle and allowing for faster corner-taking.
        bool                                        m_retro;                            // Use this for faster braking.

        Car::Type                                   m_type;
        sf::FloatRect                               m_shape;
        float                                       m_angle;
        float                                       m_speed;
        float                                       m_maxSpeed;
        float                                       m_acceleration;
        float                                       m_speedLimiter;
        std::array<sf::Vector2f, 6>                 m_carLimits;                        // corners for collisions and tracks limit detection

        int                                         m_elevation;                        // altitude to know if car is on or under the bridges
        unsigned int                                m_color;
        sf::Texture*                                m_carTexture;
        sf::VertexArray                             m_vertices;
        unsigned int                                m_frame;

        virtual void			                    draw(sf::RenderTarget& target, sf::RenderStates states) const;
        sf::Vector2f                                rotateCarCorners(const sf::Vector2f& corner, const sf::Vector2f& center, const float cosinusAngle, const float sinusAngle) ;
        void                                        updateCarLimits();
};

#endif
