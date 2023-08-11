#include <header/game.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System.hpp>

#include <iostream>

#include "assetcontainer/assetcontainer.hpp"
#include "header/race.hpp"
#include "header/track.hpp"
#include "text/bitmaptext/bitmapText.hpp"
#include "text/bitmapfont/bitmapFont.hpp"
#include "timer/timer.hpp"
#include "music/music.hpp"
#include "header/garage.hpp"
#include "jsonfile/jsonfile.hpp"
#include "header/bottompanel.hpp"
#include "header/office.hpp"
#include "header/inputs.hpp"


#include <string>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

Game::Game(sf::RenderWindow& window, Player& player, unsigned int language): m_window{window}, m_pseudo{"SOUT"}, m_gameLevel{1}, m_language{language}, m_player(player), m_inputs(m_window, player.getJoystickID()), m_languageJson()
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
    m_raceListState.fill(false);
}

void Game::launch()  // json langage et si json de config reçoit les tableaux scores classement nb races/money et liste meilleurs temps/pseudo par ref non const
{
    bool focus{true};
    mylib::Music music;
    music.loadMusic("music/track1.wav");
    music.setMusicLoop(true);
    music.setMusicVolume(100);
    music.playMusic();
    m_player.setName(enterPseudo());
    m_player.setGameDifficulty();
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::game));
    background.setTextureRect(sf::IntRect(0, 0, 640, 360));
    mylib::Timer inputsTimer(50);
    inputsTimer.startTimer();
    BottomPanel bottomPanel(m_player, m_window, m_language);
    Garage garage(m_window, m_player, bottomPanel, m_language);
    Office office(m_window, m_player, bottomPanel, m_language);
    bool canGoToOffice{true};
    m_window.setMouseCursorVisible(true);
    while(m_window.isOpen()){                                    ///////////////////////////// virer la boucle d'evenement !!!!!!!!!!!!!!!!!!!!!
        if(inputsTimer.isTimeElapsed()) {
            m_inputs.readInput();
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
                        loading(2000);
                        garage.carUpdate();
                        garage.setInitGarage(false);
                        loading(2000);
                    }
                    if(m_simpleClickBoxes[1]->isPressed(m_command.coords) && canGoToOffice) {
                        canGoToOffice = false;
                       loading(2000);
                        office.buy();
                       loading(2000);
                    }
                    int whatRace = checkButtons();
                    if(whatRace >= 0 && whatRace < 9) {
                        m_window.setMouseCursorVisible(false);
                        race(whatRace);
                        m_window.setMouseCursorVisible(true);
                        garage.setInitGarage(true);
                        canGoToOffice = true;
                    }
                    for(auto i = 0; i < 3; ++i) {
                        if(m_ComplexClickBoxes[i]->isTriggerred(m_command.coords)) {
                            loading(2000);
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
                            m_window.draw(carSpecs);
                            m_window.display();
                            m_inputs.clearCommand(m_command);
                            while(m_command.action != CommandType::joystiskButtonPressed && m_command.action != CommandType::mouseLeftButtonPressed) {
                                m_inputs.readInput();
                                if(m_inputs.isCommandAvailable()) { m_command = m_inputs.getInput(); }
                            }
                            loading(2000);
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
            if(focus) {
                m_window.clear();
                m_window.draw(background);
                drawRacePannel();
                for(auto i = 0; i < 9; ++i) { m_window.draw(*m_races[i]); }
                bottomPanel.updateUsury();
                m_window.draw(bottomPanel);
                m_window.display();
            }
        }
    }
}

void Game::loadTextures()
{
    m_textureContaigner.loadAsset(textures::ID::game, "graphics/game/game.png");
    m_textureContaigner.loadAsset(textures::ID::bluecar, "graphics/game/taraco_neroder.png");
    m_textureContaigner.loadAsset(textures::ID::redcar, "graphics/game/vaug_interceptor.png");
    m_textureContaigner.loadAsset(textures::ID::whitecar, "graphics/game/retron_parsec.png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
    m_textureContaigner.loadAsset(textures::ID::pseudo, "graphics/intro/enter_name.png");
    m_textureContaigner.loadAsset(textures::ID::loading, "graphics/intro/loading.png");
    m_textureContaigner.loadAsset(textures::ID::goldenFont, "graphics/bitmap_fonts/golden_font_32x32.png");
}

std::string Game::enterPseudo()
{
    std::string pseudo = "RICH";
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::pseudo));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    sf::Sprite clearedGlyph;
    clearedGlyph.setTexture(m_textureContaigner.getAsset(textures::ID::pseudo));
    clearedGlyph.setTextureRect(sf::IntRect(250, 335, 32, 32));
    mylib::BitmapFont font(32, 32);
    font.setTexture(m_textureContaigner.getAsset(textures::ID::goldenFont));
    mylib::BitmapText bitmapText;
    bitmapText.setFont(&font);
    mylib::Timer flashingLetter(100);
    unsigned int currentLetter{0};
    flashingLetter.startTimer();
    bool clearGlyph{false};
    mylib::Timer commandTimer(200);
    commandTimer.startTimer();
    while(m_command.action != CommandType::joystiskButtonPressed && m_command.action != CommandType::mouseLeftButtonPressed) {
        if(commandTimer.isTimeElapsed()) {
            m_inputs.readInput();
            commandTimer.restartTimer();
        }
        if(m_inputs.isCommandAvailable()) { m_command = m_inputs.getInput(); }
        m_window.clear();
        m_window.draw(background);
        bitmapText.setText(m_languageJson.m_Root["pseudo"][0].asString());
        sf::FloatRect bounds = bitmapText.getLocalBounds();
        bitmapText.setPosition(320.f - (bounds.width / 2.f), 242.f);
        m_window.draw(bitmapText);
        bitmapText.setText(m_languageJson.m_Root["pseudo"][1].asString());
        bounds = bitmapText.getLocalBounds();
        bitmapText.setPosition(320.f - (bounds.width / 2.f), 278.f);
        m_window.draw(bitmapText);
        bitmapText.setText(pseudo);
        bitmapText.setPosition(257, 335);
        m_window.draw(bitmapText);
        if(m_command.action == CommandType::joystiskMoved) {
            if(m_command.offsetX > 75) {
                currentLetter++;
                if(currentLetter > 3) { currentLetter = 0; }
            }
            if(m_command.offsetX < -75) {
                if(currentLetter == 0) { currentLetter = 3; }
                else { currentLetter--; }
            }
            if(m_command.offsetY < -75) {
                if(pseudo[currentLetter] == 'Z') { pseudo[currentLetter] = 'A';}
                else { pseudo[currentLetter] = static_cast<char>(pseudo[currentLetter] + 1); }
                bitmapText.setText(pseudo);
            }
            if(m_command.offsetY > 75) {
                if(pseudo[currentLetter] == 'A') { pseudo[currentLetter] = 'Z';}
                else { pseudo[currentLetter] = static_cast<char>(pseudo[currentLetter] - 1); }
                bitmapText.setText(pseudo);
            }
        m_inputs.clearCommand(m_command);
        }
        if(clearGlyph) {
            clearedGlyph.setPosition(257.f + (static_cast<float>(currentLetter) * 32.f), 335);
            m_window.draw(clearedGlyph);
        }
        if(flashingLetter.isTimeElapsed()) {
            if(clearGlyph) { clearGlyph = false; }
            else { clearGlyph = true; }
            flashingLetter.restartTimer();
        }
        m_window.display();
    }
    while(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {}
    return pseudo;
}

void Game::drawRacePannel()
{
    sf::Sprite sprite;
    sprite.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
    sprite.setTextureRect(sf::IntRect(m_gameLevel * 40, 220, 20, 20));
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
        m_races[i] = std::make_unique<mylib::Button>(button);
        m_races[i]->setFont("font/oneslot.ttf");
        m_races[i]->setText(m_languageJson.m_Root["track"].asString() + std::to_string(i + 1));
        m_races[i]->setTextColor(sf::Color::Black, sf::Color(0x557799FF));
        m_races[i]->setTextSize(18);
        m_races[i]->setTextSyle(sf::Text::Bold);
        m_races[i]->setTextScale(sf::Vector2f(1.5f, 1.f));
        shape.top += 16;
    }
}

int Game::checkButtons()
{
    for(auto i = 0; i < 9; ++i) {
        if(m_races[i]->isTriggerred(sf::Mouse::getPosition(m_window))) { return i; }
    }
    return -1;
}

void Game::race(int whatRace)
{
    switch (whatRace) {
        case 0 :
                std::cout << "race n° 1" << std::endl;
                break;
        case 1 :
                std::cout << "race n° 2" << std::endl;
                break;
        case 2 :
                std::cout << "race n° 3" << std::endl;
                break;
        case 3 :
                std::cout << "race n° 4" << std::endl;
                break;
        case 4:
                std::cout << "race n° 5" << std::endl;
                break;
        case 5 :
                std::cout << "race n° 6" << std::endl;
                break;
        case 6 :
                std::cout << "race n° 7" << std::endl;
                break;
        case 7:
                std::cout << "race n° 8" << std::endl;
                break;
        case 8 :
                std::cout << "race n° 9" << std::endl;
                break;
        default :
                break;

    }
}

void Game::loading(const sf::Int32 delay)
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::loading));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    mylib::Timer timer(delay);
    timer.startTimer();
    m_window.clear();
    m_window.draw(background);
    m_window.display();
    while(!timer.isTimeElapsed())
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
