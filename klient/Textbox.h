//
// Created by vrsp on 5. 1. 2023.
//
#include <SFML/Graphics.hpp>
#include <sstream>

#ifndef KLIENT_TEXTBOX_H
#define KLIENT_TEXTBOX_H
#define BACKSPACE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27


class Textbox {
public:
    Textbox(int size, sf::Color color, bool isSelected);

    void setFont(sf::Font &font);
    void setPosition(sf::Vector2f position);
    void setLimit(bool hasLimit);
    void setLimit(int limit);
    void setSelected(bool isSelected);
    std::string getText();
    sf::Text getTextBox();
    bool isSelected();
    void typed(sf::Event event);
    void setInitialText(std::string text);

private:
    sf::Text textBox_;
    std::ostringstream text_;
    bool isSelected_;
    bool hasLimit_;
    int limit_;

    void inputLogic(int charTyped);
    void deleteLastCharacter();
};


#endif //KLIENT_TEXTBOX_H
