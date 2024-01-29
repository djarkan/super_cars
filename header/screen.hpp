#ifndef    __SCREEN_HPP__
#define    __SCREEN_HPP__

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

class screen {
    public:
                screen(sf::RenderWindow& window);

    private:
                sf::RenderWindow m_window;
                sf::View playerView;


};
#endif
