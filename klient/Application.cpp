//
// Created by vrsp on 28. 12. 2022.
//

#include "Application.h"

Application::Application() {
    this->isRunning = true;
    this->packetSend_ = sf::Packet{};
    this->ipAddress_ = sf::IpAddress("158.193.128.160");
    this->id_ = 0;
    this->clientTank_ = new Tank();
    this->otherTanks = new std::vector<Tank *>();
    this->playerWasKilled = false;
}

Application::~Application() {
    delete this->mutex;
    this->mutex = nullptr;

    delete this->sendDataCond;
    this->sendDataCond = nullptr;

    delete this->clientTank_;
    this->clientTank_ = nullptr;

    for (int i = this->otherTanks->size() - 1; i > 0; i--) {
        delete this->otherTanks->at(i);

    }
    delete this->otherTanks;
    this->otherTanks = nullptr;

    delete this->window_;
    this->window_ = nullptr;

}

void Application::render() {
    this->initializeWindow();
//    this->clientTank_ = new Tank();
//    float positionX = (SCREEN_WIDTH - this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x) / 2;
//    float positionY = (SCREEN_HEIGHT - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y);
//    this->clientTank_->getSprite()->setPosition(positionX, positionY);

    while (this->isRunning) {
        this->readClientInput();
        this->checkBorders();
        this->checkBulletCollision();
        this->draw();
    }
}

void Application::draw() {
    this->window_->clear();
    this->clientTank_->render(*this->window_);
    for (Tank *tank: *this->otherTanks) {
        tank->render(*this->window_);
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
            std::unique_lock<std::mutex> loc(*this->mutex);
            this->sendDataBool = true;
            this->sendDataCond->notify_one();
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
    this->window_->setFramerateLimit(60);
}

void Application::run() {
    this->sendDataBool = false;
    this->communicationWithServer();
    this->mutex = new std::mutex();
    this->sendDataCond = new std::condition_variable();
    if (this->isRunning) {
        std::thread renderingThread(&Application::render, this);
        std::thread receivingData(&Application::receiveData, this);
        std::thread sendingThread(&Application::sendData, this);
        receivingData.join();
        sendingThread.join();
        renderingThread.join();
    }
}

void Application::sendData() {
    sf::Packet packetReceive = sf::Packet{};
    sf::Packet packetSend = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    float positionX, positionY;

    while (this->isRunning) {
        std::unique_lock<std::mutex> loc(*this->mutex);
        while (!this->sendDataBool) {
            this->sendDataCond->wait(loc);
        }

        if (this->window_ != nullptr && this->clientTank_ != nullptr) {
            packetSend.clear();

            if (this->playerWasKilled) {
                packetSend << (static_cast<int>(KILLED) + 1);
                packetSend << this->idOfKilledPlayer;
                packetSend << this->clientTank_->getPlayerId();
                std::cout << "Player " << this->clientTank_->getPlayerId() << " Killed " << this->idOfKilledPlayer << "\n";
                this->playerWasKilled = false;
            } else if (this->isRunning) {
                positionX = this->clientTank_->getSprite()->getPosition().x;
                positionY = this->clientTank_->getSprite()->getPosition().y;

                packetSend << (static_cast<int>(STATUS) + 1);
                packetSend << this->clientTank_->getPlayerId();
                packetSend << positionX;
                packetSend << positionY;
                packetSend << static_cast<int>(this->clientTank_->getDirection());
                packetSend << (this->clientTank_->getBullet()->wasFired() &&
                               !this->clientTank_->getBullet()->wasFiredAndSent());

                if (this->clientTank_->getBullet()->wasFired() && !this->clientTank_->getBullet()->wasFiredAndSent()) {
                    this->clientTank_->getBullet()->setWasFiredAndSent();
                }
            }


            if (this->socket_.send(packetSend, this->ipAddress_, 13877) != sf::Socket::Done) {
                std::cout << "Sending failed" << "\n";
            }


            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
//        std::cout << "Data were sended" << "\n";
        this->sendDataBool = false;
    }
}

void Application::connectToServer() {
    this->packetSend_.clear();
    if (this->socket_.send(this->packetSend_, this->ipAddress_, 13877) != sf::Socket::Done) {
        std::cout << "Sending failed" << "\n";
    }

    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;
    float tmpX, tmpY;
    int tmpDir, tmpID, numberOfPlayers;
    sf::Packet packetReceive = sf::Packet{};
    packetReceive.clear();

    if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done) {
        packetReceive >> tmpX;
        packetReceive >> tmpY;
        packetReceive >> tmpID;
        packetReceive >> tmpDir;
        packetReceive >> numberOfPlayers;
    }

    switch (static_cast<DIRECTION>(tmpDir)) {
        case UP:
            tmpX = tmpX - this->clientTank_->getSprite()->getTexture()->getSize().x *
                          this->clientTank_->getSprite()->getScale().x / 2;
            tmpY = tmpY - this->clientTank_->getSprite()->getTexture()->getSize().y *
                          this->clientTank_->getSprite()->getScale().y;
            break;
        case DOWN:
            tmpX = tmpX - this->clientTank_->getSprite()->getTexture()->getSize().x *
                          this->clientTank_->getSprite()->getScale().x / 2;
            tmpY = tmpY - this->clientTank_->getSprite()->getTexture()->getSize().y *
                          this->clientTank_->getSprite()->getScale().y / 3;
            break;
        case LEFT:
            tmpX = tmpX - this->clientTank_->getSprite()->getTexture()->getSize().x *
                          this->clientTank_->getSprite()->getScale().x;
            tmpY = tmpY - this->clientTank_->getSprite()->getTexture()->getSize().y *
                          this->clientTank_->getSprite()->getScale().y * 2 / 3;
            break;
        case RIGHT:
            tmpY = tmpY - this->clientTank_->getSprite()->getTexture()->getSize().y *
                          this->clientTank_->getSprite()->getScale().y * 2 / 3;
            break;
    }

    this->clientTank_->getSprite()->setPosition(tmpX, tmpY);
    this->clientTank_->rotate(static_cast<DIRECTION>(tmpDir));
    this->clientTank_->setDirection(static_cast<DIRECTION>(tmpDir));
    this->numberOfPlayers_ = numberOfPlayers;
    this->clientTank_->setPlayerId(tmpID);
    this->id_ = tmpID;

    std::cout << "X: " << tmpX << " Y: " << tmpY << " ID: " << tmpID << "\n";
}

void Application::waitForGameSettings() {
    sf::Packet packetReceive = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;

    int pId, direction;
    float positionX, positionY;

    packetReceive.clear();

    if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done)
        std::cout << "Packet with game settings was received\n";

    for (int i = 0; i < this->numberOfPlayers_ - 1; ++i) {
        packetReceive >> pId;
        packetReceive >> positionX;
        packetReceive >> positionY;
        packetReceive >> direction;
        Tank *tmpTank = new Tank();
        tmpTank->setPlayerId(pId);

        switch (static_cast<DIRECTION>(direction)) {
            case UP:
                break;
            case DOWN:
                tmpTank->setDirection(DOWN);
                tmpTank->rotate(UP);
                tmpTank->setDirection(DOWN);
                break;
            case LEFT:
                positionX = positionX + this->clientTank_->getSprite()->getTexture()->getSize().y *
                                        this->clientTank_->getSprite()->getScale().y -
                            this->clientTank_->getSprite()->getTexture()->getSize().x *
                            this->clientTank_->getSprite()->getScale().x;
                positionY = positionY - this->clientTank_->getSprite()->getTexture()->getSize().y *
                                        this->clientTank_->getSprite()->getScale().y;
                break;
            case RIGHT:
                positionX = positionX - this->clientTank_->getSprite()->getTexture()->getSize().y *
                                        this->clientTank_->getSprite()->getScale().y;
                positionY = positionY - this->clientTank_->getSprite()->getTexture()->getSize().y *
                                        this->clientTank_->getSprite()->getScale().y +
                            this->clientTank_->getSprite()->getTexture()->getSize().x *
                            this->clientTank_->getSprite()->getScale().x;
                break;
        }

        tmpTank->getSprite()->setPosition(positionX, positionY);
        tmpTank->rotate(static_cast<DIRECTION>(direction));

        this->otherTanks->push_back(tmpTank);
//        std::cout << "OtherTank --> X: " << positionX << " Y: " << positionY << " ID: " << pId << "\n";
    }

}

void Application::communicationWithServer() {
    this->connectToServer();
    this->updatePositionsOfTanks();
    this->waitForGameSettings();
//    this->sendData();
}

void Application::updatePositionsOfTanks() {
    sf::Packet packetReceive = sf::Packet{};
    sf::Packet packetSend = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;

    float positionX, positionY;

    if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done) {
        std::cout << "\n\nClient have permission to send updated position.\n";
    }
    packetReceive.clear();

    packetSend.clear();
    positionX = this->clientTank_->getSprite()->getPosition().x;
    positionY = this->clientTank_->getSprite()->getPosition().y;

    packetSend << this->clientTank_->getPlayerId();
    packetSend << positionX;
    packetSend << positionY;
    packetSend << static_cast<int>(this->clientTank_->getDirection());
    if (this->socket_.send(packetSend, this->ipAddress_, 13877) != sf::Socket::Done) {
        std::cout << "Sending failed" << "\n";
    }
}

void Application::receiveData() {
    sf::Packet packetReceive = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;

    int pId, direction;
    float positionX, positionY;
    bool fired;
    int messageType, killerId;

    while (this->isRunning) {
        packetReceive.clear();
        if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done) {
//                std::cout << "Packet with game settings was received\n";
            packetReceive >> messageType;
            messageType--;
            if (static_cast<TYPES_MESSAGES>(messageType) == STATUS) {
                for (int i = 0; i < this->numberOfPlayers_ - 1; ++i) {
                    packetReceive >> pId;
                    packetReceive >> positionX;
                    packetReceive >> positionY;
                    packetReceive >> direction;
                    packetReceive >> fired;

                    for (Tank *tank: *this->otherTanks) {
                        if (tank->getPlayerId() == pId) {
                            if (tank->getDirection() != direction) {
                                tank->rotate(static_cast<DIRECTION>(direction));
                            }
                            tank->getSprite()->setPosition(positionX, positionY);
                            if (fired)
                                tank->fire();
                        }
                        std::cout << "Client: " << tank->getPlayerId() << " --> X: " << tank->getSprite()->getPosition().x
                                  << " Y: " << tank->getSprite()->getPosition().y << "\n";
                    }
                }
            } else if (static_cast<TYPES_MESSAGES>(messageType) == KILLED) {
                packetReceive >> pId;
                packetReceive >> positionX;
                packetReceive >> positionY;
                packetReceive >> direction;
                packetReceive >> killerId;

                std::cout << "Killed player: " << pId << "Killer is: " << killerId;

                if (pId == this->id_) {
                    this->clientTank_->getBullet()->setFired(false);
                    this->clientTank_->getSprite()->setPosition(positionX, positionY);
                    this->clientTank_->rotate(static_cast<DIRECTION>(direction));


                } else {
                    for(Tank* tank: *this->otherTanks) {
                        if (tank->getPlayerId() == pId) {
                            tank->getBullet()->setFired(false);
                            if (tank->getDirection() != direction) {
                                tank->rotate(static_cast<DIRECTION>(direction));
                            }
                            tank->getSprite()->setPosition(positionX, positionY);
                        }
                    }
                }
                for(Tank* tank: *this->otherTanks) {
                    if (tank->getPlayerId() == killerId) {
                        tank->getBullet()->setFired(false);
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void Application::checkBulletCollision() {
    Bullet *bullet = this->clientTank_->getBullet();

    if (bullet != nullptr) {
        if (bullet->wasFired()) {
            float bulletPosX = bullet->getBulletPosition().x;
            float bulletPosY = bullet->getBulletPosition().y;
            float bulletSizeX = bullet->getBulletSize().x;
            float bulletSizeY = bullet->getBulletSize().y;

            for (Tank *tank: *this->otherTanks) {
                float tankPosX = tank->getSprite()->getPosition().x;
                float tankPosY = tank->getSprite()->getPosition().y;
                float tankSizeX = tank->getSprite()->getTexture()->getSize().x * tank->getSprite()->getScale().x;
                float tankSizeY = tank->getSprite()->getTexture()->getSize().y * tank->getSprite()->getScale().y;

                if (bullet->wasFired()) {
                    switch (tank->getDirection()) {
                        case UP:
                            if (bulletPosX + bulletSizeX >= tankPosX && bulletPosX <= tankPosX + tankSizeX &&
                                bulletPosY + bulletSizeY >= tankPosY && bulletPosY <= tankPosY + tankSizeY) {
                                std::cout << "Client " << tank->getPlayerId() << " was hit. (Client.getRotation()) = UP"
                                          << std::endl;
                                bullet->setFired(false);
                                this->playerWasKilled = true;
                                this->idOfKilledPlayer = tank->getPlayerId();
                                std::unique_lock<std::mutex> loc(*this->mutex);
                                this->sendDataBool = true;
                                this->sendDataCond->notify_one();
                            }
                            break;

                        case DOWN:
                            if (bulletPosX + bulletSizeX >= tankPosX - tankSizeX && bulletPosX <= tankPosX &&
                                bulletPosY + bulletSizeY >= tankPosY - tankSizeY && bulletPosY <= tankPosY) {
                                std::cout << "Client " << tank->getPlayerId()
                                          << " was hit. (Client.getRotation()) = DOWN" << std::endl;
                                bullet->setFired(false);
                                this->playerWasKilled = true;
                                this->idOfKilledPlayer = tank->getPlayerId();
                                std::unique_lock<std::mutex> loc(*this->mutex);
                                this->sendDataBool = true;
                                this->sendDataCond->notify_one();
                            }
                            break;

                        case LEFT:
                            if (bulletPosX + bulletSizeX >= tankPosX && bulletPosX <= tankPosX + tankSizeY &&
                                bulletPosY + bulletSizeY >= tankPosY - tankSizeX && bulletPosY <= tankPosY) {
                                std::cout << "Client " << tank->getPlayerId()
                                          << " was hit. (Client.getRotation()) = LEFT" << std::endl;
                                bullet->setFired(false);
                                this->playerWasKilled = true;
                                this->idOfKilledPlayer = tank->getPlayerId();
                                std::unique_lock<std::mutex> loc(*this->mutex);
                                this->sendDataBool = true;
                                this->sendDataCond->notify_one();
                            }
                            break;

                        case RIGHT:
                            if (bulletPosX + bulletSizeX >= tankPosX - tankSizeY && bulletPosX <= tankPosX &&
                                bulletPosY + bulletSizeY >= tankPosY && bulletPosY <= tankPosY + tankSizeX) {
                                std::cout << "Client " << tank->getPlayerId()
                                          << " was hit. (Client.getRotation()) = RIGHT" << std::endl;
                                bullet->setFired(false);
                                this->playerWasKilled = true;
                                this->idOfKilledPlayer = tank->getPlayerId();
                                std::unique_lock<std::mutex> loc(*this->mutex);
                                this->sendDataBool = true;
                                this->sendDataCond->notify_one();
                            }
                            break;
                    }
                }
            }
        }
    }
}