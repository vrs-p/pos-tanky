//
// Created by vrsp on 5. 1. 2023.
//

#include "Textbox.h"

Textbox::Textbox(int size, sf::Color color, bool isSelected) {
    this->textBox_.setCharacterSize(size);
    this->textBox_.setFillColor(color);
    this->isSelected_ = isSelected;
    if (isSelected) {
        this->textBox_.setString("_");
    } else {
        this->textBox_.setString("");
    }
}

void Textbox::inputLogic(int charTyped) {
    if (charTyped != BACKSPACE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
        this->text_ << static_cast<char>(charTyped);
    } else if (charTyped == BACKSPACE_KEY) {
        if (this->text_.str().length() > 0) {
            this->deleteLastCharacter();
        }
    }
    this->textBox_.setString(this->text_.str() + "_");
}

void Textbox::deleteLastCharacter() {
    std::string oldText = this->text_.str();
    std::string newText = "";
    for (int i = 0; i < oldText.length() - 1; ++i) {
        newText += oldText[i];
    }
    this->text_.str("");
    this->text_ << newText;

    this->textBox_.setString(this->text_.str());
}

void Textbox::setFont(sf::Font &font) {
    this->textBox_.setFont(font);
}

void Textbox::setPosition(sf::Vector2f position) {
    this->textBox_.setPosition(position);
}

void Textbox::setLimit(bool hasLimit) {
    this->hasLimit_ = hasLimit;
}

void Textbox::setLimit(int limit) {
    this->hasLimit_ = true;
    this->limit_ = limit - 1;
}

void Textbox::setSelected(bool isSelected) {
    this->isSelected_ = isSelected;
    if (!isSelected) {
        std::string oldText = this->text_.str();
        std::string newText = "";
        for (int i = 0; i < oldText.length(); ++i) {
            newText += oldText[i];
        }
        this->textBox_.setString(newText);
    } else {
        this->textBox_.setString(this->text_.str() + "_");
    }
}

std::string Textbox::getText() {
    return this->text_.str();
}

sf::Text Textbox::getTextBox() {
    return this->textBox_;
}

bool Textbox::isSelected() {
    return this->isSelected_;
}

void Textbox::typed(sf::Event event) {
    if (this->isSelected_) {
        int charTyped = event.text.unicode;
        if (charTyped < 128) {
            if (this->hasLimit_) {
                if (this->text_.str().length() <= this->limit_) {
                    this->inputLogic(charTyped);
                } else if (this->text_.str().length() > this->limit_ && charTyped == BACKSPACE_KEY) {
                    this->deleteLastCharacter();
                }
            } else {
                this->inputLogic(charTyped);
            }
        }
    }
}
