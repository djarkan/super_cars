#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/NonCopyable.hpp>
#include "header/car.hpp"

class Player
{
    public :
        enum                                        LevelDifficulty {normal = 0, medium, hard};
Player();
                                                    Player(const bool human);
                                                    Player(const bool human, const std::string name);

        unsigned int                                getJoystickID();
        unsigned int                                getMoney();
        unsigned int                                getGameDifficulty();
        std::string                                 getName();
        Car::Type                                   getCarType() const;
        sf::FloatRect                               getCarShape() const;
        float                                       getCarAngle() const;
        float                                       getCarSpeed() const;
        float                                       getCarMaxSpeed() const;
        float                                       getCarAcceleration() const;
        float                                       getCarSpeedLimiter() const;
        sf::Vector2f                                getCarPosition();
        sf::Vector2f                                getCarCenter();
        int                                         getCarElevation() const;
        float                                       getCarBodyState() const;
        float                                       getCarEngineState() const;
        float                                       getCarTyresState() const;
        float                                       getCarFuelState() const;
        bool                                        getCarIsFrontMissileEquiped() const;
        bool                                        getCarIsRearMissileEquiped() const;
        bool                                        getCarIsHighSpeedKitEquiped() const;
        bool                                        getCarIsTurboChargerKitEquiped() const;
        bool                                        getCarIsRetroKitEquiped() const;
        bool                                        getCarIsSpinAssistKitEquiped() const;
        bool                                        getCarIsSideArmourKitEquiped() const;
        bool                                        getCarIsPowerSteeringKitEquiped() const;
        unsigned int                                getCarColor();
        const Car&                                  getCar() const;

        void                                        setHuman(bool human);
        void                                        setJoystickID();
        void                                        setGameDifficulty();
        void                                        levelupGameDifficulty();
        void                                        setName(const std::string& name);
        void                                        setMoney(unsigned int money);
        void                                        setCarType(Car::Type type);
        void                                        setCarShape(sf::FloatRect rect);
        void                                        setCarAngle(float angle);
        void                                        setCarSpeed(float speed);
        void                                        setCarMaxSpeed(float maxSpeed);
        void                                        setCarAcceleration(float acceleration);
        void                                        setCarSpeedLimiter(float speedLimiter);
        void                                        setCarCenter(float centerX, float centerY);
        void                                        setCarCenter(sf::Vector2f& coords);
        void                                        setCarElevation(int elevation);
        void                                        setCarBodyState(float body);
        void                                        setCarEngineState(float engine);
        void                                        setCarTyresState(float tyres);
        void                                        setCarFuelState(float fuel);
        void                                        setCarFrontMissile(bool frontMissile);
        void                                        setCarRearMissile(bool rearMissile);
        void                                        setCarHighSpeedKit(bool highSpeed);
        void                                        setCarRetroKit(bool retro);
        void                                        setCarTurboChargerKit(bool turboCharger);
        void                                        setCarSpinAssistKit(bool spinAssist);
        void                                        setCarSideArmourKit(bool sideArmour);
        void                                        setCarPowerSteeringKit(bool powerSteering);
        void                                        setCarColor(unsigned int color);
        void                                        setTexture(sf::Texture* carTexture);
        void                                        setCarPosition(const sf::Vector2f& position);
        void                                        setCarStartFrame();

        void                                        moveCar();
        void                                        turnCarLeft();
        void                                        turnCarRight();
        void                                        accelerate();
        void                                        decelerate();

    private :
        bool                                        m_human;
        unsigned int                                m_joystickID;
        LevelDifficulty                             m_gameDifficulty;               //   repercuter sur les autres en muti
        std::string                                 m_name;
        unsigned int                                m_money;
        Car                                         m_car;
};
#endif // PLAYER_HPP
