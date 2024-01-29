#include <header/game.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System.hpp>

#include <iostream>

#include "assetcontainer/assetcontainer.hpp"
#include "header/race.hpp"
#include "header/track.hpp"
#include "timer/timer.hpp"
#include "music/music.hpp"
#include "header/garage.hpp"
#include "jsonfile/jsonfile.hpp"
#include "header/office.hpp"
#include "header/inputs.hpp"
#include "header/message.hpp"
#include "header/players.hpp"

#include <string>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

extern sf::Vector2f windowRatio;

Game::Game(sf::RenderWindow& window, unsigned int language, Players& players):    m_window{window}, m_language{language}, m_players{players},
                                                                m_inputs(m_window), m_languageJson(), m_bottomPanel(m_players.getHumanPlayer(),
                                                                m_window, m_language), m_completedRaces{0}, m_lastRaceRanking{1}, m_clockwiseRaceRotation{true}
{
    switch(m_language) {
        case 0 :
            m_languageJson.loadJsonFile("language/english/game.json");
            break;
        case 1:
            m_languageJson.loadJsonFile("language/usa/game.json");
            break;
        case 2 :
            m_languageJson.loadJsonFile("language/french/game.json");
            break;
        default :
            break;
    }
    setHandCursor();
    loadTextures();
    loadClickBoxes();
    loadButtons();
    initViews();
    m_raceListState.fill(false);
    m_completedRaces = m_players.getGameDifficulty() * 9;
}

bool Game::launch()  // json langage et si json de config reçoit les tableaux scores classement nb races/money et liste meilleurs temps/pseudo par ref non const
{
    bool focus{true};
    mylib::Music music;
    music.loadMusic("music/track1.wav");
    music.setMusicLoop(true);
    music.setMusicVolume(100);
//    music.playMusic();
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::game));
    background.setTextureRect(sf::IntRect(0, 0, 640, 360));
    mylib::Timer inputsTimer(10);
    inputsTimer.startTimer();
    m_bottomPanel.textConfiguration();
    Garage garage(m_window, m_players.getHumanPlayer(), m_bottomPanel, m_language);
    Office office(m_window, m_players.getHumanPlayer(), m_bottomPanel, m_language);
    bool canGoToOffice{true};
    m_window.setMouseCursorVisible(true);
    bool gameover{false};
    bool allRacesDone{false};
    while(m_window.isOpen() && !gameover){                                    ///////////////////////////// virer la boucle d'evenement !!!!!!!!!!!!!!!!!!!!!
        if(inputsTimer.isTimeElapsed()) {
            m_inputs.readInput(m_players.getHumanJoystick());
            inputsTimer.restartTimer();
        }
        if(m_inputs.isCommandAvailable()) {
            m_command = m_inputs.getInput();
            switch (m_command.action) {
                case CommandType::noAction :
                    checkButtons();
                    break;
                case CommandType::mouseLeftButtonPressed :
                case CommandType::joystiskButtonPressed :
                {
                    if(m_simpleClickBoxes[0]->isPressed(m_command.coords)) {
                        loading(2000, m_languageJson.m_Root["message"].asString());
                        garage.carUpdate();
                        garage.setInitGarage(false);
                        loading(2000, m_languageJson.m_Root["message"].asString());
                    }
                    if(m_simpleClickBoxes[1]->isPressed(m_command.coords) && canGoToOffice) {
                        canGoToOffice = false;
                        loading(2000, m_languageJson.m_Root["message"].asString());
                        office.buy();
                        loading(2000, m_languageJson.m_Root["message"].asString());
                    }
                    int whatTrack = checkButtons();
                    if(whatTrack >= 0 && whatTrack < 9) {
                        if(!m_raceListState[whatTrack]) {
                            m_window.setMouseCursorVisible(false);
                            m_players.buildPlayers(m_completedRaces, m_lastRaceRanking);
                            m_lastRaceRanking = race(whatTrack, m_lastRaceRanking);
                            if(m_lastRaceRanking > 3) { gameover = true; }
                            bool testFlag{true};
                            for(bool elem : m_raceListState) { testFlag |= elem; }
                            if(testFlag && !gameover) {
                                m_players.levelupGameDifficulty();
                                allRacesDone = true;
                            }
                            ++m_completedRaces;
                            m_raceListState[whatTrack] = true;
                            m_window.setMouseCursorVisible(true);
                            garage.setInitGarage(true);
                            canGoToOffice = true;
                        }
                    }
                    for(auto i = 0; i < 3; ++i) {
                        if(m_ComplexClickBoxes[i]->isTriggerred(m_command.coords)) {
                            loading(2000, m_languageJson.m_Root["message"].asString());
                            sf::Sprite carSpecs;
                            sf::Texture textureSDpecs;
                            switch (i) {
                                case 0 :
                                    carSpecs.setTexture(m_textureContaigner.getAsset(textures::ID::whitecar));
                                    break;
                                case 1 :
                                    carSpecs.setTexture(m_textureContaigner.getAsset(textures::ID::bluecar));
                                    break;
                                case 2 :
                                    carSpecs.setTexture(m_textureContaigner.getAsset(textures::ID::redcar));
                                    break;
                                default :
                                    break;
                            }
                            carSpecs.setTextureRect(sf::IntRect(0, 0, 640, 400));
                            m_window.clear();
                            sf::View view;
                            view.setSize(640, 400);
                            view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                            view.setCenter(sf::Vector2f(320, 200));
                            m_window.setView(view);
                            m_window.draw(carSpecs);
                            m_window.display();
                            m_inputs.clearCommand(m_command);
                            while(m_command.action != CommandType::joystiskButtonPressed && m_command.action != CommandType::mouseLeftButtonPressed) {
                                m_inputs.readInput(m_players.getHumanJoystick());
                                if(m_inputs.isCommandAvailable()) { m_command = m_inputs.getInput(); }
                            }
                            loading(2000, m_languageJson.m_Root["message"].asString());
                            m_inputs.clearCommand(m_command);
                        }
                    }
                }
                case CommandType::joystiskMoved :
                    break;
                case CommandType::keyboardPressed :
                    break;
                case CommandType::lostFocus :
                    focus = false;
                    break;
                case CommandType::gainFocus :
                    focus = true;
                    break;
                default :
                    break;
            }
            if(m_command.action == CommandType::joystiskMoved) {
                sf::Vector2i mouseCoords = sf::Mouse::getPosition(m_window);
                mouseCoords.x += ((m_command.offsetX / 100) * 4);
                mouseCoords.y += ((m_command.offsetY / 100) * 4);
                sf::Mouse::setPosition(mouseCoords, m_window);
            }
            if(focus) {
                m_window.clear();
                m_window.setView(m_topView);
                m_window.draw(background);
                drawRacePannel();
                for(auto i = 0; i < 9; ++i) { m_window.draw(*m_racesButtonsArray[i]); }
                m_window.setView(m_bottomView);
                m_bottomPanel.updateUsury();
                m_window.draw(m_bottomPanel);
                m_window.display();
            }
        }
    }
    return allRacesDone;
}

void Game::loadTextures()
{
    m_textureContaigner.loadAsset(textures::ID::game, "graphics/game/game.png");
    m_textureContaigner.loadAsset(textures::ID::bluecar, "graphics/game/taraco_neroder.png");
    m_textureContaigner.loadAsset(textures::ID::redcar, "graphics/game/vaug_interceptor.png");
    m_textureContaigner.loadAsset(textures::ID::whitecar, "graphics/game/retron_parsec.png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
    m_textureContaigner.loadAsset(textures::ID::loading, "graphics/intro/loading.png");
    m_textureContaigner.loadAsset(textures::ID::goldenFont, "graphics/bitmap_fonts/golden_font_32x32.png");
}



void Game::drawRacePannel()
{
    sf::Sprite sprite;
    sprite.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
    sprite.setTextureRect(sf::IntRect(m_players.getGameDifficulty() * 40, 220, 20, 20));
    sprite.setPosition(174, 14);
    m_window.draw(sprite);
    sprite.setTextureRect(sf::IntRect(2, 222, 16, 16));
    float y{31};
    for(auto elem : m_raceListState) {
        if(elem) {
            sprite.setPosition(176, y);
            m_window.draw(sprite);
        }
        y += 16;
    }
}

void Game::loadClickBoxes()
{
    loadSimpleClickBoxes();
    loadComplexClickBoxes();

}

void Game::loadSimpleClickBoxes()
{
    sf::FloatRect shape(40, 184, 165 ,30);
    mylib::SimpleClickBox clickBox(shape);
    m_simpleClickBoxes[0] = std::make_unique<mylib::SimpleClickBox>(clickBox);
    sf::FloatRect shape2(444, 162, 105 ,72);
    mylib::SimpleClickBox clickBox2(shape2);
    m_simpleClickBoxes[1] = std::make_unique<mylib::SimpleClickBox>(clickBox2);

}

void Game::loadComplexClickBoxes()
{
    mylib::JsonFile jsonfile("data/clickboxes/game.json");
    sf::Vector2f vertice;
    for(auto i = 0; i < 3; ++i) {
        mylib::ComplexClickBox box;
        m_ComplexClickBoxes[i] = std::make_unique<mylib::ComplexClickBox>(box);
        for(auto elem : jsonfile.m_Root["game"][i]) {
            vertice.x = elem.get("x", 0).asFloat();
            vertice.y = elem.get("y", 0).asFloat();
            m_ComplexClickBoxes[i]-> addVertice(vertice);
        }
    }
}

void Game::loadButtons()  //48  32
{
    float buttonY{32};
    sf::FloatRect shape(48, buttonY, 112, 14);
    float textureY{1760};
    sf::Vector2f textureCoords(0, textureY);
    for(auto i = 0; i < 9; ++i) {
        mylib::Button button(shape, m_textureContaigner.getAsset(textures::ID::spritesheet), textureCoords);
        m_racesButtonsArray[i] = std::make_unique<mylib::Button>(button);
        m_racesButtonsArray[i]->setFont("font/oneslot.ttf");
        m_racesButtonsArray[i]->setText(m_languageJson.m_Root["track"].asString() + std::to_string(i + 1));
        m_racesButtonsArray[i]->setTextColor(sf::Color::Black, sf::Color(0x557799FF));
        m_racesButtonsArray[i]->setTextSize(18);
        m_racesButtonsArray[i]->setTextSyle(sf::Text::Bold);
        m_racesButtonsArray[i]->setTextScale(sf::Vector2f(1.5f, 1.f));
        shape.top += 16;
    }
}

int Game::checkButtons()
{
    sf::Vector2i mouseCoords = sf::Mouse::getPosition(m_window);
    mouseCoords.x /= windowRatio.x;
    mouseCoords.y /= windowRatio.y;
    for(auto i = 0; i < 9; ++i) {
        if(m_racesButtonsArray[i]->isTriggerred(mouseCoords)) { return i; }
    }
    return -1;
}

unsigned int Game::race(int whatTrack, unsigned int lastRaceRanking)
{
    Race race(m_window, whatTrack, m_players, m_language);
    return race.racing(lastRaceRanking, m_completedRaces, m_clockwiseRaceRotation);
}

void Game::loading(const sf::Int32 delay, const std::string& text)
{
    Message message;
    if(!message.loadFont("graphics/bitmap_fonts/super_cars_16x16.png")) {
        std::cout << "loading texture failed." << std::endl;
    }
    message.setFontSize(16 , 16);
    message.setSize(sf::Vector2f(310, 96));
    message.setPosition(sf::Vector2f(165, 152));
    message.setColor(sf::Color(0x000099FF));
    message.setText(text);

    m_window.clear();
    m_window.setView(m_topView);
    m_window.draw(message);
    m_window.display();
    mylib::Timer duration(delay);
    duration.startTimer();
    while(!duration.isTimeElapsed())
        ;
}

bool Game::setHandCursor()
{
    if(m_handCursorImage.loadFromFile("graphics/game/cursor.png")) {
        m_handCursorImage.createMaskFromColor(sf::Color(0xFEFEFEFF));
        m_handCursor.loadFromPixels(m_handCursorImage.getPixelsPtr(), {32,32}, {0,0});
        m_window.setMouseCursor(m_handCursor);
        return true;
    }
    else { return false; }
}

void Game::initViews()
{
    m_topView.setSize(sf::Vector2f(640.f, 360.f));
    m_topView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.90f));
    m_topView.setCenter(sf::Vector2f(320, 180));
    m_bottomView.setSize(sf::Vector2f(640.f, 40.f));
    m_bottomView.setViewport(sf::FloatRect(0.f, 0.9f, 1.f, 0.1f));
    m_bottomView.setCenter(320, 20);

}
