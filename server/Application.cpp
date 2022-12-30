//
// Created by vrsp on 30. 12. 2022.
//

#include "Application.h"

Application::Application() {
    this->isRunning = true;
    this->otherTanks = new std::list<Tank*>;
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
    if (this->otherTanks != nullptr) {
        for (auto tank : *this->otherTanks) {
            if (tank != nullptr) {
                tank->render(*this->window_);
            }
        }
    }
//    if (this->clientReadyToPlay_ && this->clientTank_ != nullptr) {
//        this->otherClientTank_->render(*this->window_);
//    }

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

    std::cout << "server\n" << "\tip: " << this->ipAddress_.toString() << std::endl << "\tport: " << this->port_ << std::endl;
}

//typedef struct ipAndPort {
//    sf::IpAddress ip;
//    unsigned short port;
//} IPANDPORT;

void Application::waitForClients() {
    float tmpX, tmpY;
    int tmpDir;

//    std::cout << "server\n" << "\tip: " << this->ipAddress_.toString() << std::endl << "\tport: " << this->port_ << std::endl;

    std::cout << "Enter number of players (max 4): ";
    int n, count;
    std::cin >> n;
    if (n > 4) {
        n = 4;
    }
    std::cout << std::endl;

    while (count < n - 1) {
        if (this->socket_.receive(this->packetRecieve_, this->ipAddress_, this->port_) == sf::Socket::Done) {
            std::cout << "Client was connected\n";
            int id;
            this->packetRecieve_ >> id;
            this->packetRecieve_ >> tmpX;
            this->packetRecieve_ >> tmpY;
            this->packetRecieve_ >> tmpDir;
            this->packetRecieve_ >> this->clientReadyToPlay_;
        }
        this->packetSend_.clear();

        float positionX, positionY;
        DIRECTION tmpDir;

        if (count == 0) {
            positionX = (SCREEN_WIDTH - this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x) / 2;
            positionY = 0;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << 1;
            this->packetSend_ << 1;
            tmpDir = DOWN;
        } else if (count == 1) {
            positionX = 0;
            positionY = (SCREEN_HEIGHT - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y) / 2;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << 2;
            this->packetSend_ << 2;
            tmpDir = LEFT;
        } else if (count == 2) {
            positionX = (SCREEN_WIDTH - this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x);
            positionY = (SCREEN_HEIGHT - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y) / 2;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << 3;
            this->packetSend_ << 3;
            tmpDir = RIGHT;
        }

        Tank * tmpTank = new Tank();
        tmpTank->getSprite()->setPosition(sf::Vector2f(positionX, positionY));
        tmpTank->rotate(tmpDir);
        this->otherTanks->push_back(tmpTank);

        count++;

        if (this->socket_.send(this->packetSend_, this->ipAddress_, this->port_) == sf::Socket::Done) {
            std::cout << "Client has id...\n";
        }
    }

//    std::cout << "client\n" << "\tip: " << this->ipAddress_.toString() << std::endl << "\tport: " << this->port_ << std::endl;
//
//    if (this->clientReadyToPlay_) {
//        this->otherClientTank_ = new Tank();
//        this->otherClientTank_->getSprite()->setPosition(sf::Vector2f(tmpX, tmpY));
//        this->otherClientTank_->setDirection(static_cast<DIRECTION>(tmpDir));
//    }
}

//TODO: prerobiť na list tankov
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