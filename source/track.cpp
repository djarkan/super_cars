#include "header/track.hpp"

Track::Track()
{

}

Track::Track(int trackNB)
{
    loadTrackData(trackNB);
}

void Track::loadTrackData(int trackNB)
{
        mylib::JsonFile   trackData;
        trackData.loadJsonFile("data/tracks/track" + std::to_string(trackNB) + ".json");
        resolution.x = trackData.m_Root["track"]["resolution"].get("width",0).asFloat();
        resolution.y = trackData.m_Root["track"]["resolution"].get("higth",0).asFloat();
        arrivalDrawPortal.x = trackData.m_Root["track"]["arrivalDrawPortal"].get("x",0).asFloat();
        arrivalDrawPortal.y = trackData.m_Root["track"]["arrivalDrawPortal"].get("y",0).asFloat();
        arrivalDrawPortalOrientation = static_cast<Orientation>(trackData.m_Root["track"]["arrivalDrawPortal"].get("orientation",0).asInt());
        arrival.x = trackData.m_Root["track"]["arrival"].get("x",0).asFloat();
        arrival.y = trackData.m_Root["track"]["arrival"].get("y",0).asFloat();
        bridgeNB = trackData.m_Root["track"]["bridges"].get("bridgeNb",0).asInt();
        BridgeDrawsetting bridgeDrawsetting;
        for(auto i = 0; i < bridgeNB; ++i) {
            bridgeDrawsetting.x = trackData.m_Root["track"]["bridges"]["bridgeDrawsetting"][i].get("x",0).asFloat();
            bridgeDrawsetting.y = trackData.m_Root["track"]["bridges"]["bridgeDrawsetting"][i].get("y",0).asFloat();
            bridgeDrawsetting.orientation = static_cast<Orientation>(trackData.m_Root["track"]["bridges"]["bridgeDrawsetting"][i].get("orientation",0).asInt());
            bridgeDrawsetting.floor = trackData.m_Root["track"]["bridges"]["bridgeDrawsetting"][i].get("elevation",0).asInt();
            bridgeInfo.push_back(bridgeDrawsetting);
        }
        sf::Vector2f coords;
        for(auto i = 0; i < 16; ++i) {
            coords.x = trackData.m_Root["track"]["hazzardPossibleLocations"][i].get("x",0).asFloat();
            coords.y = trackData.m_Root["track"]["hazzardPossibleLocations"][i].get("y",0).asFloat();
            hazzardPossibleLocations.push_back(coords);
        }
        CarSpawnLocation carSpawnLocation;
        for(auto i = 0; i < 8; ++i) {
            carSpawnLocation.x = trackData.m_Root["track"]["carsSpawnLocationsClock"][i].get("x",0).asFloat();
            carSpawnLocation.y = trackData.m_Root["track"]["carsSpawnLocationsClock"][i].get("y",0).asFloat();
            carSpawnLocation.angle = trackData.m_Root["track"]["carsSpawnLocationsClock"][i].get("angle",0).asFloat();
            carsSpawnLocationsClock.push_back(carSpawnLocation);
        }
        for(auto i = 0; i < 8; ++i) {
            carSpawnLocation.x = trackData.m_Root["track"]["carsSpawnLocationsCounterClock"][i].get("x",0).asFloat();
            carSpawnLocation.y = trackData.m_Root["track"]["carsSpawnLocationsCounterClock"][i].get("y",0).asFloat();
            carSpawnLocation.angle = trackData.m_Root["track"]["carsSpawnLocationsCounterClock"][i].get("angle",0).asFloat();
            carsSpawnLocationsCounterClock.push_back(carSpawnLocation);
        }
        antiCheatWaypointNb = trackData.m_Root["track"]["antiCheat"].get("antiCheatWaypointNb",0).asInt();
        antiCheatWaypointRadius = trackData.m_Root["track"]["antiCheat"].get("RankingCoordRadius",0).asInt();
        for(auto i = 0; i < antiCheatWaypointNb; ++i) {
            coords.x = trackData.m_Root["track"]["antiCheat"]["antiCheatWaypoints"][i].get("x",0).asFloat();
            coords.y = trackData.m_Root["track"]["antiCheat"]["antiCheatWaypoints"][i].get("y",0).asFloat();
            antiCheatWaypoints.push_back(coords);
        }
        upHillNb = trackData.m_Root["track"]["elevation"].get("upHillNb",0).asInt();
        for(auto i = 0; i < upHillNb; ++i) {
            coords.x = trackData.m_Root["track"]["elevation"]["upHills"][i].get("x",0).asFloat();
            coords.y = trackData.m_Root["track"]["elevation"]["upHills"][i].get("y",0).asFloat();
            upHillscoords.push_back(coords);
        }
        downHillNb = trackData.m_Root["track"]["elevation"].get("downHillNb",0).asInt();
        for(auto i = 0; i < downHillNb; ++i) {
            coords.x = trackData.m_Root["track"]["elevation"]["downHills"][i].get("x",0).asFloat();
            coords.y = trackData.m_Root["track"]["elevation"]["downHills"][i].get("y",0).asFloat();
            downHillscoords.push_back(coords);
        }
        nbRankingCoords = trackData.m_Root["track"]["racingRankingCoords"].get("nbRankingCoords",0).asInt();
        RankingCoordRadius = trackData.m_Root["track"]["racingRankingCoords"].get("RankingCoordRadius",0).asInt();
        for(auto i = 0; i < nbRankingCoords; ++i) {
            coords.x = trackData.m_Root["track"]["racingRankingCoords"]["RankingCoords"][i].get("x",0).asFloat();
            coords.y = trackData.m_Root["track"]["racingRankingCoords"]["RankingCoords"][i].get("y",0).asFloat();
            RankingCoords.push_back(coords);
        }
        autoPilotNbWaypoints = trackData.m_Root["track"]["autoPilotWaypoints"].get("nbWaypointsPerLine",0).asInt();
        autoPilotWaypointRadius = trackData.m_Root["track"]["autoPilotWaypoints"].get("autoPilotWaypointRadius",0).asInt();

        for(auto j = 0; j < 3; j++) {
            for(auto i = 0; i < autoPilotNbWaypoints; ++i) {
                coords.x = trackData.m_Root["track"]["autoPilotWaypoints"]["waypointsLines"][i].get("x",0).asFloat();
                coords.y = trackData.m_Root["track"]["autoPilotWaypoints"]["waypointsLines"][i].get("y",0).asFloat();
                waypointsLines[j].push_back(coords);
            }
        }
        for(auto i = 0; i < 7; ++i) {
            firstWaypointForComputerCars[i] = trackData.m_Root["track"]["autoPilotWaypoints"]["firstWaypointForComputerCars"][i].asInt();
        }
}
