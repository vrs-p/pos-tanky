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
            positionX = this->clientTank_->getSprite()->getPosition().x;
            positionY = this->clientTank_->getSprite()->getPosition().y;

            packetSend << this->clientTank_->getPlayerId();
            packetSend << positionX;
            packetSend << positionY;
            packetSend << static_cast<int>(this->clientTank_->getDirection());
            packetSend << (this->clientTank_->getBullet()->wasFired() && !this->clientTank_->getBullet()->wasFiredAndSent());

            if (this->clientTank_->getBullet()->wasFired() && (this->clientTank_->getBullet()->wasFiredAndSent() == false)) {
//                std::cout << "Fired" << "\n";
                this->clientTank_->getBullet()->setWasFiredAndSent();
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
            tmpX = tmpX - this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x / 2;
            tmpY = tmpY - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y;
            break;
        case DOWN:
            tmpX = tmpX - this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x / 2;
            tmpY = tmpY - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y / 3;
            break;
        case LEFT:
            tmpX = tmpX - this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x;
            tmpY = tmpY - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y * 2 / 3;
            break;
        case RIGHT:
            tmpY = tmpY - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y * 2 / 3;
            break;
    }

    this->clientTank_->getSprite()->setPosition(tmpX, tmpY);
    this->clientTank_->rotate(static_cast<DIRECTION>(tmpDir));
    this->clientTank_->setDirection(static_cast<DIRECTION>(tmpDir));
    this->numberOfPlayers_ = numberOfPlayers;
    this->clientTank_->setPlayerId(tmpID);

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
                positionX = positionX + this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y - this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x;
                positionY = positionY - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y;
                break;
            case RIGHT:
                positionX = positionX - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y;
                positionY = positionY - this->clientTank_->getSprite()->getTexture()->getSize().y * this->clientTank_->getSprite()->getScale().y + this->clientTank_->getSprite()->getTexture()->getSize().x * this->clientTank_->getSprite()->getScale().x;
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

    while (this->isRunning) {
        packetReceive.clear();
        if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done)
//                std::cout << "Packet with game settings was received\n";

            for (int i = 0; i < this->numberOfPlayers_ - 1; ++i) {
                packetReceive >> pId;
                packetReceive >> positionX;
                packetReceive >> positionY;
                packetReceive >> direction;
                packetReceive >> fired;

                for (Tank *tank: *this->otherTanks) {
                    if (tank->getPlayerId() == pId) {
                        tank->getSprite()->setPosition(positionX, positionY);
                        tank->rotate(static_cast<DIRECTION>(direction));
                        if (fired)
                            tank->fire();
                    }
                    std::cout << "Client: " << tank->getPlayerId() << " --> X: " << tank->getSprite()->getPosition().x << " Y: " << tank->getSprite()->getPosition().y << "\n";
                }
            }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void Application::checkBulletCollision() {
    float bPositionX = this->clientTank_->getBullet()->getBulletPosition().x;
    float bPositionY = this->clientTank_->getBullet()->getBulletPosition().y;
    float bSizeX = this->clientTank_->getBullet()->getBulletSize().x;
    float bSizeY = this->clientTank_->getBullet()->getBulletSize().y;

    if (this->clientTank_->getBullet()->wasFired()) {
        for (Tank* tank: *this->otherTanks) {
            float tankSizeX = tank->getSprite()->getTexture()->getSize().x * tank->getSprite()->getScale().x;
            float tankSizeY = tank->getSprite()->getTexture()->getSize().y * tank->getSprite()->getScale().y;
//            if ((((bPositionX - bSizeX) >= tank->getSprite()->getPosition().x) && (bPositionX <= (tank->getSprite()->getPosition().x + tankSizeX))) &&
//            (((bPositionY - bSizeY) >= tank->getSprite()->getPosition().y) && (bPositionY <= (tank->getSprite()->getPosition().y + tankSizeY)))) {
//                std::cout << "Player: " << tank->getPlayerId() << " was hit X: " << tank->getSprite()->getPosition().x << " Y: " << tank->getSprite()->getPosition().y << " Size X: " << tankSizeX << " Y: " << tankSizeY << "\n";
//            }
//            if (((bPositionX - bSizeX) >= tank->getSprite()->getPosition().x) && ((bPositionX + bSizeX) <= (tank->getSprite()->getPosition().x + tankSizeX))) {
//                std::cout << "Player: " << tank->getPlayerId() << " was hit X: " << tank->getSprite()->getPosition().x << " Y: " << tank->getSprite()->getPosition().y << " Size X: " << tankSizeX << " Y: " << tankSizeY << "\n";
//            }

//            if (Collision::PixelPerfectTest(*tank->getSprite(), *this->clientTank_->getBullet()->getBulletIcon()));
        }
    }
}
