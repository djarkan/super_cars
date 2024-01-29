#include "header/race.hpp"

#include <string>
#include <array>
#include <algorithm>
#include <cmath>

#include <SFML/Graphics/Sprite.hpp>

#include "timer/timer.hpp"
#include "header/carTextureColorModifier.hpp"
#include "header/inputs.hpp"
#include "header/command.hpp"
#include "random/random.hpp"
#include "bitmap/simpleAnimation/animation.hpp"
#include "assetcontainer/assetcontainer.hpp"

extern std::array<std::pair<std::string, sf::Time>, 9> bestLapsTimes;
extern std::array<mylib::Triple<std::string, int, int>, 7>     bestWinnersList;

Race::Race(sf::RenderWindow& window, const unsigned int trackNb, Players& players, const unsigned int language, const bool clockwiseRaceRotation) :
                                                                                    m_window(window), m_trackData(trackNb + 1), m_textureContaigner(), m_trackNb{trackNb},
                                                                                    m_players{players}, m_language{language}, m_raceBottomPanel(language),
                                                                                    m_currentLap{1}, m_raceLapsNb{5}, m_arrivedRacersNumber{0}
{
    loadTextures(trackNb, clockwiseRaceRotation);
    loadSounds();
std::cout << "race files loaded " << std::endl;
}

void Race::loadTextures(const unsigned int trackNb, const bool clockwiseRaceRotation)
{
    std::string rotation{""};
    if(clockwiseRaceRotation) { rotation = "clock"; }
    else { rotation = "counterclock"; }
    m_textureContaigner.loadAsset(textures::ID::track, "graphics/tracks/races/track" + std::to_string(trackNb + 1) + ".png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
    m_textureContaigner.loadAsset(textures::ID::shadows, "graphics/tracks/shadows/track" + std::to_string(trackNb + 1) + " shadows.png");
    m_TrackLimits[0].loadFromFile("graphics/tracks/roads/track" + std::to_string(trackNb + 1) + " down " + rotation + ".png");
    m_TrackLimits[1].loadFromFile("graphics/tracks/roads/track" + std::to_string(trackNb + 1) + " up " + rotation + ".png");
}

void Race::loadSounds()
{
     m_audioPlayer.loadSound(sounds::ID::applause, "sound/applause.ogg");
     m_audioPlayer.loadSound(sounds::ID::bouncingWheel, "sound/bouncing wheel.ogg");
     m_audioPlayer.loadSound(sounds::ID::bump, "sound/bump.ogg");
     m_audioPlayer.loadSound(sounds::ID::gameOver, "sound/game over.ogg");
     m_audioPlayer.loadSound(sounds::ID::hornCountdown, "sound/horn countdown.ogg");
     m_audioPlayer.loadSound(sounds::ID::tyresLong, "sound/tyres long.ogg");
     m_audioPlayer.loadSound(sounds::ID::tyresShort, "sound/tyres short.ogg");
     m_audioPlayer.loadSound(sounds::ID::tyresWater, "sound/tyres water.ogg");
}

void Race::loadMusic(const unsigned int completedRaces)
{                                                                  //////////////////   music suivant circuit ou ??????????????
    unsigned int race{0};
    if(completedRaces >=18) { race = completedRaces -18; }
    else
        if(completedRaces >=9) { race = completedRaces -9; }
    switch(race) {
        case 1:
        case 5:
        case 9:
            m_audioPlayer.loadMusic("music/track1.wav");
            break;
        case 2:
        case 6:
            m_audioPlayer.loadMusic("music/track2.wav");
            break;
        case 3:
        case 7:
            m_audioPlayer.loadMusic("music/track3.wav");
            break;
        case 4:
        case 8:
            m_audioPlayer.loadMusic("music/track4.wav");
            break;
        default :
            break;
    }
}
void Race::initViews()
{
    m_topView.setSize(sf::Vector2f(512.f, 350.f));
    m_topView.setViewport(sf::FloatRect(0.1f, 0.f, 0.8f, 0.885f));
    m_bottomView.setSize(sf::Vector2f(512.f, 40.f));
    m_bottomView.setCenter(256, 20);
    m_bottomView.setViewport(sf::FloatRect(0.1f, 0.9f, 0.8f, 0.1f));
std::cout << "init views ok " << std::endl;
}

void Race::initSprites()
{
    m_track.setTexture(m_textureContaigner.getAsset(textures::ID::track));
    m_track.setTextureRect(sf::IntRect(0, 0, 1280, 800));
    m_track.setPosition(0, 0);
    m_shadows.setTexture(m_textureContaigner.getAsset(textures::ID::shadows));
    m_shadows.setTextureRect(sf::IntRect(0, 0, 1280, 800));
    m_shadows.setPosition(0, 0);
    m_bridge.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
    m_hazard.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
    m_arrival.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
    if(m_trackData.getArrivalPortalOrientation() == 0) { m_arrival.setTextureRect(sf::IntRect(40, 1570, 160, 22)); }
    else { m_arrival.setTextureRect(sf::IntRect(0, 1570, 22, 161));}
    m_arrival.setPosition(m_trackData.getArrivalPortalCoords());
std::cout << "init sprites ok " << std::endl;
}

void Race::setTopViewCenter()
{
    sf::Vector2f humanCarCoords{m_players.getHumanCarPosition()};
    if(humanCarCoords.x < 256) { humanCarCoords.x = 256; }
    else
        if(humanCarCoords.x > 1024) { humanCarCoords.x = 1024; }
    if(humanCarCoords.y < 175) { humanCarCoords.y = 175; }
    else
        if(humanCarCoords.y > 625) { humanCarCoords.y = 625; }
    m_topView.setCenter(humanCarCoords);
}

void Race::setRacersOnStartingPosition(bool clockwiseRaceRotation)
{
    for(unsigned int i = 8 - m_players.getRacerNumber(), j = 0; j < m_players.getRacerNumber(); ++i, ++j) {
        Player* player{m_players.getPlayer(j)};
        sf::Vector2f spawnCoords = m_trackData.getCarSpawnCoords(i, clockwiseRaceRotation);
        player->setCarPosition(spawnCoords);
        player->setCarAngle(m_trackData.getCarSpawnangle(i, clockwiseRaceRotation));
        player->setCarFrame();
        player->setCarElevation(m_trackData.getCarSpawnElevation(i, clockwiseRaceRotation));
        player->updateCarLimits();
    }
}

unsigned int Race::racing(const unsigned int completedRaces, const bool clockwiseRaceRotation)
{
//std::cout << "entering racing" << std::endl;
    mylib::Animation lightCountdown;
    lightCountdown.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
    for(auto i = 0; i < 5; ++i) {
        lightCountdown.addFrame(sf::IntRect(460, 60 + (i * 50), 160, 48));
    }
    lightCountdown.setFrameRate(1000);
    lightCountdown.setPosition(m_trackData.getCountdownCoords(clockwiseRaceRotation));
    loadMusic(completedRaces);
    initViews();
    initSprites();
//std::cout << "init ok" << std::endl;
    setRaceLapsNumber(completedRaces);
    setRacersOnStartingPosition(clockwiseRaceRotation);
//std::cout << "players on tracks" << std::endl;
    hazardsgeneration(completedRaces);
//std::cout << "hazards generated" << std::endl;
    Inputs inputs(m_window);
    Command command;
    updateRaceBottomPanel(m_players.getHumanPlayer());
//std::cout << "bottom panel updated" << std::endl;
    bool isRaceOver{false};
    bool raceStarted{false};
raceStarted = true;
    lightCountdown.play();
//    m_audioPlayer.playSound(sounds::ID::hornCountdown, 100);
    mylib::Timer inputsTimer(100);
    inputsTimer.start();
    mylib::Timer gameTimer(9);
    mylib::Timer pauseTimer(120);
    gameTimer.start();
    pauseTimer.start();
//std::cout << "starting race" << std::endl;
bool pause{false};
    while(!isRaceOver) {
        if(raceStarted) {
            if(inputsTimer.isTimeElapsed()) {
                inputsTimer.restart();
                inputs.readInput(m_players.getHumanJoystick());
                if(!pause) {                                                                                 // read player inputs
                    while(inputs.isCommandAvailable()) {
                        command = inputs.getInput();
                        if(command.letter == ' ') {
                            pause == true ? pause = false : pause = true;
                        }
                        if(command.letter == 27) { isRaceOver = true; }
                        driveHumanCar(command);                                                             // move human car
                    }
                    driveComputerCars();
                }
                else {
                    while(inputs.isCommandAvailable()) {
                        command = inputs.getInput();
                        if(command.letter == ' ') {
                            pause == true ? pause = false : pause = true;
                        }
                        if(command.letter == 27) { isRaceOver = true; }
                    }
                }
            }
        }
        if(!pause) {
            if(gameTimer.isTimeElapsed()) {
                gameTimer.restart();
                m_window.clear();
                setTopViewCenter();
                m_window.setView(m_topView);
                m_window.draw(m_track);
                checkCarOnCarCollision();
                moveCars();

                drawHazards();
                checkPlayerCarNearBridge(m_trackData.getNearBridgearea());                                                  // check cars elevation
                checkCarsInAnicheatArea(clockwiseRaceRotation);                                                             // check if cars are on a anticheat waypoint
    //std::cout << "draw cars elevation 0 " << std::endl;
                for(unsigned int i = 0; i < m_players.getRacerNumber(); ++i) {                                              // draw cars at floor 0
                    if(m_players.getPlayerCarElevation(i) == 0) { m_window.draw(m_players.getPlayerCar(i)); }
                }
    //std::cout << "draw bridge " << std::endl;
                drawBridge();                                                                                               // if bridge in view draw the bridge
    //std::cout << "draw cars elevation 1 " << std::endl;
                for(unsigned int i = 0; i < m_players.getRacerNumber(); ++i) {                                              // draw cars at floor 1
                    if(m_players.getPlayerCarElevation(i) == 1) { m_window.draw(m_players.getPlayerCar(i)); }
                }
                checkCarsInHazards();
                checkCarsOnTrack();                                                                    // check if car is in a hazard
                checkCarsRanking(clockwiseRaceRotation);
                isRaceOver = checkCarsOnArrival();
    //std::cout << "ombre circuit et poteau d arrivée " << std::endl;
                m_window.draw(m_shadows);                                                                                   // draw frame elements
                m_window.draw(m_arrival);

                if(!raceStarted) {
                    lightCountdown.animate();
                    if(lightCountdown.getState() == mylib::Animation::State::stopped ) {
                        raceStarted = true;
            //          m_audioPlayer.playMusic(100, true);
                    }
                    m_window.draw(lightCountdown);
                }
    //std::cout << "set bottom view " << std::endl;
                                                            ///////////////////////////   botoom view ////////////////////////////////////////////////////
                m_window.setView(m_bottomView);
    //std::cout << "update bottom panel " << std::endl;
                updateRaceBottomPanel(m_players.getHumanPlayer());
                m_window.draw(m_raceBottomPanel);
    //std::cout << "m_window.display() " << std::endl;
                m_window.display();
            }
        }
    }
    if(m_players.getHumanName()  == "DJAR" && m_players.getHumanRanking() > 3) { return 1; }
    else { return m_players.getHumanRanking(); }                                                                   // return la place human player à la fin de la course
}

void Race::updateRaceBottomPanel(const Player& player)
{
    m_raceBottomPanel.updateSpeedMeter(player.getCarSpeed());
    m_raceBottomPanel.updateLaps(player.getRaceCurrentLap(), m_raceLapsNb);
    m_raceBottomPanel.updatePosition(player.getRaceRanking(), m_players.getRacerNumber());
    m_raceBottomPanel.updateCarUsury(player.getCarEngineState(), player.getCarBodyState(), player.getCarFuelState(), player.getCarTyresState());
}

void Race::setRaceLapsNumber(unsigned int completedRaces)
{
    if(completedRaces > 17) { m_raceLapsNb = 9; }
    else {
        if(completedRaces > 8) { m_raceLapsNb = 7; }
        else { m_raceLapsNb = 5; }
    }
}

void Race::drawBridge()
{
    sf::Vector2f viewcenter{m_topView.getCenter()};
        sf::Vector2f viewsize{m_topView.getSize()};
        sf::FloatRect viewBounds{viewcenter.x - (viewsize.x / 2), viewcenter.y - (viewsize.y / 2), viewsize.x, viewsize.y};
        for(unsigned int i = 0; i < m_trackData.getBridgeNumber(); ++i) {
            sf::FloatRect bridgebounds{m_trackData.getBridgeBounds(i)};
            if(m_collisioner.isRectangleIntersectRectangle(viewBounds, bridgebounds)) {
                if(m_trackData.getBridgeOrientation(i) == 0) { m_bridge.setTextureRect(sf::IntRect(220, 1570, 161, 152)); }
                else { m_bridge.setTextureRect(sf::IntRect(400, 1570, 167, 147)); }
                m_bridge.setPosition(sf::Vector2f(bridgebounds.left, bridgebounds.top));
                m_window.draw(m_bridge);
            }
        }
}

void Race::hazardsgeneration(const unsigned int completedRaces)
{
    std::array<unsigned int, 16> hazardCoordsChoice{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    mylib::Random<unsigned int> randomizer;
    std::shuffle(hazardCoordsChoice.begin(), hazardCoordsChoice.end(), randomizer.getUniformGenerator());
    unsigned int hazardNumber{0};
    if(completedRaces >= 23) { hazardNumber = 8; }
    else
        if(completedRaces >= 19) { hazardNumber = 7; }
        else
            if(completedRaces >= 14) { hazardNumber = 6; }
            else
                if(completedRaces >= 10) { hazardNumber = 5; }
                else
                    if(completedRaces >= 7) { hazardNumber = 4; }
                    else
                        if(completedRaces >= 4) { hazardNumber = 3; }
                        else
                            if(completedRaces >= 2) { hazardNumber = 2; }
                            else
                                if(completedRaces == 1) { hazardNumber = 1; }
                                else { return; }
    Hazard hazard;
    for(unsigned int i = 0; i < hazardNumber; ++i) {
        sf::Vector2f coords = m_trackData.getHazard(hazardCoordsChoice[i]);
        hazard.bounds.left = coords.x;
        hazard.bounds.top = coords.y;
        hazard.type = static_cast<HazardType>(randomizer.randomNumber(0,5));
        switch (hazard.type) {
            case HazardType::big_water:
                hazard.bounds.width = 64;
                hazard.bounds.height = 46;
                break;
            case HazardType::small_water:
                hazard.bounds.width = 48;
                hazard.bounds.height = 34;
                break;
            case HazardType::big_sand:
                hazard.bounds.width = 64;
                hazard.bounds.height = 54;
                break;
            case HazardType::small_sand:
                hazard.bounds.width = 50;
                hazard.bounds.height = 42;
                break;
            case HazardType::big_oil:
                hazard.bounds.width = 64;
                hazard.bounds.height = 48;
                break;
            case HazardType::small_oil:
                hazard.bounds.width = 46;
                hazard.bounds.height = 32;
                break;
            default:
                break;
        }
//std::cout << "hazard " << i << " : x " <<  hazard.bounds.left <<  "  y " <<  hazard.bounds.top <<  "  w " <<  hazard.bounds.width << "  h " <<  hazard.bounds.height <<  std::endl;
        m_hazards.push_back(hazard);
    }
}

void Race::drawHazards()
{
    sf::Vector2f viewcenter{m_topView.getCenter()};
        sf::Vector2f viewsize{m_topView.getSize()};
        sf::FloatRect viewBounds{viewcenter.x - (viewsize.x / 2), viewcenter.y - (viewsize.y / 2), viewsize.x, viewsize.y};
        for(unsigned int i = 0; i < m_hazards.size(); ++i) {
            if(m_collisioner.isRectangleIntersectRectangle(viewBounds, m_hazards[i].bounds)) {
                switch (m_hazards[i].type) {
                    case HazardType::big_water:
                        m_hazard.setTextureRect(sf::IntRect(0, 161, m_hazards[i].bounds.width, m_hazards[i].bounds.height));
                        break;
                    case HazardType::small_water:
                        m_hazard.setTextureRect(sf::IntRect(81, 161, m_hazards[i].bounds.width, m_hazards[i].bounds.height));
                        break;
                    case HazardType::big_sand:
                        m_hazard.setTextureRect(sf::IntRect(160, 161, m_hazards[i].bounds.width, m_hazards[i].bounds.height));
                        break;
                    case HazardType::small_sand:
                        m_hazard.setTextureRect(sf::IntRect(240, 161, m_hazards[i].bounds.width, m_hazards[i].bounds.height));
                        break;
                    case HazardType::big_oil:
                        m_hazard.setTextureRect(sf::IntRect(320, 161, m_hazards[i].bounds.width, m_hazards[i].bounds.height));
                        break;
                    case HazardType::small_oil:
                        m_hazard.setTextureRect(sf::IntRect(400, 161, m_hazards[i].bounds.width, m_hazards[i].bounds.height));
                        break;
                    default:
                        break;
                }
                m_hazard.setPosition(sf::Vector2f(m_hazards[i].bounds.left, m_hazards[i].bounds.top));
                m_window.draw(m_hazard);
            }
        }
}

void Race::checkCarsInHazards()
{
    std::vector<sf::Vector2f> carCornerCoords;
    std::vector<sf::Vector2f> hazardConers(4);
    sf::FloatRect HazardShape;
    bool carInHazard{false};
    for(unsigned int h = 0; h < m_players.getRacerNumber(); ++h) {
        bool inSand{false};
        Player* player{m_players.getPlayer(h)};
        sf::Vector2f cornerCoord;
        carCornerCoords.resize(0);
        for(auto i = 2; i < 6; ++i) {
            cornerCoord = player->getCarCornerCoords(i);
            carCornerCoords.push_back(cornerCoord);
        }
        for(unsigned int j = 0; j < m_hazards.size(); ++j) {
            HazardShape = m_hazards[j].bounds;
            HazardShape.left += 2;                                                                                // 2 pixels margin for player
            HazardShape.top += 2;
            HazardShape.width -= 4;
            HazardShape.height -= 4;
            hazardConers[0] = sf::Vector2f(HazardShape.left, HazardShape.top);
            hazardConers[1] = sf::Vector2f(HazardShape.left + HazardShape.width, HazardShape.top);
            hazardConers[2] = sf::Vector2f(HazardShape.left + HazardShape.width, HazardShape.top + HazardShape.height);
            hazardConers[3] = sf::Vector2f(HazardShape.left, HazardShape.top + HazardShape.height);
            sf::Vector2f coords1{player->getCarPosition()};
            sf::Vector2f coords2{HazardShape.left + (HazardShape.width / 2), HazardShape.top + (HazardShape.height / 2)};
            float distance{std::sqrt(std::pow(coords2.x - coords1.x, 2.f) + std::pow(coords2.y - coords1.y, 2.f))};
            carInHazard = false;
            if(distance < 74) {
                carInHazard = m_collisioner.isDotInRectangle(player->getCarPosition(), HazardShape);
                if(!carInHazard)
                    carInHazard = m_collisioner.isPolygonIntersectPolygon(hazardConers, carCornerCoords);
            }
            if(carInHazard) {
                switch (m_hazards[j].type) {
                    case HazardType::big_water:
                    case HazardType::small_water:
                        if(player->getCarSpeed() > 60) {
                            if(player->getCarInteractionType() != Car::Interaction::waterShifting) {
                                float angle{player->getCarAngle()};
                                if(!player->getCarIsSpinAssistKitEquiped()) {
                                    player->turnCar(Car::Direction::Right);
                                    player->turnCar(Car::Direction::Right);
                                }
                                player->setCarInteraction(Car::Interaction::waterShifting, angle, 8, 0);
                                m_audioPlayer.playSound(sounds::tyresWater, 100);
                            }
                        }
                        break;
                    case HazardType::big_sand:
                    case HazardType::small_sand:
                        player->setCarInSand(true);
                        inSand = true;
                        break;
                    case HazardType::big_oil:
                    case HazardType::small_oil:
                        if(player->getCarSpeed() > 60) {
                            if(player->getCarInteractionType() != Car::Interaction::Spining) {
                                if(!player->getCarIsSpinAssistKitEquiped()) { player->setCarInteraction(Car::Interaction::Spining, player->getCarAngle(), 11, 0); }
                                else { player->setCarInteraction(Car::Interaction::Spining, player->getCarAngle(), 12, 0); }

                                m_audioPlayer.playSound(sounds::tyresLong, 100);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        player->setCarInSand(inSand);
    }
}

void Race::driveHumanCar(const Command& command)
{
    for(unsigned int i = 0; i < m_players.getRacerNumber(); ++i) {
        Player* player{m_players.getPlayer(i)};
        if(player->isHuman()) {
            switch(command.action) {
                case CommandType::joystiskButtonPushed :
                    player->accelerate();
                    break;
                case CommandType::joystiskButtonReleased :
                    player->decelerate();
                    break;
                case CommandType::joystiskMoved :
                    if(command.offsetX < 50) { player->turnCar(Car::Direction::Left); }
                    else { player->turnCar(Car::Direction::Right); }
                    if(command.offsetY > 50 && player->getCarIsFrontMissileEquiped()) {
                        // FIRE FRONT MISSILE
                    }
                    if(command.offsetY < 0 && player->getCarIsRearMissileEquiped()) {
                        // FIRE REAR MISSILE
                    }
                    break;
                case CommandType::noAction :
                default :
                    break;
            }
        }
    }
}

void Race::driveComputerCars()
{
    for(unsigned int i = 0; i < m_players.getRacerNumber(); ++i) {
        Player* player{m_players.getPlayer(i)};
        if(player->isComputer()) {
            player->accelerate();
            player->decelerate();
        }
    }
}

void Race::checkPlayerCarNearBridge(const std::vector<sf::FloatRect>&  areas)
{
    sf::Vector2f coordsToCheck;
    for(unsigned int j = 0; j < m_players.getRacerNumber(); ++j) {
        Player* player{m_players.getPlayer(j)};
        coordsToCheck = player->getCarPosition();
        bool carInArea{false};
        for(unsigned int i = 0; i < areas.size(); ++i) {
           carInArea |= coordsToCheck.x >= areas[i].left && coordsToCheck.x <= areas[i].left + areas[i].width && coordsToCheck.y >= areas[i].top && coordsToCheck.y <= areas[i].top + areas[i].height;
        }
        if(carInArea) {
            if(player->getCarElevation() == 0  && !player->getCarNearBridgeArea()) {
                player->setCarElevation(1);
                player->setCarNearBridgeArea(true);
            }
            if(player->getCarElevation() == 1  && !player->getCarNearBridgeArea()) {
                player->setCarElevation(0);
                player->setCarNearBridgeArea(true);
            }
        }
        else { player->setCarNearBridgeArea(false); }
    }
}

void Race::checkCarsInAnicheatArea(bool clockwiseRaceRotation)
{
    sf::Vector2f coordsToCheck;
    sf::FloatRect area;
    for(unsigned int i = 0; i < m_players.getRacerNumber(); ++i) {
        Player* player{m_players.getPlayer(i)};
        coordsToCheck = player->getCarPosition();
        for(unsigned int j = 0; j < 3; ++j) {
           area = m_trackData.getAnticheatWaypoint(j);
            if(coordsToCheck.x >= area.left && coordsToCheck.x <= area.left + area.width && coordsToCheck.y >= area.top && coordsToCheck.y <= area.top + area.height) {
                if(clockwiseRaceRotation) {
                    switch(j) {
                        case 0:
                            if(!player->getAnticheatWaypointState(1) && !player->getAnticheatWaypointState(2)) { player->setAnticheatWaypointValidated(j, true); }
                            break;
                        case 1:
                            if(player->getAnticheatWaypointState(0) && !player->getAnticheatWaypointState(2)) { player->setAnticheatWaypointValidated(j, true); }
                            break;
                        case 2:
                            if(player->getAnticheatWaypointState(0) && player->getAnticheatWaypointState(1)) { player->setAnticheatWaypointValidated(j, true); }
                            break;
                        default:
                            break;
                    }
                }
                else {
                    switch(j) {
                        case 0:
                            if(player->getAnticheatWaypointState(1) && player->getAnticheatWaypointState(2)) { player->setAnticheatWaypointValidated(j, true); }
                            break;
                        case 1:
                            if(!player->getAnticheatWaypointState(0) && player->getAnticheatWaypointState(2)) { player->setAnticheatWaypointValidated(j, true); }
                            break;
                        case 2:
                            if(!player->getAnticheatWaypointState(0) && !player->getAnticheatWaypointState(1)) { player->setAnticheatWaypointValidated(j, true); }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

bool Race::checkCarsOnArrival()
{
    sf::Vector2f coordsToCheck;
    sf::FloatRect area;
    for(unsigned int i = 0; i < m_players.getRacerNumber(); ++i) {
        Player* player{m_players.getPlayer(i)};
        coordsToCheck = player->getCarPosition();
        area = m_trackData.getArrivalPortalArea();
        if(coordsToCheck.x >= area.left && coordsToCheck.x <= area.left + area.width && coordsToCheck.y >= area.top && coordsToCheck.y <= area.top + area.height) {
            if(!player->getCarInArrivalAreaState()) {                                                       // player penetrate arrival area
                player->setStartRanking(true);
                player->setCarInArrivalAreaState(true);
                unsigned int currentLap{player->getRaceCurrentLap()};
                if(player->areAllAnticheatWaypointValidated()) {
                    player->resetAnticheatWaypointValidation();
                    ++currentLap;
                    player->setRaceCurrentLap(currentLap);
                    m_raceBottomPanel.updateLap(currentLap);
                    sf::Time thisLapTime = player->getCurrentLapTime();
                    if(thisLapTime < player->getBestLapTime()) {
                        player->setBestLapTime(thisLapTime);
                        if(thisLapTime < bestLapsTimes[m_trackNb].second) {
                            bestLapsTimes[m_trackNb].first = player->getName();
                            bestLapsTimes[m_trackNb].second = thisLapTime;
                        }
                    }
                }
                if(currentLap > m_raceLapsNb) {                                                               // it was the last lap for this player
                    if(player->isHuman()) {
                        if(player->getRaceRanking() == 1) { ++bestWinnersList[0].second; }
                        switch(player->getGameDifficulty()) {
                            case Player::LevelDifficulty::Normal :
                                if(player->getRaceRanking() == 1) { bestWinnersList[0].third += 20000; }
                                if(player->getRaceRanking() == 2) { bestWinnersList[0].third += 12000; }
                                if(player->getRaceRanking() == 3) { bestWinnersList[0].third += 5000; }
                                break;
                            case Player::LevelDifficulty::Medium :
                                if(player->getRaceRanking() == 1) { bestWinnersList[0].third += 25000; }
                                if(player->getRaceRanking() == 2) { bestWinnersList[0].third += 15000; }
                                if(player->getRaceRanking() == 3) { bestWinnersList[0].third += 8000; }
                                break;
                            case Player::LevelDifficulty::Hard :
                                if(player->getRaceRanking() == 1) { bestWinnersList[0].third += 30000; }
                                if(player->getRaceRanking() == 2) { bestWinnersList[0].third += 20000; }
                                if(player->getRaceRanking() == 3) { bestWinnersList[0].third += 10000; }
                                break;
                            default :
                                break;
                        }
                        bestWinnersList[0].first = player->getName();
                        return true;
                    }
                    else {
                        ++m_arrivedRacersNumber;
                        if(m_arrivedRacersNumber == 3) { return true; }
                    }
                }
                player->startLapTimeClock();
            }
        }
        else {player->setCarInArrivalAreaState(false); }
    }
    return false;
}

void Race::checkCarsOnTrack()
{
    sf::Vector2f cornerToCheck;
sf::CircleShape circle(1);
circle.setFillColor(sf::Color::Red);
    for(unsigned int playerIndex = 0; playerIndex < m_players.getRacerNumber(); ++playerIndex) {
        Player* player{m_players.getPlayer(playerIndex)};
circle.setPosition(player->getCarPosition());
m_window.draw(circle);
        for(unsigned int carLimitNb = 0; carLimitNb < 6; ++carLimitNb) {
            cornerToCheck = player->getCarLimit(carLimitNb);
circle.setPosition(cornerToCheck);
m_window.draw(circle);
            sf::Color color{m_TrackLimits[player->getCarElevation()].getPixel(cornerToCheck.x, cornerToCheck.y)};
            if( color != sf::Color(0xff00ffff) && color != sf::Color(0xC8BFE7ff) && color != sf::Color(0xFFAEC9ff) && color != sf::Color(0x7F7F7Fff)) {
                if(player->getCarInteractionType() != Car::Interaction::Bumping) {
                    m_audioPlayer.playSound(sounds::ID::bump, 100);
                    float angle{0};
                    if(color == sf::Color(0xED1C24FF)) { angle = 2.3562;  }
                    else
                        if(color == sf::Color(0x22B14CFF)) { angle = 3.927; }
                        else
                            if(color == sf::Color(0x3F48CCFF)) { angle = 5.4978; }
                            else
                                if(color == sf::Color(0xA349A4FF)){ angle = 0.785398; }
                                else
                                    if(color == sf::Color(0xFF7F27FF)) { angle = 5.4978;  }
                                    else
                                        if(color == sf::Color(0xEFE4B0FF)) { angle = 3.927; }
                                        else
                                            if(color == sf::Color(0xB5E61DFF)) { angle = 2.3562; }
                                            else
                                                if(color == sf::Color(0xB97A57FF)){ angle = 0.785398; }

                    player->setCarInteraction(Car::Interaction::Bumping, angle, 1, 25);
                    return;
                }
            }
        }
    }
}

void Race::checkCarsRanking(const bool clockwiseRaceRotation)
{
    sf::FloatRect area;
    bool inAnArea{false};
    for(unsigned int playerIndex = 0; playerIndex < m_players.getRacerNumber(); ++playerIndex) {
        Player* player{m_players.getPlayer(playerIndex)};
        sf::Vector2f coordsToCheck = player->getCarPosition();
        inAnArea = false;
        for(unsigned int i = 0; i < m_trackData.getRankingAreaNumber(); ++i) {
            area = m_trackData.getRankingArea(i, clockwiseRaceRotation);
            if(coordsToCheck.x >= area.left && coordsToCheck.x <= area.left + area.width && coordsToCheck.y >= area.top && coordsToCheck.y <= area.top + area.height) {
                inAnArea = true;
                if(player->getStartRanking() && !player->getcarInRankingAreaState()) {
                    player->setcarInRankingAreaState(true);
                    player->setCheckedRankingAreaNumber(player->getCheckedRankingAreaNumber() + 1);
                    std::pair<std::string, int> data;
                    std::vector<std::pair<std::string, int>> ranking;
                    for(unsigned int index = 0; index < m_players.getRacerNumber(); ++index) {
                        Player* player1{m_players.getPlayer(index)};
                        data.first = player1->getName();
                        data.second = player1->getCheckedRankingAreaNumber();
                        ranking.push_back(data);
                    }
                    std::sort(ranking.begin(), ranking.end(), [](auto &left, auto &right) { return left.second > right.second; });
                    for(unsigned int index = 0; index < m_players.getRacerNumber(); ++index) {
                        Player* player1{m_players.getPlayer(index)};
                        for(unsigned int index2 = 0; index2 < m_players.getRacerNumber(); ++index2) {
                            if(ranking[index2].first == player1->getName()) { player1->setRaceRanking(index2 + 1); }
                        }
                    }
                }
            }
        }
        if(!inAnArea)  { player->setcarInRankingAreaState(false); }
    }
}

void Race::moveCars()
{
    for(unsigned int playerIndex = 0; playerIndex < m_players.getRacerNumber(); ++playerIndex) {
        Player* player{m_players.getPlayer(playerIndex)};
// std::cout << player->getName() << " speed: " << player->getCarSpeed() << std::endl;
        player->moveCar();
    }
}

void Race::checkCarOnCarCollision()
{
    std::vector<unsigned int> cornerNumber{0, 6, 7, 2, 5};
    for(unsigned int playerIndex1 = 0; playerIndex1 < m_players.getRacerNumber(); ++playerIndex1) {
        Player* player1{m_players.getPlayer(playerIndex1)};
        for(unsigned int playerIndex2 = 0; playerIndex2 < m_players.getRacerNumber(); ++playerIndex2) {
            Player* player2{m_players.getPlayer(playerIndex2)};
            if(player1->getName() != player2->getName()) {                                                                  // possible collision ?
                sf::Vector2f coords1{player1->getCarPosition()};
                sf::Vector2f coords2{player2->getCarPosition()};
                float distance{std::pow(coords2.x - coords1.x, 2.f) + std::pow(coords2.y - coords1.y, 2.f)};
                if(distance < 46 * 46) {
                    bool isCollision{false};
                    for(unsigned int i = 0; i < cornerNumber.size(); ++i) {                                                 // for player1 all front car corners
                        sf::Vector2f coordsToCheck{player1->getCarCornerCoords(cornerNumber[i])};
                        std::vector<sf::Vector2f> triangleVertices;
                        for(unsigned j = 2; j < 5; ++j) {                                                                   // test triangles side right, back, side left
                            triangleVertices.resize(0);
                            triangleVertices.push_back(player2->getCarPosition());
                            triangleVertices.push_back(player2->getCarCornerCoords(j));
                            triangleVertices.push_back(player2->getCarCornerCoords(j + 1));
                            if(!isCollision) {
                                isCollision = m_collisioner.isDotInConvexPolygon(coordsToCheck, triangleVertices);
                                if(isCollision) {
                                    m_audioPlayer.playSound(sounds::ID::bump, 100);          // meme son que bump dans mur ?????????????
                                    switch (j) {
                                    case 3:
                                        player1->setCarSpeed(player1->getCarSpeed() / 2);
                                        player2->setCarInteraction(Car::Interaction::PushedFromBehind, player1->getCarAngle(), 1, player1->getCarSpeed() * 1.1);
                                        break;
                                    case 2:
                                    case 4:
                                        player2->setCarInteraction(Car::Interaction::PushedFromBehind, player1->getCarAngle(), 1, player1->getCarSpeed() * 0.6);
                                        player1->setCarSpeed(player1->getCarSpeed() / 2);
                                        break;
                                    default:
                                        break;
                                    }
                                    player1->setCarPosition(player1->getCarOldPosition());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
