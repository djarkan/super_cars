#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <string>

#include "text/bitmapfont/bitmapFont.hpp"
#include "text/bitmaptext/bitmapText.hpp"

class Message : public sf::Drawable {
    public :
                                            Message();
                                            Message(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text);
        bool                                loadFont(const std::string& name);
        void                                setText(const std::string& text);
        void                                setPosition(const sf::Vector2f& position);
        void                                setFont(mylib::BitmapFont& font);
        void                                setFontSize(unsigned int glyphWidth, unsigned int glyphHigth);
        void                                setColor(const sf::Color& color);
        void                                setOutlineColor(const sf::Color& color);
        void                                setTextColor(const sf::Color& color);
        void                                setSize(const sf::Vector2f& size);

    private :
        sf::RectangleShape                  m_retctangle;
        mylib::BitmapFont                   m_font;
        mylib::BitmapText                   m_text;

        virtual void                        draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // MESSAGE_HPP
