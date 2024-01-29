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
        m_trackData.loadJsonFile("data/tracks/track" + std::to_string(trackNB) + ".json");
        resolution.x = m_trackData.m_Root["track"]["resolution"].get("width",0).asFloat();
        resolution.y = m_trackData.m_Root["track"]["resolution"].get("higth",0).asFloat();
        arrivalDrawPortal.x = m_trackData.m_Root["track"]["arrivalDrawPortal"].get("x",0).asFloat();
        arrivalDrawPortal.y = m_trackData.m_Root["track"]["arrivalDrawPortal"].get("y",0).asFloat();
        arrivalDrawPortalOrientation = static_cast<Orientation>(m_trackData.m_Root["track"]["arrivalDrawPortal"].get("orientation",0).asInt());
        arrival.x = m_trackData.m_Root["track"]["arrival"].get("x",0).asFloat();
        arrival.y = m_trackData.m_Root["track"]["arrival"].get("y",0).asFloat();
        bridgeNB = m_trackData.m_Root["track"]["bridges"].get("bridgeNb",0).asInt();
        BridgeDrawsetting bridgeDrawsetting;
        for(auto i = 0; i < bridgeNB; ++i) {
            bridgeDrawsetting.x = m_trackData.m_Root["track"]["bridges"]["bridgeDrawsetting"][i].get("x",0).asFloat();
            bridgeDrawsetting.y = m_trackData.m_Root["track"]["bridges"]["bridgeDrawsetting"][i].get("y",0).asFloat();
            bridgeDrawsetting.orientation = static_cast<Orientation>(m_trackData.m_Root["track"]["bridges"]["bridgeDrawsetting"][i].get("orientation",0).asInt());
            bridgeDrawsetting.floor = m_trackData.m_Root["track"]["bridges"]["bridgeDrawsetting"][i].get("elevation",0).asInt();
            bridgeInfo.push_back(bridgeDrawsetting);
        }
        sf::Vector2f coords;
        for(auto i = 0; i < 16; ++i) {
            coords.x = m_trackData.m_Root["track"]["hazzardPossibleLocations"][i].get("x",0).asFloat();
            coords.y = m_trackData.m_Root["track"]["hazzardPossibleLocations"][i].get("y",0).asFloat();
            hazzardPossibleLocations.push_back(coords);
        }
        antiCheatWaypointNb = m_trackData.m_Root["track"]["antiCheat"].get("antiCheatWaypointNb",0).asInt();
        antiCheatWaypointRadius = m_trackData.m_Root["track"]["antiCheat"].get("RankingCoordRadius",0).asInt();
        for(auto i = 0; i < antiCheatWaypointNb; ++i) {
            coords.x = m_trackData.m_Root["track"]["antiCheat"]["antiCheatWaypoints"][i].get("x",0).asFloat();
            coords.y = m_trackData.m_Root["track"]["antiCheat"]["antiCheatWaypoints"][i].get("y",0).asFloat();
            antiCheatWaypoints.push_back(coords);
        }
        upHillNb = m_trackData.m_Root["track"]["elevation"].get("upHillNb",0).asInt();
        for(auto i = 0; i < upHillNb; ++i) {
            coords.x = m_trackData.m_Root["track"]["elevation"]["upHills"][i].get("x",0).asFloat();
            coords.y = m_trackData.m_Root["track"]["elevation"]["upHills"][i].get("y",0).asFloat();
            upHillscoords.push_back(coords);
        }
        downHillNb = m_trackData.m_Root["track"]["elevation"].get("downHillNb",0).asInt();
        for(auto i = 0; i < downHillNb; ++i) {
            coords.x = m_trackData.m_Root["track"]["elevation"]["downHills"][i].get("x",0).asFloat();
            coords.y = m_trackData.m_Root["track"]["elevation"]["downHills"][i].get("y",0).asFloat();
            downHillscoords.push_back(coords);
        }
        nbRankingCoords = m_trackData.m_Root["track"]["racingRankingCoords"].get("nbRankingCoords",0).asInt();
        RankingCoordRadius = m_trackData.m_Root["track"]["racingRankingCoords"].get("RankingCoordRadius",0).asInt();
        for(auto i = 0; i < nbRankingCoords; ++i) {
            coords.x = m_trackData.m_Root["track"]["racingRankingCoords"]["RankingCoords"][i].get("x",0).asFloat();
            coords.y = m_trackData.m_Root["track"]["racingRankingCoords"]["RankingCoords"][i].get("y",0).asFloat();
            RankingCoords.push_back(coords);
        }
        autoPilotNbWaypoints = m_trackData.m_Root["track"]["autoPilotWaypoints"].get("nbWaypointsPerLine",0).asInt();
        autoPilotWaypointRadius = m_trackData.m_Root["track"]["autoPilotWaypoints"].get("autoPilotWaypointRadius",0).asInt();

        for(auto j = 0; j < 3; j++) {
            for(auto i = 0; i < autoPilotNbWaypoints; ++i) {
                coords.x = m_trackData.m_Root["track"]["autoPilotWaypoints"]["waypointsLines"][i].get("x",0).asFloat();
                coords.y = m_trackData.m_Root["track"]["autoPilotWaypoints"]["waypointsLines"][i].get("y",0).asFloat();
                waypointsLines[j].push_back(coords);
            }
        }
        for(auto i = 0; i < 7; ++i) {
            firstWaypointForComputerCars[i] = m_trackData.m_Root["track"]["autoPilotWaypoints"]["firstWaypointForComputerCars"][i].asInt();
        }
}

sf::Vector2f Track::getCarSpawnCoords(unsigned int ranking, bool clock)
{
    if(clock) { return sf::Vector2f(m_trackData.m_Root["track"]["carsSpawnLocationsClock"][ranking]["x"].asFloat(), m_trackData.m_Root["track"]["carsSpawnLocationsClock"][ranking]["y"].asFloat()); }
    else { return sf::Vector2f(m_trackData.m_Root["track"]["carsSpawnLocationsCounterClock"][ranking]["x"].asFloat(), m_trackData.m_Root["track"]["carsSpawnLocationsCounterClock"][ranking]["y"].asFloat()); }
}

double Track::getCarSpawnangle(unsigned int ranking, bool clock)
{
    if(clock) { return m_trackData.m_Root["track"]["carsSpawnLocationsClock"][ranking]["angle"].asFloat(); }
    else { return m_trackData.m_Root["track"]["carsSpawnLocationsCounterClock"][ranking]["angle"].asFloat(); }
}
