//
// Created by filip on 5. 1. 2023.
//
#include <regex>
#include "Menu.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

/**
 * Constructor for Menu class
 */
Menu::Menu() {
    float xPosition = 50.0;
    float xSpace = 100.0;
    this->gameStarted_ = false;
    this->appClosed_ = false;
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

    this->ipAddressText_.setFont(this->font_);
    this->ipAddressText_.setCharacterSize(32);
    this->ipAddressText_.setFillColor(sf::Color::White);
    this->ipAddressText_.setString("Enter IP address:");
    this->ipAddressText_.setPosition(xPosition, SCREEN_HEIGHT / 2 - this->ipAddressText_.getLocalBounds().height * 7);

    this->portText_.setFont(this->font_);
    this->portText_.setCharacterSize(32);
    this->portText_.setFillColor(sf::Color::White);
    this->portText_.setString("Enter port:");
    this->portText_.setPosition(xPosition, SCREEN_HEIGHT / 2 - this->portText_.getLocalBounds().height * 4);

    this->textboxes_ = new std::vector<Textbox*>();
    for (int i = 0; i < 3; ++i) {
        Textbox* tmpTextbox = new Textbox(32, sf::Color::White, false);
        tmpTextbox->setFont(this->font_);
        this->textboxes_->push_back(tmpTextbox);
    }
    this->textboxes_->at(0)->setPosition(sf::Vector2f(this->name_.getLocalBounds().width + xSpace, SCREEN_HEIGHT / 2 - this->name_.getLocalBounds().height * 9));
    this->textboxes_->at(0)->setLimit(15);
    this->textboxes_->at(0)->setSelected(true);
    this->textboxes_->at(1)->setPosition(sf::Vector2f(this->ipAddressText_.getLocalBounds().width + xSpace, SCREEN_HEIGHT / 2 - this->ipAddressText_.getLocalBounds().height * 7));
    this->textboxes_->at(1)->setLimit(15);
    this->textboxes_->at(2)->setPosition(sf::Vector2f(this->portText_.getLocalBounds().width + xSpace, SCREEN_HEIGHT / 2 - this->portText_.getLocalBounds().height * 4));
    this->textboxes_->at(2)->setLimit(5);

    this->button_ = new Button(sf::Vector2f(200, 50), sf::Color(192, 192, 192), "Play", 24, sf::Color::Black);
    this->button_->setFont(this->font_);
    this->button_->setPosition(sf::Vector2f(SCREEN_WIDTH - this->button_->getButton().getSize().x - 100, this->portText_.getPosition().y + this->portText_.getLocalBounds().height + 50));

    this->textboxes_->at(1)->setInitialText("158.193.128.160");
    this->textboxes_->at(2)->setInitialText("13877");
}

/**
 * Destructor for Menu class
 */
Menu::~Menu() {
    delete this->window_;
    this->window_ = nullptr;

    for (int i = this->textboxes_->size() - 1; i >= 0; i--) {
        delete this->textboxes_->at(i);

    }
    delete this->textboxes_;
    this->textboxes_ = nullptr;

    delete this->button_;
    this->button_ = nullptr;
}

/**
 * By calling this function you'll initialize window and render main menu to player
 * He'll be able to set name, IP address of server and port
 */
void Menu::render() {
    this->initializeWindow();

    while (!this->gameStarted_ && !this->appClosed_) {
        this->window_->clear();

        this->window_->draw(this->textMainMenu_);
        this->window_->draw(this->name_);
        this->window_->draw(this->ipAddressText_);
        this->window_->draw(this->portText_);

        for (Textbox *textbox: *this->textboxes_) {
            this->window_->draw(textbox->getTextBox());
        }

        this->validate();
        this->window_->draw(this->button_->getButton());
        this->window_->draw(this->button_->getText());

        this->window_->display();

        sf::Event event;
        while (this->window_->pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                        for (int i = 0; i < this->textboxes_->size(); ++i) {
                            if (this->textboxes_->at(i)->isSelected() && i > 0) {
                                this->textboxes_->at(i)->setSelected(false);
                                this->textboxes_->at(i - 1)->setSelected(true);
                                break;
                            }
                        }
                        break;

                    case sf::Keyboard::Down:
                        for (int i = 0; i < this->textboxes_->size(); ++i) {
                            if (this->textboxes_->at(i)->isSelected() && i < this->textboxes_->size() - 1) {
                                this->textboxes_->at(i)->setSelected(false);
                                this->textboxes_->at(i + 1)->setSelected(true);
                                break;
                            }
                        }
                        break;

                    case sf::Keyboard::Escape:
                        this->appClosed_ = true;
                        break;
                }
            } else if (event.type == sf::Event::TextEntered) {
                for (Textbox* textbox : *this->textboxes_) {
                    if (textbox->isSelected()) {
                        textbox->typed(event);
                    }
                }
            } else if (event.type == sf::Event::MouseMoved && this->validate()) {
                if (this->button_->isMouseOver(*this->window_)) {
                    this->button_->setBgColor(sf::Color(0, 153, 0));
                    this->button_->setTextColor(sf::Color::White);
                } else {
                    this->button_->setBgColor(sf::Color(0, 255, 0));
                    this->button_->setTextColor(sf::Color::Black);
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (this->button_->isMouseOver(*this->window_) && this->validate()) {
                    this->gameStarted_ = true;
                }
            } else if (event.type == sf::Event::Closed) {
                this->appClosed_ = true;
            }
        }
    }

    this->window_->setActive(false);
    this->window_->close();
}

/**
 * Getter for information if menu was closed
 * @return
 */
bool Menu::getAppClosed() {
    return this->appClosed_;
}

/**
 * Getter for IP address
 * @return IP address
 */
sf::IpAddress Menu::getIpAddress() {
    return this->textboxes_->at(1)->getText();
}

/**
 * Getter for port
 * @return port
 */
int Menu::getPort() {
    std::string port = this->textboxes_->at(2)->getText();
    return std::stoi(port);
}

/**
 * Getter for name of player
 */
std::string Menu::getName() {
    return this->textboxes_->at(0)->getText();
}

/**
 * By calling this function you'll initialize window
 */
void Menu::initializeWindow() {
    this->window_ = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "POS-Tanks", sf::Style::Close);
    this->window_->setFramerateLimit(60);
    this->window_->setActive(true);
}

/**
 * By calling this function you'll validate player inputs
 * @return if all inputs are valid
 */
bool Menu::validate() {
    if (!this->textboxes_->at(0)->getText().empty() && std::regex_match(this->textboxes_->at(1)->getText(), std::regex("^(?:25[0-5]|2[0-4]\\d|[0-1]?\\d{1,2})(?:\\.(?:25[0-5]|2[0-4]\\d|[0-1]?\\d{1,2})){3}$")) && std::regex_match(this->textboxes_->at(2)->getText(), std::regex("[0-9]+"))) {
        if (this->button_->isMouseOver(*this->window_)){
            this->button_->setBgColor(sf::Color(0, 153, 0));
            this->button_->setTextColor(sf::Color::White);
        } else {
            this->button_->setBgColor(sf::Color(0, 255, 0));
            this->button_->setTextColor(sf::Color::Black);
        }
        return true;
    }
    this->button_->setBgColor(sf::Color(192, 192, 192));
    this->button_->setTextColor(sf::Color::Black);
    return false;
}