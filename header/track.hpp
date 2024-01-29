#ifndef TRACK_HPP
#define TRACK_HPP

#include <jsonfile/jsonfile.hpp>
#include <SFML/Graphics/Image.hpp>
#include <string>
#include <vector>
#include <array>
#include <iostream>

class Track {
    public :
                                                                        enum class Orientation{ vertical = 0, horizontal };
                                                                        Track();
                                                                        Track(int trackNB);
        void                                                            loadTrackData(int trackNB);
        sf::Vector2f                                                    getCarSpawnCoords(unsigned int ranking, bool clock);
        double                                                          getCarSpawnangle(unsigned int ranking, bool clock);

    private :
        mylib::JsonFile                                                 m_trackData;
        sf::Vector2f                                                    resolution;
        sf::Vector2f                                                    arrivalDrawPortal;
        Orientation                                                     arrivalDrawPortalOrientation;
        sf::Vector2f                                                    arrival;
        int                                                             bridgeNB;
        struct BridgeDrawsetting {
            float x;
            float y;
            Orientation orientation;
            int floor;
        };
        std::vector<BridgeDrawsetting>                                  bridgeInfo;
        std::vector<sf::Vector2f>                                       hazzardPossibleLocations;
        struct CarSpawnLocation {
            float x;
            float y;
            float angle;
        };
        std::vector<CarSpawnLocation>                                   carsSpawnLocationsClock;
        std::vector<CarSpawnLocation>                                   carsSpawnLocationsCounterClock;
        int                                                             antiCheatWaypointNb;
        int                                                             antiCheatWaypointRadius;
        std::vector<sf::Vector2f>                                       antiCheatWaypoints;
        int                                                             upHillNb;
        std::vector<sf::Vector2f>                                       upHillscoords;
        int                                                             downHillNb;
        std::vector<sf::Vector2f>                                       downHillscoords;
        int                                                             nbRankingCoords;
        int                                                             RankingCoordRadius;
        std::vector<sf::Vector2f>                                       RankingCoords;
        int                                                             autoPilotNbWaypoints;
        int                                                             autoPilotWaypointRadius;
        std::array<std::vector<sf::Vector2f>, 3>                        waypointsLines;
        std::array<int, 7>                                              firstWaypointForComputerCars;
};

#endif // TRACK_HPP
