//
// Created by vrsp on 28. 12. 2022.
//

#include "Application.h"

Application::Application() {
    this->isRunning = true;
    this->packetSend_ = sf::Packet{};
    this->ipAddress_ = sf::IpAddress("158.193.128.160");
    this->id_ = 0;
}

Application::~Application() {

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
        std::thread sendingThread(&Application::sendData, this);
        renderingThread.join();
        sendingThread.join();
    }
}

void Application::sendData() {
    while (this->isRunning) {
        if (this->window_ != nullptr && this->clientTank_ != nullptr) {
            this->packetSend_.clear();

            this->packetSend_ << this->id_;
            this->packetSend_ << this->clientTank_->getSprite()->getPosition().x;
            this->packetSend_ << this->clientTank_->getSprite()->getPosition().y;
            this->packetSend_ << this->clientTank_->getDirection();
            this->packetSend_ << this->isRunning;
            if (this->socket_.send(this->packetSend_, this->ipAddress_, 13877) != sf::Socket::Done) {
                std::cout << "Sending failed" << "\n";
            }
            if (this->id_ == 0) {
                sf::Packet packetRecieve = sf::Packet{};
                sf::IpAddress ipAddress = sf::IpAddress{};
                unsigned short port;
                float tmpX, tmpY;
                int tmpDir, tmpID;
                if (this->socket_.receive(packetRecieve, ipAddress, port) == sf::Socket::Done) {
                    packetRecieve >> tmpX;
                    packetRecieve >> tmpY;
                    packetRecieve >> tmpDir;
                    packetRecieve >> tmpID;
                }
                this->clientTank_->getSprite()->setPosition(tmpX, tmpY);
                this->clientTank_->rotate(static_cast<DIRECTION>(tmpDir));
                this->id_ = tmpID;
                if (this->socket_.receive(packetRecieve, ipAddress, port) == sf::Socket::Done) {

                }
            }
        }
    }
}

void Application::connectToServer() {
//    if (this->socket_.bind(43333) != sf::Socket::Done) {
//        std::cout << "Unable to bind PORT 55000" << std::endl;
//    }


    this->packetSend_.clear();
    if (this->socket_.send(this->packetSend_, this->ipAddress_, 13877) != sf::Socket::Done) {
        std::cout << "Sending failed" << "\n";
    }

    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;
    float tmpX, tmpY;
    int tmpDir, tmpID;
    sf::Packet packetRecieve = sf::Packet{};
    packetRecieve.clear();

    if (this->socket_.receive(packetRecieve, ipAddress, port) == sf::Socket::Done) {
        packetRecieve >> tmpX;
        packetRecieve >> tmpY;
        packetRecieve >> tmpID;
        packetRecieve >> tmpDir;
    }

    std::cout << "X: " << tmpX << " Y: " << tmpY << " ID: " << tmpID << "\n";
}
