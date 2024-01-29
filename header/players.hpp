#ifndef PLAYERS_HPP
#define PLAYERS_HPP

#include <vector>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "jsonfile/jsonfile.hpp"
#include "timer/timer.hpp"
#include "header/inputs.hpp"


class Players
{
    public :
                                                Players(sf::RenderWindow& window);
                                                ~Players();

        Player&                                 getHumanPlayer();
        unsigned int                            getHumanJoystick();
        sf::Vector2f                            getHumanCarPosition();
        unsigned int                            getHumanRanking() const;
        const std::string&                      getHumanName() const;
        unsigned int                            getGameDifficulty();
        unsigned int                            getRacerNumber() const;
        sf::Vector2f                            getCarPosition(const unsigned int playerIndex) const;
        Player*                                 getPlayer(const unsigned int index);
        const Car&                              getPlayerCar(unsigned int index);
        unsigned int                            getPlayerCarElevation(unsigned int index) const;

        bool                                    setHumanName(bool won);

        std::string                             enterPseudo();
        void                                    buildPlayers(unsigned int completedRaces, unsigned int lastRaceRanking);
        void                                    levelupGameDifficulty();

    private :
        sf::RenderWindow&                       m_window;
        Player                                  m_humanPlayer;
        std::vector<Player*>                    m_players;
        mylib::JsonFile                         m_languageJson;
        unsigned int                            m_language;
        sf::Texture                             m_carsTexture;
};
#endif


/*
        void                                    setPlayerCarPosition(sf::Vector2f& coords, unsigned int index);
        void                                    setPlayerCarAngle(double angle, unsigned int index);
        void                                    setPlayerCarStartFrame(unsigned int index);
        void                                    setPlayerCarElevation(unsigned int elevation, unsigned int index);

        float                                   getCarAngle(Player& player) const;
        float                                   getCarSpeed(Player& player) const;
        float                                   getCarMaxSpeed(Player& player) const;
        float                                   getCarAcceleration(Player& player) const;
        sf::Vector2f&                           getCarCenter(Player& player);
        int                                     getPlayerCarElevation() const;
        float                                   getCarBodyState(Player& player) const;
        float                                   getCarEngineState(Player& player) const;
        float                                   getCarTyresState(Player& player) const;
        float                                   getCarFuelState(Player& player) const;
        bool                                    getCarIsFrontMissileEquiped(Player& player) const;
        bool                                    getCarIsRearMissileEquiped(Player& player) const;
        bool                                    getCarIsHighSpeedKitEquiped(Player& player) const;
        bool                                    getCarIsTurboChargerKitEquiped(Player& player) const;
        bool                                    getCarIsRetroKitEquiped(Player& player) const;
        bool                                    getCarIsSpinAssistKitEquiped(Player& player) const;
        bool                                    getCarIsSideArmourKitEquiped(Player& player) const;
        bool                                    getCarIsPowerSteeringKitEquiped(Player& player) const;


        void                                    setCarSpeed(float speed, Player& player);
        void                                    setCarMaxSpeed(float maxSpeed, Player& player);
        void                                    setCarAcceleration(float acceleration, Player& player);
        void                                    setCarCenter(sf::Vector2f& coords, Player& player);
        void                                    setCarElevation(int elevation, Player& player);
        void                                    setCarBodyState(float body, Player& player);
        void                                    setCarEngineState(float engine, Player& player);
        void                                    setCarTyresState(float tyres, Player& player);
        void                                    setCarFuelState(float fuel, Player& player);
        void                                    setCarFrontMissile(bool frontMissile, Player& player);
        void                                    setCarRearMissile(bool rearMissile, Player& player);
        void                                    setCarHighSpeedKit(bool highSpeed, Player& player);
        void                                    setCarRetroKit(bool retro, Player& player);
        void                                    setCarTurboChargerKit(bool turboCharger, Player& player);
        void                                    setCarSpinAssistKit(bool spinAssist, Player& player);
        void                                    setCarSideArmourKit(bool sideArmour, Player& player);
        void                                    setCarPowerSteeringKit(bool powerSteering, Player& player);
*/
