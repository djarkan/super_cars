#include "header/message.hpp"

Message::Message()
{

}

Message::Message(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text)
{
    m_retctangle.setSize(size);
    m_retctangle.setPosition(position);
    setText(text);
}

void Message::setText(const std::string& text)
{
    m_text.setText(text);
    sf::FloatRect bounds{m_text.getLocalBounds()};
    sf::Vector2f rectSize{m_retctangle.getSize()};
    sf::Vector2f position{m_retctangle.getPosition()};
    m_text.setPosition(sf::Vector2f(position .x + ((rectSize.x - bounds.width) / 2), position .y + ((rectSize.y - bounds.height) / 2)));
}

void Message::setPosition(const sf::Vector2f& position)
{
    m_retctangle.setPosition(position);
}

void Message::setFont(mylib::BitmapFont& font)
{
     m_text.setFont(&font);
}

bool Message::loadFont(const std::string& name)
{
     bool success{m_font.loadTexture(name)};
     m_text.setFont(&m_font);
     return success;
}

void Message::setFontSize(unsigned int glyphWidth, unsigned int glyphHigth)
{
    m_font.setSize(glyphWidth, glyphHigth);
}

void Message::setColor(const sf::Color& color)
{
    m_retctangle.setFillColor(color);
}

void Message::setOutlineColor(const sf::Color& color)
{
    m_retctangle.setOutlineColor(color);
}

void Message::setTextColor(const sf::Color& color)
{
    m_text.setColor(color);
}

void Message::setSize(const sf::Vector2f& size)
{
    m_retctangle.setSize(size);
}

void Message::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_retctangle, states);
 //   states.texture = m_font.getTexture();
    target.draw(m_text, states);
}
