//
// Created by vrsp on 28. 12. 2022.
//

#include "Application.h"

Application::Application() {
    this->isRunning = true;
}

Application::~Application() {

}

void Application::render() {
    this->initializeWindow();
    this->clientTank_ = new Tank();
    float positionX = (SCREEN_WIDTH - this->clientTank_->getIcon()->getSize().x) / 2;
    float positionY = (SCREEN_HEIGHT - this->clientTank_->getIcon()->getSize().x) / 2;
    this->clientTank_->getIcon()->setPosition(positionX, positionY);

    while (this->isRunning) {
        this->readClientInput();
        this->checkBorders();
        this->draw();
    }
}

void Application::draw() {
    this->window_->clear();
    this->clientTank_->render(*this->window_);

    this->window_->display();
}

void Application::readClientInput() {
    sf::Event event;
    while (this->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->isRunning = false;
            this->window_->close();
        } else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Up:
                    this->clientTank_->moveUp();
                    break;
                case sf::Keyboard::Down:
                    this->clientTank_->moveDown();
                    break;
                case sf::Keyboard::Left:
                    this->clientTank_->moveLeft();
                    break;
                case sf::Keyboard::Right:
                    this->clientTank_->moveRight();
                    break;
                case sf::Keyboard::Space:
                    this->clientTank_->fire();
                    break;
            }
        }
    }
}

void Application::checkBorders() {
    float xPosition = this->clientTank_->getIcon()->getPosition().x;
    float yPosition = this->clientTank_->getIcon()->getPosition().y;
    float width = this->clientTank_->getIcon()->getSize().x;
    float height = this->clientTank_->getIcon()->getSize().y;

//    if (xPosition + width > SCREEN_WIDTH)
//        this->clientTank_->getIcon()->x = SCREEN_WIDTH - this->clientTank_->getIcon()->w;
//
//    if (this->clientTank_->getIcon()->x < 0)
//        this->clientTank_->getIcon()->x = 0;
//
//    if (this->clientTank_->getIcon()->y + this->clientTank_->getIcon()->h > SCREEN_HEIGHT)
//        this->clientTank_->getIcon()->y = SCREEN_HEIGHT - this->clientTank_->getIcon()->h;
//
//    if (this->clientTank_->getIcon()->y < 0)
//        this->clientTank_->getIcon()->y = 0;
}

void Application::initializeWindow() {
    this->window_ = new sf::RenderWindow(sf::VideoMode(800, 800), "POS-Tanks");
}

void Application::run() {
    if (this->isRunning) {
        std::thread renderingThread(&Application::render, this);
        renderingThread.join();
    }
}
