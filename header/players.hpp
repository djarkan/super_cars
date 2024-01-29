#ifndef PLAYERS_HPP
#define PLAYERS_HPP

#include <vector>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "header/player.hpp"
#include "jsonfile/jsonfile.hpp"

class Players
{
    public :
                                                Players(sf::RenderWindow& window);
        Player&                                 getHumanPlayer();
        unsigned int                            getHumanJoystick();
        unsigned int                            getGameDifficulty();
        unsigned int                            getRacerNumber();
        sf::Vector2f                            getHumanCarPosition();
        const Car&                              getPlayerCar(unsigned int index);


        void                                    setHumanName(bool won);
        void                                    setCarPosition(sf::Vector2f& coords, unsigned int index);
        void                                    setCarAngle(double angle, unsigned int index);
        void                                    setCarStartFrame(unsigned int index);

        std::string                             enterPseudo();
        void                                    buildPlayers(unsigned int completedRaces, unsigned int lastRaceRanking);
        void                                    levelupGameDifficulty();

        void                                    moveCars();

    private :
        sf::RenderWindow&                       m_window;
        Player                                  m_humanPlayer;
        std::vector<std::unique_ptr<Player>>                    m_players;
        mylib::JsonFile                         m_languageJson;
        unsigned int                            m_language;
        sf::Texture                             m_carsTexture;


};
#endif


/*
        float                                   getCarAngle(Player& player) const;
        float                                   getCarSpeed(Player& player) const;
        float                                   getCarMaxSpeed(Player& player) const;
        float                                   getCarAcceleration(Player& player) const;
        sf::Vector2f&                           getCarCenter(Player& player);
        int                                     getCarElevation(Player& player) const;
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
