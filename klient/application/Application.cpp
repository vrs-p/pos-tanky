//
// Created by vrsp on 28. 12. 2022.
//
#include <utility>
#include "Application.h"

/**
 * Constructor for Application class
 */
Application::Application() {
    this->packetSend_ = sf::Packet{};
    this->id_ = 0;

    this->isRunning_ = true;
    this->playerWasKilled_ = false;

    this->font_.loadFromFile("../font/consola.ttf");
    this->nameOfPlayer_.setFont(this->font_);
    this->nameOfPlayer_.setCharacterSize(11);
    this->nameOfPlayer_.setFillColor(sf::Color::White);
    this->clientTank_ = new Tank();
    this->otherTanks_ = new std::vector<Tank *>();
}

/**
 * Destructor for Application class
 */
Application::~Application() {
    delete this->mutex_;
    this->mutex_ = nullptr;

    delete this->sendDataCond_;
    this->sendDataCond_ = nullptr;

    delete this->window_;
    this->window_ = nullptr;

    delete this->clientTank_;
    this->clientTank_ = nullptr;

    for (int i = this->otherTanks_->size() - 1; i >= 0; i--) {
        delete this->otherTanks_->at(i);

    }
    delete this->otherTanks_;
    this->otherTanks_ = nullptr;
}

/**
 * By calling this method you are able to start main program
 * You'll also initialize communication with server and whole game
 */
void Application::run(sf::IpAddress ipAddress, int port, std::string playerName) {
    this->ipAddress_ = ipAddress;
    this->port_ = port;
    this->sendDataBool_ = false;
    this->nameOfPlayer_.setString(playerName);
    this->clientTank_->setPlayerName(std::move(playerName));

    this->communicationWithServer();

    this->mutex_ = new std::mutex();
    this->sendDataCond_ = new std::condition_variable();

    if (this->isRunning_) {
        std::thread renderingThread(&Application::render, this);
        std::thread receivingData(&Application::receiveData, this);
        std::thread sendingThread(&Application::sendData, this);
        receivingData.join();
        sendingThread.join();
        renderingThread.join();
    }
}

/**
 * Getter for score of player
 * @return score of player
 */
int Application::getPlayerScore() {
    return this->clientTank_->getScore();
}

/**
 * Getter for list of other players
 * @return list of oponents
 */
std::vector<Tank *> *Application::getOthersTanks() {
    return this->otherTanks_;
}

/**
 * By calling this function you'll initialize communication with server
 * You'll connect to server
 * Receive ID, position and direction of your tank
 * Receive IDs, positions and directions of your oponents
 */
void Application::communicationWithServer() {
    this->connectToServer();
    this->updatePositionsOfTanks();
    this->waitForGameSettings();
}

/**
 * By calling this function you'll connect to server
 */
void Application::connectToServer() {
    this->packetSend_.clear();
    this->packetSend_ << this->clientTank_->getPlayerName();
    if (this->socket_.send(this->packetSend_, this->ipAddress_, this->port_) != sf::Socket::Done) {
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
                          this->clientTank_->getSprite()->getScale().y - 20;
            break;

        case DOWN:
            tmpX = tmpX - this->clientTank_->getSprite()->getTexture()->getSize().x *
                          this->clientTank_->getSprite()->getScale().x / 2;
            tmpY = tmpY - this->clientTank_->getSprite()->getTexture()->getSize().y *
                          this->clientTank_->getSprite()->getScale().y / 3 + 20;
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

    switch (tmpID) {
        case 2:
            this->clientTank_->getSprite()->setColor(sf::Color(0, 255, 0, 192));
            break;

        case 3:
            this->clientTank_->getSprite()->setColor(sf::Color(255, 0, 0, 192));
            break;

        case 4:
            this->clientTank_->getSprite()->setColor(sf::Color(0, 255, 255, 192));
            break;

        default:
            break;
    }

    this->id_ = tmpID;
    this->numberOfPlayers_ = numberOfPlayers;
    this->clientTank_->getSprite()->setPosition(tmpX, tmpY);
    this->clientTank_->rotate(static_cast<DIRECTION>(tmpDir));
    this->clientTank_->setDirection(static_cast<DIRECTION>(tmpDir));
    this->clientTank_->setPlayerId(tmpID);

    std::cout << "X: " << tmpX << " Y: " << tmpY << " ID: " << tmpID << "\n";
}

/**
 * By calling this function you'll wait for game settings
 * Like number of opponents and information about them
 */
void Application::waitForGameSettings() {
    sf::Packet packetReceive = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;
    int pId, direction;
    float positionX, positionY;
    std::string pName;

    packetReceive.clear();

    if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done)
        std::cout << "Packet with game settings was received\n";

    for (int i = 0; i < this->numberOfPlayers_ - 1; ++i) {
        packetReceive >> pId;
        packetReceive >> pName;
        packetReceive >> positionX;
        packetReceive >> positionY;
        packetReceive >> direction;
        Tank *tmpTank = new Tank();
        tmpTank->setPlayerId(pId);
        tmpTank->setPlayerName(pName);

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

        switch (tmpTank->getPlayerId()) {
            case 2:
                tmpTank->getSprite()->setColor(sf::Color(0, 255, 0, 192));
                break;

            case 3:
                tmpTank->getSprite()->setColor(sf::Color(255, 0, 0, 192));
                break;

            case 4:
                tmpTank->getSprite()->setColor(sf::Color(0, 255, 255, 192));
                break;
        }

        tmpTank->getSprite()->setPosition(positionX, positionY);
        tmpTank->rotate(static_cast<DIRECTION>(direction));

        this->otherTanks_->push_back(tmpTank);
    }

}

/**
 * Thread function used for rendering game
 * You'll read input from player update his location
 * and render yours and other players tanks
 */
void Application::render() {
    this->initializeWindow();
    this->window_->setActive(true);

    while (this->isRunning_) {
        this->readClientInput();
        this->checkBorders();
        this->checkBulletCollision();
        this->draw();
    }

    this->window_->setActive(false);
    this->window_->close();
}

/**
 * By calling this function you'll initialize graphic window for game
 */
void Application::initializeWindow() {
    this->window_ = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "POS-Tanks", sf::Style::Close);
    this->window_->setFramerateLimit(60);
    this->window_->setActive(true);
}

/**
 * By calling this function you'll read client input
 * if he pressed any key it'll update his status
 */
void Application::readClientInput() {
    sf::Event event;
    while (this->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->clientTank_->setLeft(true);
            std::unique_lock<std::mutex> loc(*this->mutex_);
            this->sendDataBool_ = true;
            this->sendDataCond_->notify_one();
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

                case sf::Keyboard::Q:
                case sf::Keyboard::Escape:
                    this->clientTank_->setLeft(true);
                    break;
            }

            std::unique_lock<std::mutex> loc(*this->mutex_);
            this->sendDataBool_ = true;
            this->sendDataCond_->notify_one();
        }
    }
}

/**
 * By calling this function it'll check if tank is not outside of borders
 */
void Application::checkBorders() {
    float xPosition = this->clientTank_->getSprite()->getPosition().x;
    float yPosition = this->clientTank_->getSprite()->getPosition().y;

    if (xPosition > SCREEN_WIDTH) {
        this->clientTank_->getSprite()->setPosition(sf::Vector2f(SCREEN_WIDTH, yPosition));
    }

    if (xPosition < 0) {
        this->clientTank_->getSprite()->setPosition(sf::Vector2f(0, yPosition));
    }

    if (yPosition > SCREEN_HEIGHT) {
        this->clientTank_->getSprite()->setPosition(sf::Vector2f(xPosition, SCREEN_HEIGHT));
    }

    if (yPosition < 0) {
        this->clientTank_->getSprite()->setPosition(sf::Vector2f(xPosition, 0));
    }
}

/**
 * By calling this function you'll check if your bullet hit some other player
 */
void Application::checkBulletCollision() {
    Bullet *bullet = this->clientTank_->getBullet();

    if (bullet != nullptr) {
        if (bullet->wasFired()) {
            float bulletPosX = bullet->getBulletPosition().x;
            float bulletPosY = bullet->getBulletPosition().y;
            float bulletSizeX = bullet->getBulletSize().x;
            float bulletSizeY = bullet->getBulletSize().y;

            for (Tank *tank: *this->otherTanks_) {
                float tankPosX = tank->getSprite()->getPosition().x;
                float tankPosY = tank->getSprite()->getPosition().y;
                float tankSizeX = tank->getSprite()->getTexture()->getSize().x * tank->getSprite()->getScale().x;
                float tankSizeY = tank->getSprite()->getTexture()->getSize().y * tank->getSprite()->getScale().y;

                if (bullet->wasFired()) {
                    switch (tank->getDirection()) {
                        case UP:
                            if (bulletPosX + bulletSizeX >= tankPosX && bulletPosX <= tankPosX + tankSizeX &&
                                bulletPosY + bulletSizeY >= tankPosY && bulletPosY <= tankPosY + tankSizeY) {
                                bullet->setFired(false);
                                this->playerWasKilled_ = true;
                                this->idOfKilledPlayer_ = tank->getPlayerId();
                                std::unique_lock<std::mutex> loc(*this->mutex_);
                                this->sendDataBool_ = true;
                                this->sendDataCond_->notify_one();
                            }
                            break;

                        case DOWN:
                            if (bulletPosX + bulletSizeX >= tankPosX - tankSizeX && bulletPosX <= tankPosX &&
                                bulletPosY + bulletSizeY >= tankPosY - tankSizeY && bulletPosY <= tankPosY) {
                                bullet->setFired(false);
                                this->playerWasKilled_ = true;
                                this->idOfKilledPlayer_ = tank->getPlayerId();
                                std::unique_lock<std::mutex> loc(*this->mutex_);
                                this->sendDataBool_ = true;
                                this->sendDataCond_->notify_one();
                            }
                            break;

                        case LEFT:
                            if (bulletPosX + bulletSizeX >= tankPosX && bulletPosX <= tankPosX + tankSizeY &&
                                bulletPosY + bulletSizeY >= tankPosY - tankSizeX && bulletPosY <= tankPosY) {
                                bullet->setFired(false);
                                this->playerWasKilled_ = true;
                                this->idOfKilledPlayer_ = tank->getPlayerId();
                                std::unique_lock<std::mutex> loc(*this->mutex_);
                                this->sendDataBool_ = true;
                                this->sendDataCond_->notify_one();
                            }
                            break;

                        case RIGHT:
                            if (bulletPosX + bulletSizeX >= tankPosX - tankSizeY && bulletPosX <= tankPosX &&
                                bulletPosY + bulletSizeY >= tankPosY && bulletPosY <= tankPosY + tankSizeX) {
                                bullet->setFired(false);
                                this->playerWasKilled_ = true;
                                this->idOfKilledPlayer_ = tank->getPlayerId();
                                std::unique_lock<std::mutex> loc(*this->mutex_);
                                this->sendDataBool_ = true;
                                this->sendDataCond_->notify_one();
                            }
                            break;
                    }
                }
            }
        }
    }
}

/**
 * By calling this function you'll render tank and name of player
 */
void Application::draw() {
    this->window_->clear();

    this->clientTank_->render(*this->window_);

    float tankPosX = this->clientTank_->getSprite()->getPosition().x;
    float tankPosY = this->clientTank_->getSprite()->getPosition().y;
    float tankSizeX = this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x;
    float tankSizeY = this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y;

    switch (this->clientTank_->getDirection()) {
        case UP:
            this->nameOfPlayer_.setPosition(tankPosX + tankSizeX / 2 - this->nameOfPlayer_.getLocalBounds().width / 2, tankPosY + tankSizeY + 5);
            break;

        case DOWN:
            this->nameOfPlayer_.setPosition(tankPosX - tankSizeX / 2 - this->nameOfPlayer_.getLocalBounds().width / 2, tankPosY - tankSizeY - this->nameOfPlayer_.getLocalBounds().height - 10);
            break;

        case LEFT:
            if (this->nameOfPlayer_.getLocalBounds().width <= tankSizeY) {
                this->nameOfPlayer_.setPosition(tankPosX + tankSizeY - this->nameOfPlayer_.getLocalBounds().width, tankPosY + 5);
            } else {
                this->nameOfPlayer_.setPosition(tankPosX + tankSizeY / 2 - this->nameOfPlayer_.getLocalBounds().width / 2, tankPosY + 5);
            }
            break;

        case RIGHT:
            if (this->nameOfPlayer_.getLocalBounds().width <= tankSizeY) {
                this->nameOfPlayer_.setPosition(tankPosX - tankSizeY, tankPosY + tankSizeX + 5);
            } else {
                this->nameOfPlayer_.setPosition(tankPosX - tankSizeY / 2 - this->nameOfPlayer_.getLocalBounds().width / 2,tankPosY + tankSizeX + 5);
            }
            break;
    }

    this->window_->draw(this->nameOfPlayer_);

    for (Tank *tank: *this->otherTanks_) {
        tank->lockMutex();
        if (!tank->getLeft()) {
            tank->render(*this->window_);
        }
        tank->unlockMutex();
    }

    this->window_->display();
}

/**
 * Thread function.
 * Function for receiving data from server
 */
void Application::receiveData() {
    sf::Packet packetReceive = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;
    int pId, direction;
    float positionX, positionY;
    bool fired;
    int messageType, killerId, score;

    while (this->isRunning_) {
        packetReceive.clear();
        if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done) {
            packetReceive >> messageType;
            messageType--;

            if (static_cast<TYPES_MESSAGES>(messageType) == STATUS) {
                for (int i = 0; i < this->numberOfPlayers_ - 1; ++i) {
                    packetReceive >> pId;
                    packetReceive >> positionX;
                    packetReceive >> positionY;
                    packetReceive >> direction;
                    packetReceive >> fired;

                    for (Tank *tank: *this->otherTanks_) {
                        tank->lockMutex();
                        if (tank->getPlayerId() == pId) {
                            if (tank->getDirection() != direction) {
                                tank->rotate(static_cast<DIRECTION>(direction));
                            }

                            tank->getSprite()->setPosition(positionX, positionY);

                            if (fired) {
                                tank->fire();
                            }
                        }
                        tank->unlockMutex();
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
                    this->clientTank_->rotate(static_cast<DIRECTION>(direction));
                    this->clientTank_->getSprite()->setPosition(positionX, positionY);
                } else {
                    for(Tank* tank: *this->otherTanks_) {
                        tank->lockMutex();
                        if (tank->getPlayerId() == pId) {
                            tank->getBullet()->setFired(false);

                            if (tank->getDirection() != direction) {
                                tank->rotate(static_cast<DIRECTION>(direction));
                            }

                            tank->getSprite()->setPosition(positionX, positionY);
                        }
                        tank->unlockMutex();
                    }
                }

                for(Tank* tank: *this->otherTanks_) {
                    if (tank->getPlayerId() == killerId) {
                        tank->getBullet()->setFired(false);
                    }
                }
            } else if (static_cast<TYPES_MESSAGES>(messageType) == END) {
                for (int i = 0; i < this->numberOfPlayers_; ++i) {
                    packetReceive >> pId;
                    packetReceive >> score;

                    if (this->clientTank_->getPlayerId() == pId) {
                        this->clientTank_->setScore(score);
                    } else {
                        for (Tank* tank: *this->otherTanks_) {
                            if (tank->getPlayerId() == pId) {
                                tank->setScore(score);
                            }
                        }
                    }
                }

                this->isRunning_ = false;
            } else if (static_cast<TYPES_MESSAGES>(messageType) == PLAYER_QUIT) {
                packetReceive >> pId;

                for(Tank* tank: *this->otherTanks_) {
                    tank->lockMutex();

                    if (tank->getPlayerId() == pId) {
                        tank->setLeft(true);
                    }

                    tank->unlockMutex();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

/**
 * Thread function.
 * Function for sending data to server.
 */
void Application::sendData() {
    sf::Packet packetReceive = sf::Packet{};
    sf::Packet packetSend = sf::Packet{};
    sf::IpAddress ipAddress;
    float positionX, positionY;
    bool continueWithSending = true;

    while (continueWithSending) {
        std::unique_lock<std::mutex> loc(*this->mutex_);
        while (!this->sendDataBool_) {
            this->sendDataCond_->wait(loc);
        }

        if (this->window_ != nullptr && this->clientTank_ != nullptr) {
            packetSend.clear();

            if (this->clientTank_->getLeft()) {
                packetSend << (static_cast<int>(END) + 1);
                packetSend << this->clientTank_->getPlayerId();
                continueWithSending = false;
            } else if (this->playerWasKilled_) {
                packetSend << (static_cast<int>(KILLED) + 1);
                packetSend << this->idOfKilledPlayer_;
                packetSend << this->clientTank_->getPlayerId();
                std::cout << "Player " << this->clientTank_->getPlayerId() << " Killed " << this->idOfKilledPlayer_ << "\n";
                this->playerWasKilled_ = false;
                this->clientTank_->getBullet()->resetWasFiredAndSent();
            } else if (this->isRunning_) {
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

            if (this->socket_.send(packetSend, this->ipAddress_, this->port_) != sf::Socket::Done) {
                std::cout << "Sending failed" << "\n";
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        this->sendDataBool_ = false;
    }
}

/**
 * By calling this function you'll send specific initial position to server
 */
void Application::updatePositionsOfTanks() {
    sf::Packet packetReceive = sf::Packet{};
    sf::Packet packetSend = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;
    float positionX, positionY;

    if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done) {

    }
    packetReceive.clear();

    packetSend.clear();
    positionX = this->clientTank_->getSprite()->getPosition().x;
    positionY = this->clientTank_->getSprite()->getPosition().y;

    packetSend << this->clientTank_->getPlayerId();
    packetSend << positionX;
    packetSend << positionY;
    packetSend << static_cast<int>(this->clientTank_->getDirection());
    if (this->socket_.send(packetSend, this->ipAddress_, this->port_) != sf::Socket::Done) {
        std::cout << "Sending failed" << "\n";
    }
}