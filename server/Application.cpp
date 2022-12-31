//
// Created by vrsp on 30. 12. 2022.
//

#include "Application.h"

Application::Application() {
    this->isRunning = true;
    this->clients_ = new std::vector<Client*>();
}

Application::~Application() {

}


void Application::run() {
    if (this->isRunning) {
//        std::thread renderingThread(&Application::render, this);
        std::thread communicatingThread(&Application::communicate, this);
//        renderingThread.join();
        communicatingThread.join();
    }
}

void Application::initializeSocket() {
    this->packetSend_ = sf::Packet{};
    this->packetRecieve_ = sf::Packet{};
    this->ipAddress_ = sf::IpAddress::Any;

    if (this->socket_.bind(13877, this->ipAddress_) != sf::Socket::Done) {
        std::cout << "Unable to bind PORT 55000" << std::endl;
    }

//    std::cout << "server\n" << "\tip: " << this->ipAddress_.toString() << std::endl << "\tport: " << this->port_ << std::endl;
}


void Application::waitForClients() {
    float tmpX, tmpY;
    int tmpDir;

//    std::cout << "server\n" << "\tip: " << this->ipAddress_.toString() << std::endl << "\tport: " << this->port_ << std::endl;

    std::cout << "Enter number of players (max 4): ";
    int n, count = 0;
    std::cin >> n;
    if (n > 4) {
        n = 4;
    }
    std::cout << std::endl;


    while (count < n) {
        float positionX, positionY;
        unsigned short tmpPort;
        sf::IpAddress tmpIp = sf::IpAddress::Any;
        DIRECTION tmpDir;

        if (this->socket_.receive(this->packetRecieve_, tmpIp, tmpPort) == sf::Socket::Done) {
            std::cout << "Client was connected\n";
        }
        this->packetSend_.clear();

        if (count == 0) {
            positionX = 400;
            positionY = 1;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << count;
            this->packetSend_ << static_cast<int>(UP);
            tmpDir = UP;
        } else if (count == 1) {
            positionX = 400;
            positionY = 800;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << count;
            this->packetSend_ << static_cast<int>(DOWN);
            tmpDir = DOWN;
        } else if (count == 2) {
            positionX = 1;
            positionY = 400;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << count;
            this->packetSend_ << static_cast<int>(LEFT);
            tmpDir = LEFT;
        } else {
            positionX = 800;
            positionY = 400;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << count;
            this->packetSend_ << static_cast<int>(LEFT);
            tmpDir = RIGHT;
        }

        std::cout << "Sending packet on IP: " << tmpIp.toString() << " With port: " << tmpPort << "\n";

        Client* tmpClient = new Client(count, positionX, positionY, tmpDir, tmpPort, tmpIp);
        this->clients_->push_back(tmpClient);

        count++;

        if (this->socket_.send(this->packetSend_, tmpIp, tmpPort) == sf::Socket::Done) {
            std::cout << "Data were sent to client\n";
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

//    while (this->clientReadyToPlay_) {
//        float tmpX, tmpY;
//        int tmpDir;
//
//        if (this->socket_.receive(this->packetRecieve_, this->ipAddress_, this->port_) == sf::Socket::Done) {
//            this->packetRecieve_ >> tmpX;
//            this->packetRecieve_ >> tmpY;
//            this->packetRecieve_ >> tmpDir;
//            this->packetRecieve_ >> this->clientReadyToPlay_;
//        }
//
//        if (this->otherClientTank_->getDirection() != tmpDir) {
////            this->otherClientTank_->rotate(static_cast<DIRECTION>(tmpDir));
//        }
////        this->otherClientTank_->getSprite()->setPosition(sf::Vector2f(tmpX, tmpY));
////        this->otherClientTank_->setDirection(static_cast<DIRECTION>(tmpDir));
//    }

    this->isRunning = false;
}