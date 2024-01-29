#ifndef    RACE_HPP
#define    RACE_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>
#include <array>

#include <jsonfile/jsonfile.hpp>
#include "header/assetkey.hpp"
#include "header/track.hpp"
#include "header/players.hpp"
#include "header/racebottompanel.hpp"
#include "header/player.hpp"
#include "header/command.hpp"
#include "triple/triple.hpp"
#include "audioPlayer/audioPlayer.hpp"
#include "collisioner/collisioner.hpp"

class Race {
    public:
                enum                                                    HazardType{big_water = 0, small_water, big_sand, small_sand, big_oil, small_oil};

                                                                        Race(sf::RenderWindow& window, const unsigned int trackNb, Players& players, const unsigned int language, const bool clockwiseRaceRotation);

                unsigned int                                            racing(const unsigned int completedRaces, const bool clockwiseRaceRotation);

    private:
                sf::RenderWindow&                                       m_window;
                sf::View                                                m_topView;
                sf::View                                                m_bottomView;
                sf::Sprite                                              m_track;
                sf::Sprite                                              m_arrival;
                sf::Sprite                                              m_bridge;
                sf::Sprite                                              m_hazard;
                sf::Sprite                                              m_shadows;
                Track                                                   m_trackData;
                mylib::AssetContainer<textures::ID, sf::Texture>        m_textureContaigner;
                const unsigned int                                      m_trackNb;
                Players&                                                m_players;
                const unsigned int                                      m_language;
                RaceBottomPanel                                         m_raceBottomPanel;
                unsigned int                                            m_currentLap;
                unsigned int                                            m_raceLapsNb;
                unsigned int                                            m_arrivedRacersNumber;
                struct Hazard {
                    HazardType     type;
                    sf::FloatRect   bounds;
                };
                std::vector<Hazard>                                     m_hazards;
                std::vector<mylib::Triple<bool, bool, bool>>            m_anticheatWaypointsPlayersStatus;
                std::array<sf::Image, 2>                                m_TrackLimits;
                mylib::AudioPlayer                                      m_audioPlayer;
                mylib::Collisioner                                      m_collisioner;

                void                                                    loadTextures(const unsigned int trackNb, const bool clockwiseRaceRotation);
                void                                                    loadSounds();
                void                                                    loadMusic(const unsigned int completedRaces);
                void                                                    initViews();
                void                                                    initSprites();

                void                                                    updateRaceBottomPanel(const Player& player);
                void                                                    hazardsgeneration(const unsigned int completedRaces);

                void                                                    setRaceLapsNumber(unsigned int completedRaces);
                void                                                    setRacersOnStartingPosition();
                void                                                    setTopViewCenter();
                void                                                    setRacersOnStartingPosition(const bool clockwiseRaceRotation);

                void                                                    drawHazards();
                void                                                    drawBridge();

                void                                                    checkCarsInHazards();
                void                                                    checkPlayerCarNearBridge(const std::vector<sf::FloatRect>&  areas);
                void                                                    checkCarInHazards();
                void                                                    checkCarsInAnicheatArea(bool clockwiseRaceRotation);
                bool                                                    checkCarsOnArrival();
                void                                                    checkCarsOnTrack();
                void                                                    checkCarsRanking(const bool clockwiseRaceRotation);
                void                                                    checkCarOnCarCollision();

                void                                                    moveCars();
                void                                                    driveHumanCar(const Command& command);
                void                                                    driveComputerCars();
};
#endif
