#include "header/garage.hpp"
#include "jsonfile/jsonfile.hpp"
#include "header/inputs.hpp"
#include "timer/timer.hpp"
#include "random/random.hpp"
#include "bitmap/simpleAnimation/animation.hpp"

#include <sfml/window/mouse.hpp>

#include <iostream>
Garage::Garage( sf::RenderWindow& window, Player& player, BottomPanel& bottomPanel, const unsigned int language) :
                                                                m_window{window}, m_language{language}, m_bottomPanel{bottomPanel},
                                                                m_switch{true}, m_inGarage{true}, m_coordsJson("data/garage/coords.json"),
                                                                m_initGrage{true}, m_font(14, 10), m_text(), m_player(player), m_inputs(m_window, player.getJoystickID())
{
    loadClickBoxes();
    loadTextures();
    m_OptionsBought.fill(false);
    switch(m_language) {
        case 0 :
            m_languageJson.loadJsonFile("language/english/garage.json");
            break;
        case 1:
            m_languageJson.loadJsonFile("language/usa/garage.json");
            break;
        case 2 :
            m_languageJson.loadJsonFile("language/french/garage.json");
            break;
        default :
            break;
    }
    m_font.setTexture(m_textureContaigner.getAsset(textures::ID::garageFont));
    m_font.setGlyphWidth('I', 9);
    m_font.setGlyphWidth('J', 10);
    m_text.setFont(&m_font);
    m_text.setText(m_languageJson.m_Root["garage"]["text"].asString());
    m_text.setLetterSpacing(2.f);
    m_garageSoundBuffer.loadFromFile("sound/cashdrawer.ogg");
    m_cashDrawerSound.setBuffer(m_garageSoundBuffer);
    m_soundbuffer.loadFromFile("C:/Users/Djarkan/Documents/C++/codeblocks/Super_Cars_Revival/sound/whatareyoudoing.ogg");
    m_whatryoudoing.setBuffer(m_soundbuffer);
}

void Garage::loadClickBoxes()
{
    mylib::JsonFile boxesData("data/clickboxes/garage.json");
    for(auto i = 0; i < 19; ++i) {
        sf::FloatRect clickbox( boxesData.m_Root["garage"]["boxes"][i].get("x", 25).asFloat(),
                                boxesData.m_Root["garage"]["boxes"][i].get("y", 25).asFloat(),
                                boxesData.m_Root["garage"]["boxes"][i].get("w", 25).asFloat(),
                                boxesData.m_Root["garage"]["boxes"][i].get("h", 25).asFloat());
        m_clickBoxes[i] = std::make_unique<mylib::SimpleClickBox>(clickbox);
    }
}

int Garage::checkClickboxes()
{
    for(auto i = 0; i < 19; ++i) {
        if(m_clickBoxes[i]->isPressed(m_command.coords)) { return i; }
    }
    return -1;
}

void Garage::loadTextures()
{
    m_textureContaigner.loadAsset(textures::ID::garage, "graphics/garage/garage.png");
    m_textureContaigner.loadAsset(textures::ID::darkgarage, "graphics/garage/garage_light_off.png");
    m_textureContaigner.loadAsset(textures::ID::spritesheet, "graphics/sprite_sheet/sprite_sheet.png");
    m_textureContaigner.loadAsset(textures::ID::garageFont, "graphics/bitmap_fonts/garage_14x10.png");
}

void Garage::setBackground()
{
    m_background.setTexture(m_textureContaigner.getAsset(textures::ID::garage));
    m_background.setTextureRect(sf::IntRect(0, 0, 640, 360));
    m_background.setPosition(0, 0);

    m_smiling.setTexture(m_textureContaigner.getAsset(textures::ID::spritesheet));
    m_smiling.setTextureRect(sf::IntRect(101, 1080, 58, 83));
    m_smiling.setPosition(64, 91);
}

void Garage::setInitGarage(bool init)
{
    m_initGrage = init;
}

void Garage::carUpdate()  // sold texture 180 980   90 * 17
{
    int choice{-1};
    setBackground();
    mylib::Timer smilingTimer(2000);
    smilingTimer.restartTimer();
    mylib::Random<sf::Int32> randomiser;
    sf::Vector2f animationcoord;
    std::vector<sf::IntRect> frames;
    mylib::JsonFile jsonAnimations("data/animations/garage.json");
    animationcoord.x = jsonAnimations.m_Root["garage"]["destination"].get("x", 100).asFloat();
    animationcoord.y = jsonAnimations.m_Root["garage"]["destination"].get("y", 100).asFloat();
    bool loop = jsonAnimations.m_Root["garage"].get("loop", true).asBool();
    bool randomisedLoop = jsonAnimations.m_Root["garage"].get("randomisedLoop", false).asBool();
    sf::IntRect rect;
    for(auto elem : jsonAnimations.m_Root["garage"]["frames"]){
        rect.left = elem.get("x", 0).asInt();
        rect.top = elem.get("y", 0).asInt();
        rect.width = elem.get("w", 0).asInt();
        rect.height = elem.get("h", 0).asInt();
        frames.push_back(rect);
    }
    mylib::Animation animation(m_textureContaigner.getAsset(textures::ID::spritesheet), animationcoord, loop, randomisedLoop, frames);
    mylib::Timer eyesDelay;
    bool eyesDelayState{false};
    bool canDrawAnimation{false};
    if(m_initGrage) { definePrices(); }
    mylib::Timer inputsTimer(50);
    inputsTimer.startTimer();
    bool focus{true};
    while(m_inGarage) {
        if(inputsTimer.isTimeElapsed()) {
            m_inputs.readInput();
            inputsTimer.restartTimer();
        }
        if(m_inputs.isCommandAvailable()) {
            m_command = m_inputs.getInput();
            switch (m_command.action) {
                case CommandType::mouseLeftButtonPressed :
                case CommandType::joystiskButtonPressed :
                    choice = checkClickboxes();
                    if(choice >= 0) { action(choice, m_player); }
                    break;
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
        }
        if(focus) {
            m_window.clear();
            if(m_switch) { m_window.draw(m_background); }
            m_window.draw(m_bottomPanel);
            drawPrices();
            drawlabels();
            if(canDrawAnimation) { m_window.draw(animation); }
            if(!m_switch) { m_window.draw(m_background); }
            m_window.display();
            if(smilingTimer.isTimeElapsed()) {
                m_smiling.setPosition(-100, -100);
                canDrawAnimation = true;
            }
            m_window.draw(m_smiling);
            if(animation.getState() == mylib::Animation::State::stopped && eyesDelayState == false ) {
                eyesDelayState = true;
                eyesDelay.setTimerDuration(randomiser.randomNumber(1000,3000));
                eyesDelay.startTimer();
            }
            if(eyesDelay.isTimeElapsed()) {
                eyesDelayState = false;
                animation.play();
            }
            animation.animate();
        }
    }
    m_inGarage = true;
}

void Garage::action(int choice, Player& player)
{
    unsigned int playerCash{player.getMoney()};

    switch(choice) {
        case 0 :
        case 1 :
            m_inGarage = false;
            break;
        case 2 :
            if(choice == 2) {
                if(m_switch) {
                    m_background.setTexture(m_textureContaigner.getAsset(textures::ID::darkgarage));
                    m_switch = false;
                    m_whatryoudoing.play();
                }
                else {
                    m_background.setTexture(m_textureContaigner.getAsset(textures::ID::garage));
                    m_switch = true;
                }
            }
            break;
        case 3 :
            if(playerCash >= m_prices[0] && !m_OptionsBought[0]) {
                player.setCarPowerSteeringKit(true);
                m_OptionsBought[0] = true;
                player.setMoney(playerCash - m_prices[0]);
                m_prices[0] = 0;
                m_cashDrawerSound.play();
            }
            break;
        case 4 :
            if(playerCash >= m_prices[1] && !m_OptionsBought[1]) {
                player.setCarTurboChargerKit(true);
                m_OptionsBought[1] = true;
                player.setMoney(playerCash - m_prices[1]);
                m_prices[1] = 0;
                m_cashDrawerSound.play();
            }
            break;
        case 5 :
            if(playerCash >= m_prices[2] && !m_OptionsBought[2]) {
                player.setCarHighSpeedKit(true);
                m_OptionsBought[2] = true;
                player.setMoney(playerCash - m_prices[2]);
                m_prices[2] = 0;
                m_cashDrawerSound.play();
            }
            break;
        case 6 :
            if(playerCash >= m_prices[3] && !m_OptionsBought[3]) {
                player.setCarRetroKit(true);
                m_OptionsBought[3] = true;
                player.setMoney(playerCash - m_prices[3]);
                m_prices[3] = 0;
                m_cashDrawerSound.play();
            }
            break;
        case 7 :
            if(playerCash >= m_prices[4] && !m_OptionsBought[4]) {
                player.setCarSpinAssistKit(true);
                m_OptionsBought[4] = true;
                player.setMoney(playerCash - m_prices[4]);
                m_prices[4] = 0;
                m_cashDrawerSound.play();
            }
            break;
        case 8 :
            if(playerCash >= m_prices[5] && !m_OptionsBought[5]) {
                player.setCarFrontMissile(true);
                m_OptionsBought[5] = true;
                player.setMoney(playerCash - m_prices[5]);
                m_prices[5] = 0;
                m_cashDrawerSound.play();
            }
            break;
        case 9 :
            if(playerCash >= m_prices[6] && !m_OptionsBought[6]) {
                player.setCarRearMissile(true);
                m_OptionsBought[6] = true;
                player.setMoney(playerCash - m_prices[6]);
                m_prices[6] = 0;
                m_cashDrawerSound.play();
            }
            break;
        case 10 :
            if(playerCash >= m_prices[7] && !m_OptionsBought[7]) {
                player.setCarSideArmourKit(true);
                m_OptionsBought[7] = true;
                player.setMoney(playerCash - m_prices[7]);
                m_prices[7] = 0;
                m_cashDrawerSound.play();
            }
            break;
        case 11 :
        case 15 :
            if(playerCash >= m_prices[8] && m_prices[8] > 0) {
               playerCash -= m_prices[8];
               player.setCarEngineState(1);
               player.setMoney(playerCash);
               m_prices[8] = 0;
            }
            else {
                if(playerCash > 0 && m_prices[8] > 0) {
                    float percentRepairPossible = playerCash / m_prices[8];
                    float state = player.getCarEngineState();
                    player.setCarEngineState(1 - ((1  - state) * (1 - percentRepairPossible)));
                    player.setMoney(0);
                    m_cashDrawerSound.play();
                }
                else { break; }
            }
            m_cashDrawerSound.play();
            m_bottomPanel.updateUsury();
            break;
        case 12 :
        case 16 :
            if(playerCash >= m_prices[9] && m_prices[9] > 0) {
               playerCash -= m_prices[9];
               player.setCarBodyState(1);
               player.setMoney(playerCash);
               m_prices[9] = 0;
            }
            else {
                if(playerCash > 0 && m_prices[9] > 0) {
                    float percentRepairPossible = playerCash / m_prices[9];
                    float state = player.getCarBodyState();
                    player.setCarBodyState(1 - ((1  - state) * (1 - percentRepairPossible)));
                    player.setMoney(0);
                }
                else { break; }
            }
            m_cashDrawerSound.play();
            m_bottomPanel.updateUsury();
            break;
        case 13 :
        case 17 :
            if(playerCash >= m_prices[10] && m_prices[10] > 0) {
               playerCash -= m_prices[10];
               player.setCarTyresState(1);
               player.setMoney(playerCash);
               m_prices[10] = 0;
            }
            else {
                if(playerCash > 0 && m_prices[10] > 0) {
                    float percentRepairPossible = playerCash / m_prices[10];
                    float state = player.getCarTyresState();
                    player.setCarTyresState(1 - ((1  - state) * (1 - percentRepairPossible)));
                    player.setMoney(0);
                }
                else { break; }
            }
            m_cashDrawerSound.play();
            m_bottomPanel.updateUsury();
            break;
        case 14 :
        case 18 :
            if(playerCash >= m_prices[11] && m_prices[11] > 0) {
               playerCash -= m_prices[11];
               player.setCarFuelState(1);
               player.setMoney(playerCash);
               m_prices[11] = 0;
            }
            else {
                if(playerCash > 0 && m_prices[11] > 0) {
                    float percentRepairPossible = playerCash / m_prices[11];
                    float state = player.getCarFuelState();
                    player.setCarFuelState(1 - ((1  - state) * (1 - percentRepairPossible)));
                    player.setMoney(0);
                }
                else { break; }
            }
            m_cashDrawerSound.play();
            m_bottomPanel.updateUsury();
            break;
        default :
            break;
    }
}

void Garage::definePrices()
{
    mylib::Random<int> randomiser;
    m_prices[0] = 2000 + randomiser.randomNumber(0,19) * 50;
    m_prices[1] = 4000 + randomiser.randomNumber(0,19) * 50;
    m_prices[2] = 4000 + randomiser.randomNumber(0,19) * 50;
    m_prices[3] = 1000 + randomiser.randomNumber(0,19) * 50;
    m_prices[4] = 1000 + randomiser.randomNumber(0,19) * 50;
    m_prices[5] = 4000 + randomiser.randomNumber(0,19) * 50;
    m_prices[6] = 3000 + randomiser.randomNumber(0,19) * 50;
    m_prices[7] = 3000 + randomiser.randomNumber(0,19) * 50;
    if(m_player.getCarEngineState() < 1) { m_prices[8] = 4000 + randomiser.randomNumber(0,19) * 50; }
    if(m_player.getCarBodyState() < 1) { m_prices[9] = 4000 + randomiser.randomNumber(0,19) * 50; }
    if(m_player.getCarTyresState() < 1) { m_prices[10] = 2000 + randomiser.randomNumber(0,19) * 50; }
    if(m_player.getCarFuelState() < 1) { m_prices[11] = 1000 + randomiser.randomNumber(0,19) * 50; }
}

void Garage::drawPrices()
{
    for(auto i = 0; i < 12; ++i) {
        if(m_prices[i] == 0) {
            if(m_language < 2) { m_text.setText(m_languageJson.m_Root["garage"]["currency"].asString() + m_languageJson.m_Root["garage"]["sold"].asString()); }
            else { m_text.setText(m_languageJson.m_Root["garage"]["sold"].asString()); }
        }
        else {
            if(m_language < 2) { m_text.setText(m_languageJson.m_Root["garage"]["currency"].asString() + std::to_string(m_prices[i])); }
            else { m_text.setText(std::to_string(m_prices[i]) + m_languageJson.m_Root["garage"]["currency"].asString()); }
        }
        m_text.setPosition(m_coordsJson.m_Root["prices"][i]["x"].asInt(), m_coordsJson.m_Root["prices"][i]["y"].asInt());
        m_window.draw(m_text);
    }
}

void Garage::drawlabels()
{
    m_text.setText(m_languageJson.m_Root["garage"]["title"].asString());
    sf::FloatRect bounds{m_text.getLocalBounds()};
    m_text.setPosition(226 - (bounds.width /2), 294);
    m_window.draw(m_text);
    m_text.setText(m_languageJson.m_Root["garage"]["motor"].asString());
    m_text.setPosition(24, 310);
    m_window.draw(m_text);
    m_text.setText(m_languageJson.m_Root["garage"]["tyres"].asString());
    m_text.setPosition(24, 326);
    m_window.draw(m_text);
    m_text.setText(m_languageJson.m_Root["garage"]["body"].asString());
    m_text.setPosition(232, 310);
    m_window.draw(m_text);
    m_text.setText(m_languageJson.m_Root["garage"]["fuel"].asString());
    m_text.setPosition(232, 326);
    m_window.draw(m_text);
}
