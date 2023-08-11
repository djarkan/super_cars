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
#include "bitmap/simpleanimation/animation.hpp"
#include "bitmap/animationPlayer/animationPlayer.hpp"
#include "header/command.hpp"

Intro::Intro() : m_window(sf::VideoMode(640, 400), "Super Cars Revival", sf::Style::Close), m_player(true), m_inputs(m_window, m_player.getJoystickID()) //(648, 409), "Super Cars Revival", sf::Style::Close)
{

    loadAssets();
    m_bestWinnersList =  { {{"SOUT", 3, 10000}, {"ANDY", 3, 5000}, {"JEZZ", 3, 4000}, {"DAVE", 3, 3000}, {"DOUG", 3, 2000}, {"PETE", 3, 1000}} };
    m_bestLapsTimes =    { {{"SOUT", 15000} , {"ANDY", 15000}, {"JEZZ", 20000}, {"DAVE", 20000}, {"DOUG", 25000}, { "PETE", 25000}, {"GARF", 30000}, {"MARI", 30000}, {"ANNE", 30000}} };
    mylib::JsonFile configJson("config.json");
    m_language = configJson.m_Root["language"].asInt();
    m_window.setMouseCursorVisible(false);

    // m_window.setSize(sf::Vector2u(1280, 800));                                                       ///////////////////// mettre au bon ratio suivant fichier de config

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
    m_textureContaigner.loadAsset(textures::ID::garageFont, "graphics/bitmap_fonts/ron_and_nancy_16x16.png");
    m_jsonConfig.loadJsonFile("config.json");

}

void Intro::launchIntro()
{
launchGame();
    gremlinsAndMagneticFields();
    mylib::Music music;
    music.loadMusic("music/intro.wav");
    music.setMusicLoop(true);
    music.setMusicVolume(100);
    music.playMusic();
    superCars();
    loading(2500);
    bool displayIntro{true};
    while(displayIntro)
    {
        if(displayIntro) { displayIntro = guyAndGirl(); }
        if(displayIntro) { loading(2500); }
        if(displayIntro) { displayIntro = ronAndNancy(); }
        if(displayIntro) { loading(1000); }
        if(displayIntro) { displayIntro = creditsOriginal(); }
        if(displayIntro) { loading(1000); }
        if(displayIntro) { displayIntro = creditsAdditionnal(); }
        loading(3000);
    }
    music.stopMusic();
    launchGame();
}

void Intro::gremlinsAndMagneticFields()
{
    mylib::Music music;
    music.loadMusic("music/magnetic fields.wav");
    music.setMusicVolume(100);
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::magnetic));
    background.setPosition(sf::Vector2f(320, 200));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    background.setOrigin(sf::Vector2f(320, 200));
    float factorX{10};
    float factorY{10};
    background.setScale(factorX, factorY);

    sf::Sprite gremlins;
    gremlins.setTexture(m_textureContaigner.getAsset(textures::ID::gremlins));
    gremlins.setTextureRect(sf::IntRect(0, 0, 640, 400));
    mylib::Timer timer(3000);
    timer.startTimer();
    m_window.clear();
    m_window.draw(gremlins);
    m_window.display();
    while(!timer.isTimeElapsed())
        ;

   timer.setTimerDuration(30);
    timer.startTimer();
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
    timer.setTimerDuration(9500);
    timer.restartTimer();
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
    timer.startTimer();
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
    timer.startTimer();
    bool exitIntro{false};
    mylib::Random <sf::Int32>randomTime;
    float carHight;
    mylib::Random <int>randomCarModel;
    sf::Vector2f carCoord(700, 310);
    mylib::Random <int>distanceToGo;
    mylib::Timer timerCarMovement(randomTime.randomNumber(4, 10));
    timerCarMovement.startTimer();
    int distance{distanceToGo.randomNumber(1200, 1500)};
    int carModel;
    Inputs inputs(m_window, m_player.getJoystickID());
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
        if(isIntroSkipped()) { exitIntro = true; }
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
    Inputs inputs(m_window, m_player.getJoystickID());
    mylib::BitmapFont font(16, 16);
    font.setTexture(m_textureContaigner.getAsset(textures::ID::garageFont));
    mylib::BitmapText bitmapText;
    bitmapText.setFont(&font);
    bitmapText.setLineSpacing(4.f);
    std::array<std::string, 21> RonAndNancySpeechArray;
    buildRonAndNancySpeech(RonAndNancySpeechArray);
    bool scroling{true};
    mylib::Timer timer(5000);
    mylib::Timer scrollingRate(47);
    scrollingRate.startTimer();
    float x{16};
    float watchingY{351};
    float y =  watchingY;
    int speechArrayIndex{0};
    bitmapText.setText(RonAndNancySpeechArray[speechArrayIndex] + RonAndNancySpeechArray[speechArrayIndex + 1]);
    mylib::JsonFile jsonAnimations("data/animations/intro.json");
    mylib::AnimationPlayer animPlayer;
    sf::Vector2f animationcoord;
    std::vector<sf::IntRect> frames;
    for(auto i = 0; i < 4; ++i) {
        frames.resize(0);
        animationcoord.x = jsonAnimations.m_Root["ronAndNancy"][i]["destination"].get("x", 100).asFloat();
        animationcoord.y = jsonAnimations.m_Root["ronAndNancy"][i]["destination"].get("y", 100).asFloat();
        bool loop = jsonAnimations.m_Root["ronAndNancy"][i].get("loop", true).asBool();
        bool randomisedLoop = jsonAnimations.m_Root["ronAndNancy"][i].get("randomisedLoop", false).asBool();
        sf::IntRect rect;
        for(auto elem : jsonAnimations.m_Root["ronAndNancy"][i]["frames"]){
            rect.left = elem.get("x", 0).asInt();
            rect.top = elem.get("y", 0).asInt();
            rect.width = elem.get("w", 0).asInt();
            rect.height = elem.get("h", 0).asInt();
            frames.push_back(rect);
        }
        mylib::Animation animation(m_textureContaigner.getAsset(textures::ID::spritesheet), animationcoord, loop, randomisedLoop, frames);
        mylib::Animation* animPtr = new mylib::Animation (animation);
        *animPtr = std::move(animation);
        animPlayer.addAnimation(animPtr);
    }
    animPlayer.m_player[0]->play();
    animPlayer.m_player[2]->play();
    mylib::Random<sf::Int32> randomiser;
    mylib::Timer ronEyesDelay(randomiser.randomNumber(1000,3000));
    ronEyesDelay.startTimer();
    mylib::Timer someoneMouthDelay(1000);
    someoneMouthDelay.startTimer();
    bool ronEyesDelayState{false};
    bool someoneSpeaking{false};
    bool ronSpeech{true};
    mylib::Timer nancyEyesDelay(randomiser.randomNumber(1000,3000));
    nancyEyesDelay.startTimer();
    bool nancyEyesDelayState{false};
    mylib::Timer speechLength(49000);
    speechLength.startTimer();
    while(!speechEnded && !exitIntro) {
        m_window.clear();                       // texte visible coord 16, 351 step 4

        for(auto elem : animPlayer.m_player) {
            elem->animate();
        }
        if(animPlayer.m_player[0]->getState() == mylib::Animation::State::stopped && ronEyesDelayState == false ) {
            ronEyesDelayState = true;
            ronEyesDelay.setTimerDuration(randomiser.randomNumber(1000,3000));
            ronEyesDelay.startTimer();
        }
        if(ronEyesDelay.isTimeElapsed()) {
            ronEyesDelayState = false;
            animPlayer.m_player[0]->play();
        }
        if(animPlayer.m_player[2]->getState() == mylib::Animation::State::stopped && nancyEyesDelayState == false ) {
            nancyEyesDelayState = true;
            nancyEyesDelay.setTimerDuration(randomiser.randomNumber(1000,3000));
            nancyEyesDelay.startTimer();
        }
        if(nancyEyesDelay.isTimeElapsed()) {
            nancyEyesDelayState = false;
            animPlayer.m_player[2]->play();
        }
        if(!someoneSpeaking && someoneMouthDelay.isTimeElapsed()) {
            if(ronSpeech) { animPlayer.m_player[1]->play(); }
            else { animPlayer.m_player[3]->play(); }
            someoneSpeaking = true;
            someoneMouthDelay.setTimerDuration(5000);
            someoneMouthDelay.restartTimer();
        }
        if(someoneSpeaking && someoneMouthDelay.isTimeElapsed()) {
            if(ronSpeech) { animPlayer.m_player[1]->stop(); }
            else { animPlayer.m_player[3]->stop(); }
            someoneSpeaking = false;
            someoneMouthDelay.setTimerDuration(1000);
            someoneMouthDelay.restartTimer();
        }
        if(!ronSpeech && speechLength.isTimeElapsed()) { exitIntro = false; }
        if(speechLength.isTimeElapsed()) {
            ronSpeech = false;
            speechLength.setTimerDuration(66000);
            speechLength.restartTimer();
        }
        if(scroling) {
            if(y > 309) {
                if(scrollingRate.isTimeElapsed()) {
                    y -= 2;
                    scrollingRate.restartTimer();
                }
                else {}
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
        bitmapText.setText(RonAndNancySpeechArray[speechArrayIndex] + RonAndNancySpeechArray[speechArrayIndex + 1]);
        bitmapText.setPosition(x, y);
        m_window.draw(bitmapText);
        m_window.draw(background);
        m_window.draw(animPlayer);
        m_window.display();
        if(isIntroSkipped()) { exitIntro = true;  }
    }
    if(!exitIntro) { return true; }
    else { return false; }
}

void Intro::buildRonAndNancySpeech(std::array<std::string, 21>& speechArray)
{
    mylib::JsonFile jsonRonAndNancy;
    switch(m_language) {
        case 0 :
            jsonRonAndNancy.loadJsonFile("language/english/RonAndNancy.json");
            break;
        case 1 :
            jsonRonAndNancy.loadJsonFile("language/usa/RonAndNancy.json");
            break;
        case 2 :
            jsonRonAndNancy.loadJsonFile("language/french/RonAndNancy.json");
            break;
        default :
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
    timer.startTimer();
    m_window.clear();
    m_window.draw(background);
    m_window.display();
    bool exitIntro{false};
    while(!timer.isTimeElapsed() && !exitIntro) {
        sf::sleep(sf::milliseconds(100));
        if(isIntroSkipped()) { exitIntro = true; }
    }
    if(!exitIntro) { return true; }
    else { return false; }
}

bool Intro::creditsAdditionnal()
{
    sf::Sprite background;
    background.setTexture(m_textureContaigner.getAsset(textures::ID::credits_additionnal));
    background.setTextureRect(sf::IntRect(0, 0, 640, 400));
    mylib::Timer timer(6000);
    timer.startTimer();
    m_window.clear();
    m_window.draw(background);
    m_window.display();
    bool exitIntro{false};
    while(!timer.isTimeElapsed() && !exitIntro) {
        sf::sleep(sf::milliseconds(100));
        if(isIntroSkipped()) { exitIntro = true; }
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
    timer.startTimer();
    m_window.clear();
    m_window.draw(background);
    m_window.display();
    while(!timer.isTimeElapsed())
        ;
}

void Intro::launchGame()
{
    Game game(m_window, m_player, m_language);
    game.launch();
}

bool Intro::isIntroSkipped()
{
    m_inputs.readInput();
    Command command;
    while(m_inputs.isCommandAvailable()) {
        command = m_inputs.getInput();
        if(command.action == CommandType::joystiskButtonPressed || command.action == CommandType::mouseLeftButtonPressed) { return true; }
    }
    return false;
}
