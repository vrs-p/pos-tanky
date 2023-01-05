//
// Created by filip on 5. 1. 2023.
//
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
#include "Menu.h"

Menu::Menu() {
    float xPosition = 50.0;
    float xSpace = 100.0;
    this->gameStarted_ = false;
    this->font_.loadFromFile("../font/consola.ttf");

    this->textMainMenu_.setFont(this->font_);
    this->textMainMenu_.setCharacterSize(32);
    this->textMainMenu_.setFillColor(sf::Color::White);
    this->textMainMenu_.setString("POS-Tanks");
    this->textMainMenu_.setPosition(sf::Vector2f((SCREEN_WIDTH - this->textMainMenu_.getLocalBounds().width) / 2, SCREEN_HEIGHT / 2 - this->textMainMenu_.getLocalBounds().height * 15));

    this->name_.setFont(this->font_);
    this->name_.setCharacterSize(32);
    this->name_.setFillColor(sf::Color::White);
    this->name_.setString("Enter name:");
    this->name_.setPosition(xPosition, SCREEN_HEIGHT / 2 - this->name_.getLocalBounds().height * 9);

    this->nameInput_.setFont(this->font_);
    this->nameInput_.setCharacterSize(32);
    this->nameInput_.setFillColor(sf::Color::White);
    this->nameInput_.setString("LogiMou");
    this->nameInput_.setPosition(this->name_.getLocalBounds().width + xSpace, SCREEN_HEIGHT / 2 - this->name_.getLocalBounds().height * 9);


    this->ipAddressText_.setFont(this->font_);
    this->ipAddressText_.setCharacterSize(32);
    this->ipAddressText_.setFillColor(sf::Color::White);
    this->ipAddressText_.setString("Enter IP address:");
    this->ipAddressText_.setPosition(xPosition, SCREEN_HEIGHT / 2 - this->ipAddressText_.getLocalBounds().height * 7);

    this->ipAddressTextInput_.setFont(this->font_);
    this->ipAddressTextInput_.setCharacterSize(32);
    this->ipAddressTextInput_.setFillColor(sf::Color::White);
    this->ipAddressTextInput_.setString("158.193.128.160");
    this->ipAddressTextInput_.setPosition(this->ipAddressText_.getLocalBounds().width + xSpace, SCREEN_HEIGHT / 2 - this->ipAddressText_.getLocalBounds().height * 7);


    this->portText_.setFont(this->font_);
    this->portText_.setCharacterSize(32);
    this->portText_.setFillColor(sf::Color::White);
    this->portText_.setString("Enter port:");
    this->portText_.setPosition(xPosition, SCREEN_HEIGHT / 2 - this->portText_.getLocalBounds().height * 4);

    this->portTextInput_.setFont(this->font_);
    this->portTextInput_.setCharacterSize(32);
    this->portTextInput_.setFillColor(sf::Color::White);
    this->portTextInput_.setString("13877");
    this->portTextInput_.setPosition(this->portText_.getLocalBounds().width + xSpace, SCREEN_HEIGHT / 2 - this->portText_.getLocalBounds().height * 4);

    this->continueText_.setFont(this->font_);
    this->continueText_.setCharacterSize(32);
    this->continueText_.setFillColor(sf::Color::White);
    this->continueText_.setString("Press ENTER to join");
    this->continueText_.setPosition(sf::Vector2f((SCREEN_WIDTH - this->continueText_.getLocalBounds().width) / 2, SCREEN_HEIGHT / 2 - this->continueText_.getLocalBounds().height * 1));
}

Menu::~Menu() {
    delete this->window_;
    this->window_ = nullptr;
}

void Menu::render() {
    this->initializeWindow();

    while (!this->gameStarted_) {
        this->window_->clear();
        this->window_->draw(this->textMainMenu_);
        this->window_->draw(this->name_);
        this->window_->draw(this->nameInput_);
        this->window_->draw(this->ipAddressText_);
        this->window_->draw(this->ipAddressTextInput_);
        this->window_->draw(this->portText_);
        this->window_->draw(this->portTextInput_);
        this->window_->draw(this->continueText_);
        this->window_->display();

        sf::Event event;
        while (this->window_->pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Enter:
                        this->gameStarted_ = true;
                        break;
                }
            }
        }
    }
    this->window_->setActive(false);
    this->window_->close();
}

std::string Menu::getName() {
    return this->nameInput_.getString();
}

sf::IpAddress Menu::getIpAddress() {
    return sf::IpAddress(this->ipAddressTextInput_.getString());
}

int Menu::getPort() {
    std::string port = this->portTextInput_.getString();
    return std::stoi(port);
}

void Menu::initializeWindow() {
    this->window_ = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "POS-Tanks", sf::Style::Close);
    this->window_->setFramerateLimit(60);
    this->window_->setActive(true);
}
