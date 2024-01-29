#include "header/race.hpp"

#include <string>

#include <SFML/Graphics/Sprite.hpp>

#include "timer/timer.hpp"
#include "header/carTextureColorModifier.hpp"

Race::Race(sf::RenderWindow& window, const unsigned int trackNb, Players& players, const unsigned int language) :
                                                                                    m_window(window), m_trackData(trackNb + 1), m_textureContaigner(), m_trackNb{trackNb},
                                                                                    m_players{players}, m_language{language}, m_raceBottomPanel(language),
                                                                                    m_currentLap{1}, m_raceLapsNb{5}
{
    loadTextures(trackNb);
}

void Race::loadTextures(const unsigned int trackNb)
{
    m_textureContaigner.loadAsset(textures::ID::track, "graphics/tracks/races/track" + std::to_string(trackNb + 1) + ".png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
    m_textureContaigner.loadAsset(textures::ID::shadows, "graphics/tracks/shadows/track" + std::to_string(trackNb + 1) + " shadows.png");
    m_textureContaigner.loadAsset(textures::ID::roadUp, "graphics/tracks/roads/track" + std::to_string(trackNb + 1) + " up.png");
    m_textureContaigner.loadAsset(textures::ID::roadDown, "graphics/tracks/roads/track" + std::to_string(trackNb + 1) + " down.png");
}

void Race::initViews()
{
    m_topView.setSize(sf::Vector2f(512.f, 350.f));
    m_topView.setViewport(sf::FloatRect(0.1f, 0.f, 0.8f, 0.885f));
    m_bottomView.setSize(sf::Vector2f(512.f, 40.f));
    m_bottomView.setCenter(256, 20);
    m_bottomView.setViewport(sf::FloatRect(0.1f, 0.9f, 0.8f, 0.1f));
}

void Race::initSprites()
{
    m_track.setTexture(m_textureContaigner.getAsset(textures::ID::track));
    m_track.setTextureRect(sf::IntRect(0, 0, 1280, 800));
    m_track.setPosition(0, 0);
    m_shadows.setTexture(m_textureContaigner.getAsset(textures::ID::shadows));
    m_shadows.setTextureRect(sf::IntRect(0, 0, 1280, 800));
    m_shadows.setPosition(0, 0);
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
    for(unsigned int i = 8 - m_players.getRacerNumber(), h = 0, j = 0; h < m_players.getRacerNumber(); ++i, ++h, ++j) {
        sf::Vector2f spawnCoords = m_trackData.getCarSpawnCoords(i, clockwiseRaceRotation);
        m_players.setCarPosition(spawnCoords, j);
        m_players.setCarAngle(m_trackData.getCarSpawnangle(i, clockwiseRaceRotation), j);
        m_players.setCarStartFrame(j);
    }
}

unsigned int Race::racing(unsigned int lastRaceRanking, unsigned int completedRaces, bool clockwiseRaceRotation)
{
    initViews();
    initSprites();
    setRaceLapsNumber(completedRaces);
    setRacersOnStartingPosition(clockwiseRaceRotation);
    mylib::Timer moveCarsTimer(350);
    moveCarsTimer.startTimer();
    while(1) {
        updateRaceBottomPanel(m_players.getHumanPlayer());
        m_window.clear();
        setTopViewCenter();
        m_window.setView(m_topView);
        m_window.draw(m_track);

        if(moveCarsTimer.isTimeElapsed()) {
            m_players.moveCars();
            moveCarsTimer.restartTimer();


            for(unsigned int i = 0; i < m_players.getRacerNumber(); ++i) {
                m_window.draw(m_players.getPlayerCar(i));
            }

            m_window.draw(m_shadows);

            m_window.setView(m_bottomView);
            m_window.draw(m_raceBottomPanel);

            m_window.display();
        }
    }
    return 1;                                           /////////// return la place à la fin de la course
}

void Race::updateRaceBottomPanel(const Player& player)
{
    m_raceBottomPanel.updateSpeedMeter(player.getCarSpeed());
    m_raceBottomPanel.updateLaps(m_currentLap, m_raceLapsNb);
    m_raceBottomPanel.updatePosition(4, 4);                             /////////////////////////////////  à modifier
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
