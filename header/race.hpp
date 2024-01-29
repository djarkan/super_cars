#ifndef    RACE_HPP
#define    RACE_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

#include <jsonfile/jsonfile.hpp>
#include <assetcontainer/assetcontainer.hpp>
#include "header/assetkey.hpp"
#include "header/track.hpp"
#include "header/players.hpp"
#include "header/racebottompanel.hpp"

class Race {
    public:
                                                                        Race(sf::RenderWindow& window, const unsigned int trackNb, Players& players, const unsigned int language);
                void                                                    loadTextures(const unsigned int trackNb);
                unsigned int                                            racing(unsigned int lastRaceRanking, unsigned int completedRaces, bool clockwiseRaceRotation);
                void                                                    updateRaceBottomPanel(const Player& player);
                void                                                    setRacersOnStartingPosition();
                void                                                    setTopViewCenter();
                void                                                    setRacersOnStartingPosition(bool clockwiseRaceRotation);


    private:
                sf::RenderWindow&                                       m_window;
                sf::View                                                m_topView;
                sf::View                                                m_bottomView;
                sf::Sprite                                              m_track;
                Track                                                   m_trackData;
                sf::Sprite                                              m_shadows;
                mylib::AssetContainer<textures::ID, sf::Texture>        m_textureContaigner;
                const unsigned int                                      m_trackNb;
                Players&                                                m_players;
                const unsigned int                                      m_language;
                RaceBottomPanel                                         m_raceBottomPanel;
                unsigned int                                            m_currentLap;
                unsigned int                                            m_raceLapsNb;

                void                                                    initViews();
                void                                                    initSprites();
                void                                                    setRaceLapsNumber(unsigned int completedRaces);
};
#endif
