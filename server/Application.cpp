//
// Created by vrsp on 30. 12. 2022.
//

#include "Application.h"

Application::Application() {
    this->isRunning = true;
    this->clients_ = new std::vector<Client *>();
}

Application::~Application() {

}


void Application::run() {
    if (this->isRunning) {
        std::thread sendDataThread(&Application::sendData, this);
        std::thread receiveData(&Application::receiveData, this);
        sendDataThread.join();
        receiveData.join();
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
            this->packetSend_ << count + 1;
            this->packetSend_ << static_cast<int>(UP);
            tmpDir = UP;
        } else if (count == 1) {
            positionX = 400;
            positionY = 800;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << count + 1;
            this->packetSend_ << static_cast<int>(DOWN);
            tmpDir = DOWN;
        } else if (count == 2) {
            positionX = 1;
            positionY = 400;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << count + 1;
            this->packetSend_ << static_cast<int>(LEFT);
            tmpDir = LEFT;
        } else {
            positionX = 800;
            positionY = 400;
            this->packetSend_ << positionX;
            this->packetSend_ << positionY;
            this->packetSend_ << count + 1;
            this->packetSend_ << static_cast<int>(LEFT);
            tmpDir = RIGHT;
        }

        this->packetSend_ << n;
        std::cout << "Sending packet on IP: " << tmpIp.toString() << " With port: " << tmpPort << "\n";

        Client *tmpClient = new Client(count + 1, positionX, positionY, tmpDir, tmpPort, tmpIp);
        this->clients_->push_back(tmpClient);
        count++;

        if (this->socket_.send(this->packetSend_, tmpIp, tmpPort) == sf::Socket::Done) {
            std::cout << "Data with his status were sent\n";
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
    sf::Packet packetRecieve = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;
    float tmpX = 0, tmpY = 0;
    int tmpDir, pId;
    int testInt = 0;

    while (true) {

        packetRecieve.clear();

        if (this->socket_.receive(packetRecieve, ipAddress, port) == sf::Socket::Done) {
            packetRecieve >> pId;
            packetRecieve >> tmpX;
            packetRecieve >> tmpY;
            packetRecieve >> tmpDir;
            packetRecieve >> this->clientReadyToPlay_;
        }

        this->packetSend_.clear();
        for (Client *clientInfo: *this->clients_) {
            if (clientInfo->getClientId() != pId) {
                this->packetSend_ << clientInfo->getClientId();
                this->packetSend_ << clientInfo->getPosition()->xPosition_;
                this->packetSend_ << clientInfo->getPosition()->yPosition_;
                this->packetSend_ << static_cast<int>(clientInfo->getPosition()->direction_);
            }
            if (clientInfo->getClientId() == pId) {
                clientInfo->getPosition()->xPosition_ = tmpX;
                clientInfo->getPosition()->yPosition_ = tmpY;
                clientInfo->getPosition()->direction_ = static_cast<DIRECTION>(tmpDir);
            }
//            std::cout << "Client: " << clientInfo->getClientId() << " --> X: " << clientInfo->getPosition()->xPosition_ << " Y: " << clientInfo->getPosition()->yPosition_ << "\n";
        }
        if (this->socket_.send(this->packetSend_, ipAddress, port) == sf::Socket::Done) {
//            std::cout << "Data were sent to client to client with ID: " << pId << "\n";
        }
    }

    this->isRunning = false;
}

void Application::initializeGame() {
    for (Client *client: *this->clients_) {
        this->packetSend_.clear();
        for (Client *clientInfo: *this->clients_) {
            if (clientInfo->getClientId() != client->getClientId()) {
                this->packetSend_ << clientInfo->getClientId();
                this->packetSend_ << clientInfo->getPosition()->xPosition_;
                this->packetSend_ << clientInfo->getPosition()->yPosition_;
                this->packetSend_ << static_cast<int>(clientInfo->getPosition()->direction_);
            }
        }
        if (this->socket_.send(this->packetSend_, client->getConnetcion()->ipAddress_,
                               client->getConnetcion()->port_) == sf::Socket::Done) {
//            std::cout << "Data were sent to client to client with ID: " << client->getClientId() << "\n";
        }
    }

}

void Application::sendData() {
    while (true) {
        for (Client *client: *this->clients_) {
            this->packetSend_.clear();
            for (Client *clientInfo: *this->clients_) {
                if (clientInfo->getClientId() != client->getClientId()) {
                    this->packetSend_ << clientInfo->getClientId();
                    this->packetSend_ << clientInfo->getPosition()->xPosition_;
                    this->packetSend_ << clientInfo->getPosition()->yPosition_;
                    this->packetSend_ << static_cast<int>(clientInfo->getPosition()->direction_);
                }
            }
            if (this->socket_.send(this->packetSend_, client->getConnetcion()->ipAddress_,
                                   client->getConnetcion()->port_) == sf::Socket::Done) {
    //            std::cout << "Data were sent to client to client with ID: " << client->getClientId() << "\n";
            }
        }
    }
}

void Application::receiveData() {
    sf::Packet packetRecieve = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;
    float tmpX = 0, tmpY = 0;
    int tmpDir, pId;

    while (true) {
        packetRecieve.clear();

        if (this->socket_.receive(packetRecieve, ipAddress, port) == sf::Socket::Done) {
            packetRecieve >> pId;
            packetRecieve >> tmpX;
            packetRecieve >> tmpY;
            packetRecieve >> tmpDir;
            packetRecieve >> this->clientReadyToPlay_;
        }

        for (Client* client : *this->clients_) {
            if (client->getClientId() == pId) {
                client->getPosition()->xPosition_ = tmpX;
                client->getPosition()->yPosition_ = tmpY;
                client->getPosition()->direction_ = static_cast<DIRECTION>(tmpDir);
            }
        }
    }
}























