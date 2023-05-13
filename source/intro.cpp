#include "header/intro.hpp"

#include <iostream>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>

#include "header/car.hpp"
#include "header/inputs.hpp"
#include "timer/timer.hpp"
#include "header/game.hpp"
#include "music/music.hpp"
#include "random/random.hpp"
#include "header/inputs.hpp"
#include "text/bitmapfont/bitmapFont.hpp"
#include "text/bitmaptext/bitmapText.hpp"
#include "jsonfile/jsonfile.hpp"


Intro::Intro() : m_window(sf::VideoMode(640, 400), "Super Cars Revival", sf::Style::Close) //(648, 409), "Super Cars Revival", sf::Style::Close)
{
    loadAssets();
    m_bestWinnersList =  { {{"SOUT", 3, 10000}, {"ANDY", 3, 5000}, {"JEZZ", 3, 4000}, {"DAVE", 3, 3000}, {"DOUG", 3, 2000}, {"PETE", 3, 1000}} };
    m_bestLapsTimes =    { {{"SOUT", 15000} , {"ANDY", 15000}, {"JEZZ", 20000}, {"DAVE", 20000}, {"DOUG", 25000}, { "PETE", 25000}, {"GARF", 30000}, {"MARI", 30000}, {"ANNE", 30000}} };
}

void Intro::loadAssets()
{
    m_textureContaigner.loadAsset(textures::ID::loading, "graphics/intro/loading.png");
    m_textureContaigner.loadAsset(textures::ID::gremlins, "graphics/intro/gremlin.png");
    m_textureContaigner.loadAsset(textures::ID::magnetic, "graphics/intro/magnetic_fields.png");
    m_textureContaigner.loadAsset(textures::ID::super_cars, "graphics/intro/super_cars.png");
    m_textureContaigner.loadAsset(textures::ID::guyAndGirl, "graphics/intro/guy_and_girl.png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
    m_textureContaigner.loadAsset(textures::ID::ronAndNancy, "graphics/intro/ron_and_nancy.png");
    m_textureContaigner.loadAsset(textures::ID::credits_original, "graphics/intro/credits.png");
    m_textureContaigner.loadAsset(textures::ID::credits_additionnal, "graphics/intro/credits aditional.png");
    m_jsonConfig.loadJsonFile("config.json");

}

void Intro::launchIntro()   // 3mn dans la video
{

    ///////   recup la resolution pour voir si plus que 640 * 400 et mettre un scale plus grand
    ///////   mettre le scale en param des fonctions

/*    gremlins();
    magneticFields();
    mylib::Music music;
    music.loadMusic("music/intro.wav");
    music.setMusicLoop(true);
    music.setMusicVolume(100);
    music.playMusic();
    superCars();
    loading(2500);
*/    bool displayIntro{true};
    while(displayIntro)
    {
//        if(displayIntro) { displayIntro = guyAndGirl(); }
//        if(displayIntro) { loading(2500); }
        if(displayIntro) { displayIntro = ronAndNancy(); }
//        if(displayIntro) { loading(1000); }
//        if(displayIntro) { displayIntro = creditsOriginal(); }
//        if(displayIntro) { loading(1000); }
//        if(displayIntro) { displayIntro = creditsAdditionnal(); }
        loading(4000);
    }
//    music.stopMusic();
    launchGame();
}

void Intro::gremlins()
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::gremlins));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    mylib::Timer timer(3000);
    m_window.clear();
    m_window.draw(background);
    m_window.display();
    while(!timer.isTimeElapsed())
        ;
}

void Intro::magneticFields()
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::magnetic));
    background.setPosition(sf::Vector2f(320, 200));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    background.setOrigin(sf::Vector2f(320, 200));
    float factorX{10};
    float factorY{10};
    background.setScale(factorX, factorY);
    mylib::Timer timer(30);
    while(factorX >= 1.f)
    {
        m_window.clear();
        m_window.draw(background);
        while(!timer.isTimeElapsed())
        ;
        timer.restartTimer();
        factorX -= 0.2f;
        factorY -= 0.2f;
        background.setScale(factorX, factorY);
        m_window.display();
    }
    timer.setTimerDuration(9000);
    mylib::Music music;
    music.loadMusic("music/magnetic fields.wav");
    music.setMusicVolume(100);
    music.playMusic();
    while(!timer.isTimeElapsed())
        ;
}

void Intro::superCars()
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::super_cars));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    mylib::Timer timer(5000);
    m_window.clear();
    m_window.draw(background);
    m_window.display();
    while(!timer.isTimeElapsed())
        ;
}

bool Intro::guyAndGirl()
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::guyAndGirl));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    sf::Sprite foreground;
    foreground.setTexture(m_textureContaigner.getAsset(textures::ID::guyAndGirl));
    foreground.setTextureRect(sf::IntRect(640, 0, 640, 400));
    sf::Sprite car;
    car.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
    mylib::Timer timer(17000);
    bool exitIntro{false};
    mylib::Random <sf::Int32>randomTime;
    float carHight;
    mylib::Random <int>randomCarModel;
    sf::Vector2f carCoord(700, 310);
    mylib::Random <int>distanceToGo;
    mylib::Timer timerCarMovement(randomTime.randomNumber(4, 10));
    int distance{distanceToGo.randomNumber(1200, 1500)};
    int carModel;
    Inputs inputs(m_window);
    while(!timer.isTimeElapsed() && !exitIntro)                     // speech reste immobile 5s et scrolling de 1s   // commence par 1 scrolling
    {                                                               // animation bouches 5s
        if(carCoord.x >= distance  ) {

            timerCarMovement.setTimerDuration(randomTime.randomNumber(3, 6));
            distance = distanceToGo.randomNumber(1500, 2000);
            int oldCarModel{carModel};
            do
                carModel = randomCarModel.randomNumber(0, 2);
            while(carModel == oldCarModel);
            switch(carModel) {
                case 0 :
                    carHight = 83;
                    car.setTextureRect(sf::IntRect(0, 390, 253, static_cast<int>(carHight)));

                    break;
                case 1 :
                    carHight = 80;
                    car.setTextureRect(sf::IntRect(0, 480, 296, static_cast<int>(carHight)));
                    break;
                case 2 :
                    carHight = 85;
                    car.setTextureRect(sf::IntRect(0, 580, 302, static_cast<int>(carHight)));
                    break;
                default :
                    break;
            }
            carCoord.x = 0 - 400.f;
            carCoord.y =  310.f - carHight;
        }
        m_window.clear(sf::Color::Cyan);
        m_window.draw(background);
        car.setPosition(carCoord);
        m_window.draw(car);
        m_window.draw(foreground);
        m_window.display();
        if(timerCarMovement.isTimeElapsed()) {
            carCoord.x += 5;
            timerCarMovement.restartTimer();
        }
        if(inputs.readSelectionInput() == Inputs::Command::selection) {
            exitIntro = true;
        }
    }
    if(exitIntro) { return false; }
    else { return true; }
}

bool Intro::ronAndNancy()
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::ronAndNancy));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    bool exitIntro{false};
    bool speechEnded(false);
    Inputs inputs(m_window);
    mylib::BitmapText bitmapText("graphics/bitmat_fonts/ron_and_nancy_16x16.png", 16, 16);
    std::array<std::string, 21> RonAndNancySpeechArray;
    buildRonAndNancySpeech(RonAndNancySpeechArray);
    bool scroling{true};
    mylib::Timer timer(5000);
    mylib::Timer scrollingRate(50);
    float x{16};
    float watchingY{351};
    float y =  watchingY;
    int speechArrayIndex{0};
    bitmapText.setBitmapText(RonAndNancySpeechArray[speechArrayIndex] + RonAndNancySpeechArray[speechArrayIndex + 1]);
    while(!speechEnded && !exitIntro) {
        m_window.clear();                       // texte visible coord 16, 351 step 4
        if(scroling) {
            if(y > 309) {
                if(scrollingRate.isTimeElapsed()) {
                    y -= 2;
                    scrollingRate.restartTimer();
                }
                else ;
            }
            else {
                scroling = false;
                timer.restartTimer();
                speechArrayIndex++;
                if(speechArrayIndex == 20) {speechEnded = true; }
            }
        }
        else {
            y = watchingY;
            if(timer.isTimeElapsed()) {
                scroling = true;
                scrollingRate.restartTimer();
            }
        }
        bitmapText.setBitmapText(RonAndNancySpeechArray[speechArrayIndex] + RonAndNancySpeechArray[speechArrayIndex + 1]);
       // bitmapText.setBitmapText("£");
        bitmapText.drawBitmapText(x, y, 4, m_window);
        m_window.draw(background);
        m_window.display();
        if(inputs.readSelectionInput() == Inputs::Command::selection) {
            exitIntro = true;
        }
    }
    if(exitIntro) { return false; }
    else { return true; }
}

void Intro::buildRonAndNancySpeech(std::array<std::string, 21>& speechArray)
{
    mylib::JsonFile jsonRonAndNancy;
    switch(jsonRonAndNancy.m_Root.get("language", 0).asInt()) {
        case 1 :
            std::setlocale(LC_ALL, "fr_FR.UTF-8");
            jsonRonAndNancy.loadJsonFile("language/french/RonAndNancy.json");
            break;
        case 0 :
        default :
            std::setlocale(LC_ALL, "us_US.UTF-8");
            jsonRonAndNancy.loadJsonFile("language/english/RonAndNancy.json");
            break;
    }
    mylib::Random <int>speechRandimiser;
    speechArray[0] = "\n\n";
    speechArray[1] = jsonRonAndNancy.m_Root["Ron&Nancy"]["intro"][speechRandimiser.randomNumber(0, 5)].asString();
    speechArray[2] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestDrivers"][0][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[3] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestDrivers"][1][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[4] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestDrivers"][2][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[5] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestDrivers"][3][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[6] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestDrivers"][4][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[7] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestDrivers"][5][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[8] = jsonRonAndNancy.m_Root["Ron&Nancy"]["ronToNancy"][speechRandimiser.randomNumber(0, 4)].asString();
    speechArray[9] = jsonRonAndNancy.m_Root["Ron&Nancy"]["startNancy"][speechRandimiser.randomNumber(0, 4)].asString();
    speechArray[10] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestTracks"][0][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[11] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestTracks"][1][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[12] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestTracks"][2][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[13] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestTracks"][3][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[14] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestTracks"][4][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[15] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestTracks"][5][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[16] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestTracks"][6][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[17] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestTracks"][7][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[18] = jsonRonAndNancy.m_Root["Ron&Nancy"]["bestTracks"][8][speechRandimiser.randomNumber(0, 3)].asString();
    speechArray[19] = jsonRonAndNancy.m_Root["Ron&Nancy"]["outro"][speechRandimiser.randomNumber(0, 5)].asString();
    speechArray[20] = "\n\n";
    auto j{0};
    auto position{0};
    for(auto i = 2; i < 8; ++i, ++j) {
        position = speechArray[i].find("NAME");
        speechArray[i].replace(position, 4, m_bestWinnersList[j].first);
        position = speechArray[i].find("X");
        speechArray[i].replace(position, 1, std::to_string(m_bestWinnersList[j].second));
        position = speechArray[i].find("MONEY");
        speechArray[i].replace(position, 5, std::to_string(m_bestWinnersList[j].third));
    }
    j = 0;
    std::string convert;
    for(auto i = 10; i < 19; ++i, ++j) {
        position = speechArray[i].find("NAME");
        speechArray[i].replace(position, 4, m_bestLapsTimes[j].first);
        position = speechArray[i].find("TIMES");
        convert = std::to_string(static_cast<float>(m_bestLapsTimes[j].second)/1000.f);
        convert.resize(5);
        speechArray[i].replace(position, 5, convert);
    }
}

bool Intro::creditsOriginal()
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::credits_original));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    mylib::Timer timer(6000);
    m_window.clear();
    m_window.draw(background);
    m_window.display();
    bool exitIntro{false};
    Inputs inputs(m_window);
    while(!timer.isTimeElapsed() && !exitIntro)
        if(inputs.readSelectionInput() == Inputs::Command::selection) {
            exitIntro = true;
        }
    if(exitIntro) { return false; }
    else { return true; }
}

bool Intro::creditsAdditionnal()
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::credits_additionnal));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    mylib::Timer timer(6000);
    m_window.clear();
    m_window.draw(background);
    m_window.display();
    bool exitIntro{false};
    Inputs inputs(m_window);
    while(!timer.isTimeElapsed() && !exitIntro)
        if(inputs.readSelectionInput() == Inputs::Command::selection) {
            exitIntro = true;
        }
    if(exitIntro) { return false; }
    else { return true; }
}

void Intro::loading(const sf::Int32 delay)
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::loading));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    mylib::Timer timer(delay);
    m_window.clear();
    m_window.draw(background);
    m_window.display();
    while(!timer.isTimeElapsed())
        ;
}

void Intro::launchGame()
{
    Game game(m_window);
    game.launch();
}
