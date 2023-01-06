//
// Created by vrsp on 5. 1. 2023.
//

#include "ShowScore.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

ShowScore::ShowScore(int playerScore, std::vector<Tank*>* otherTanks) {
    this->font_.loadFromFile("../font/consola.ttf");

    textGame_.setFont(font_);
    textGame_.setCharacterSize(32);
    textGame_.setFillColor(sf::Color::White);
    this->textGame_.setString("POS-Tanks");
    this->textGame_.setPosition(sf::Vector2f((SCREEN_WIDTH - this->textGame_.getLocalBounds().width) / 2, SCREEN_HEIGHT / 2 - this->textGame_.getLocalBounds().height * 11));

    this->textEndGame_.setFont(font_);
    this->textEndGame_.setCharacterSize(24);
    this->textEndGame_.setFillColor(sf::Color::White);
    this->textEndGame_.setString("Game Over");
    this->textEndGame_.setPosition(sf::Vector2f((SCREEN_WIDTH - this->textEndGame_.getLocalBounds().width) / 2, SCREEN_HEIGHT / 2 - this->textGame_.getLocalBounds().height * 9));

    this->textYourScore_.setFont(font_);
    this->textYourScore_.setCharacterSize(32);
    this->textYourScore_.setFillColor(sf::Color::White);
    this->textYourScore_.setString("Your score is: " + std::to_string(playerScore));
    this->textYourScore_.setPosition(sf::Vector2f((SCREEN_WIDTH - this->textYourScore_.getLocalBounds().width) / 2, SCREEN_HEIGHT / 2));

    this->textOthersScore_.setFont(font_);
    this->textOthersScore_.setCharacterSize(32);
    this->textOthersScore_.setFillColor(sf::Color::White);
    std::string stringOthersScore;
    for (Tank* tank: *otherTanks) {
        this->stringOthersScore_.append("Score of player " + tank->getPlayerName() + " is: " + std::to_string(tank->getScore()) + "\n");
    }
    this->textOthersScore_.setString(this->stringOthersScore_);
    this->textOthersScore_.setPosition(sf::Vector2f((SCREEN_WIDTH - this->textOthersScore_.getLocalBounds().width) / 2, SCREEN_HEIGHT / 2 + this->textYourScore_.getLocalBounds().height * 2));

    this->showScore_ = true;
}

ShowScore::~ShowScore() {
    delete this->window_;
    this->window_ = nullptr;
}

void ShowScore::initializeWindow() {
    this->window_ = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "POS-Tanks", sf::Style::Close);
    this->window_->setFramerateLimit(60);
    this->window_->setActive(true);
}

void ShowScore::render() {
    this->initializeWindow();

    this->window_->clear();
    this->window_->draw(this->textGame_);
    this->window_->draw(this->textEndGame_);
    this->window_->draw(this->textYourScore_);
    this->window_->draw(this->textOthersScore_);
    this->window_->display();

    while (this->showScore_) {
        sf::Event event;
        while (this->window_->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->showScore_ = false;
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Escape) {
                    this->showScore_ = false;
                }
            }
        }
    }

    this->window_->setActive(false);
    this->window_->close();
}
