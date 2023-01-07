//
// Created by vrsp on 5. 1. 2023.
//
#include "Button.h"

/**
 * Inspired by this tutorial
 * https://www.youtube.com/watch?v=T31MoLJws4U
 * @param size
 * @param bgColor
 * @param text
 * @param characterSize
 * @param textColor
 */
Button::Button(sf::Vector2f size, sf::Color bgColor, std::string text, int characterSize, sf::Color textColor) {
    this->text_.setString(text);
    this->text_.setFillColor(textColor);
    this->text_.setCharacterSize(characterSize);

    this->button_.setSize(size);
    this->button_.setFillColor(bgColor);

    this->clickable_ = false;
}

void Button::setFont(sf::Font &font) {
    this->text_.setFont(font);
}

void Button::setBgColor(sf::Color color) {
    this->button_.setFillColor(color);
}

void Button::setTextColor(sf::Color color) {
    this->text_.setFillColor(color);
}

void Button::setPosition(sf::Vector2f position) {
    this->button_.setPosition(position);

    float xPos = (position.x + this->button_.getLocalBounds().width / 2) - this->text_.getLocalBounds().width / 2;
    float yPos = (position.y + this->button_.getLocalBounds().height / 2) - this->text_.getLocalBounds().height * 2 / 3;

    this->text_.setPosition(xPos, yPos);
}

void Button::setClickable(bool isClickable) {
    this->clickable_ = isClickable;
}

sf::RectangleShape Button::getButton() {
    return this->button_;
}

sf::Text Button::getText() {
    return this->text_;
}

bool Button::isMouseOver(sf::RenderWindow &window) {
    float mouseX = sf::Mouse::getPosition(window).x;
    float mouseY = sf::Mouse::getPosition(window).y;

    float btnX = this->button_.getPosition().x;
    float btnY = this->button_.getPosition().y;
    float btnSizeX = this->button_.getSize().x;
    float btnSizeY = this->button_.getSize().y;

    if (mouseX >= btnX && mouseX <= btnX + btnSizeX && mouseY >= btnY && mouseY <= btnY + btnSizeY) {
        return true;
    }

    return false;
}

bool Button::isClickable() {
    return this->clickable_;
}