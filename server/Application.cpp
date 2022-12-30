//
// Created by vrsp on 30. 12. 2022.
//

#include "Application.h"

Application::Application() {
    this->isRunning = true;
}

Application::~Application() {
    delete this->clientTank_;
    delete this->otherTanks;
    delete this->otherClientTank_;
    delete this->window_;

    this->clientTank_ = nullptr;
    this->otherTanks = nullptr;
    this->otherClientTank_ = nullptr;
    this->window_ = nullptr;
}

void Application::render() {
    this->initializeWindow();
    this->clientTank_ = new Tank();
    float positionX = (SCREEN_WIDTH - this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x) / 2;
    float positionY = (SCREEN_HEIGHT - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y);
    this->clientTank_->getSprite()->setPosition(positionX, positionY);

    while (this->isRunning) {
        this->readClientInput();
        this->checkBorders();
        this->draw();
    }
}

void Application::draw() {
    this->window_->clear();
    this->clientTank_->render(*this->window_);
    if (this->clientReadyToPlay_ && this->clientTank_ != nullptr) {
        this->otherClientTank_->render(*this->window_);
    }

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
    float xPosition = this->clientTank_->getSprite()->getPosition().x;
    float yPosition = this->clientTank_->getSprite()->getPosition().y;

    if (xPosition > SCREEN_WIDTH)
        this->clientTank_->getSprite()->setPosition(sf::Vector2f(SCREEN_WIDTH, yPosition));

    if (xPosition < 0)
        this->clientTank_->getSprite()->setPosition(sf::Vector2f(0, yPosition));

    if (yPosition > SCREEN_HEIGHT)
        this->clientTank_->getSprite()->setPosition(sf::Vector2f(xPosition, SCREEN_HEIGHT));

    if (yPosition < 0)
        this->clientTank_->getSprite()->setPosition(sf::Vector2f(xPosition, 0));
}

void Application::initializeWindow() {
    this->window_ = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "POS-Tanks", sf::Style::Close);
}

void Application::run() {
    if (this->isRunning) {
        std::thread renderingThread(&Application::render, this);
        std::thread communicatingThread(&Application::communicate, this);
        renderingThread.join();
        communicatingThread.join();
    }
}

void Application::initializeSocket() {
    this->packetSend_ = sf::Packet{};
    this->packetRecieve_ = sf::Packet{};
    this->ipAddress_ = sf::IpAddress::Any;

    if (this->socket_.bind(55000, this->ipAddress_) != sf::Socket::Done) {
        std::cout << "Unable to bind PORT 55000" << std::endl;
    }
}

void Application::waitForClients() {
    float tmpX, tmpY;
    int tmpDir;

    if (socket_.receive(this->packetRecieve_, this->ipAddress_, this->port_) == sf::Socket::Done) {
        std::cout << "Client was connected\n";
        this->packetRecieve_ >> tmpX;
        this->packetRecieve_ >> tmpY;
        this->packetRecieve_ >> tmpDir;
        this->packetRecieve_ >> this->clientReadyToPlay_;
    }

    if (this->clientReadyToPlay_) {
        this->otherClientTank_ = new Tank();
        this->otherClientTank_->getSprite()->setPosition(sf::Vector2f(tmpX, tmpY));
        this->otherClientTank_->setDirection(static_cast<DIRECTION>(tmpDir));
    }
}

void Application::communicate() {
    this->initializeSocket();
    this->waitForClients();

    while (this->clientReadyToPlay_) {
        float tmpX, tmpY;
        int tmpDir;

        if (this->socket_.receive(this->packetRecieve_, this->ipAddress_, this->port_) == sf::Socket::Done) {
            this->packetRecieve_ >> tmpX;
            this->packetRecieve_ >> tmpY;
            this->packetRecieve_ >> tmpDir;
            this->packetRecieve_ >> this->clientReadyToPlay_;
        }

        if (this->otherClientTank_->getDirection() != tmpDir) {
            this->otherClientTank_->rotate(static_cast<DIRECTION>(tmpDir));
        }
        this->otherClientTank_->getSprite()->setPosition(sf::Vector2f(tmpX, tmpY));
        this->otherClientTank_->setDirection(static_cast<DIRECTION>(tmpDir));
    }

    this->isRunning = false;
}