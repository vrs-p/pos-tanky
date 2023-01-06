//
// Created by vrsp on 5. 1. 2023.
//
#include <SFML/Graphics.hpp>

#ifndef KLIENT_BUTTON_H
#define KLIENT_BUTTON_H


class Button {
public:
    Button(sf::Vector2f size, sf::Color bgColor, std::string text, int characterSize, sf::Color textColor);

    void setFont(sf::Font &font);
    void setBgColor(sf::Color color);
    void setTextColor(sf::Color color);
    void setPosition(sf::Vector2f position);
    void setClickable(bool isClickable);
    sf::RectangleShape getButton();
    sf::Text getText();
    bool isMouseOver(sf::RenderWindow &window);
    bool isClickable();

private:
    sf::Text text_;
    sf::RectangleShape button_;

    bool clickable_;
};


#endif //KLIENT_BUTTON_H