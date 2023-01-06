//
// Created by vrsp on 30. 12. 2022.
//

#include "Application.h"

/**
 * Constructor for Application class
 */
Application::Application() {
    this->isRunning_ = true;
    this->numberOfLeftPlayers_ = 0;
    this->clients_ = new std::vector<Client *>();
    this->sendDataBool_ = false;
    this->mutex_ = new std::mutex();
    this->sendDataCond_ = new std::condition_variable();
}

/**
 * Destructor for Application class
 */
Application::~Application() {
    for (int i = this->clients_->size() - 1; i >= 0; i--) {
        delete this->clients_->at(i);
    }
    delete this->clients_;
    this->clients_ = nullptr;

    delete this->mutex_;
    this->mutex_ = nullptr;

    delete this->sendDataCond_;
    this->sendDataCond_ = nullptr;

}

/**
 * By calling this method you are able to start main program
 * You'll also initialize communication and whole game
 */
void Application::run() {
    this->initializeSocket();
    this->waitForClients();
    this->updateOfTanksPositions();
    this->initializeGame();
    if (this->isRunning_) {
        std::thread sendDataThread(&Application::sendData, this);
        std::thread receiveDataThread(&Application::receiveData, this);
        sendDataThread.join();
        receiveDataThread.join();
    }
}

/**
 * By calling this method you are able to bind port and
 * initialize socket
 */
void Application::initializeSocket() {
    this->packetSend_ = sf::Packet{};
    this->packetReceive_ = sf::Packet{};
    this->ipAddress_ = sf::IpAddress::Any;

    if (this->socket_.bind(13877, this->ipAddress_) != sf::Socket::Done) {
        std::cout << "Unable to bind PORT 13877" << std::endl;
    }
}

/**
 * By calling this method you are able to set number of players in game
 * and wait for them until they join to the game
 */
void Application::waitForClients() {
    std::cout << "Enter number of players (max 4): ";
    int numberOfPlayers, count = 0;
    std::cin >> numberOfPlayers;
    while (numberOfPlayers < 1 || numberOfPlayers > 4) {
        std::cout << "Incorrect input. Allowed number of players is in range 1-4\n";
        std::cout << "Enter number of players (max 4): ";
        std::cin >> numberOfPlayers;
    }


    while (count < numberOfPlayers) {
        float positionX, positionY;
        unsigned short tmpPort;
        sf::IpAddress tmpIp = sf::IpAddress::Any;
        DIRECTION tmpDir;
        std::string pName;

        this->packetReceive_.clear();
        if (this->socket_.receive(this->packetReceive_, tmpIp, tmpPort) == sf::Socket::Done) {
            this->packetReceive_ >> pName;
            std::cout << "Client was connected\n";
        }
        this->packetSend_.clear();

        if (count == 0) {
            positionX = SCREEN_WIDTH / 2;
            positionY = SCREEN_HEIGHT;
            tmpDir = UP;
        } else if (count == 1) {
            positionX = SCREEN_WIDTH / 2;
            positionY = 0;
            tmpDir = DOWN;
        } else if (count == 2) {
            positionX = SCREEN_WIDTH;
            positionY = SCREEN_HEIGHT / 2;
            tmpDir = LEFT;
        } else {
            positionX = 0;
            positionY = SCREEN_HEIGHT / 2;
            tmpDir = RIGHT;
        }

        this->packetSend_ << positionX;
        this->packetSend_ << positionY;
        this->packetSend_ << count + 1;
        this->packetSend_ << static_cast<int>(tmpDir);
        this->packetSend_ << numberOfPlayers;

        std::cout << "Sending packet on IP: " << tmpIp.toString() << " With port: " << tmpPort << "\n";
        Client *tmpClient = new Client(count + 1, pName, positionX, positionY, tmpDir, tmpPort, tmpIp);
        this->clients_->push_back(tmpClient);
        count++;

        if (this->socket_.send(this->packetSend_, tmpIp, tmpPort) != sf::Socket::Done) {
            std::cout << "Sending failed\n";
        }
    }
}

/**
 * By calling this method you are able to send specific initial
 * position to the players
 */
void Application::updateOfTanksPositions() {
    for (Client *client: *this->clients_) {
        this->packetSend_.clear();
        if (this->socket_.send(this->packetSend_, client->getConnection()->ipAddress_, client->getConnection()->port_) != sf::Socket::Done) {
            std::cout << "Sending failed" << "\n";
        }
    }

    int count = 0;
    sf::Packet packetReceive = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;
    float tmpX = 0, tmpY = 0;
    int tmpDir, pId;
    while (count < this->clients_->size()) {

        packetReceive.clear();
        if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done) {
            packetReceive >> pId;
            packetReceive >> tmpX;
            packetReceive >> tmpY;
            packetReceive >> tmpDir;
        }

        for (Client* client : *this->clients_) {
            if (client->getClientId() == pId) {
                client->updatePosition(tmpX, tmpY, static_cast<DIRECTION>(tmpDir));
                client->setInitialPosition(tmpX, tmpY, static_cast<DIRECTION>(tmpDir));
            }
        }

        count++;
    }
}

/**
 * By calling this method you will send initial info about game
 * to the players. Like their IDs, names, positions and directions
 */
void Application::initializeGame() {
    for (Client *client: *this->clients_) {
        this->packetSend_.clear();
        for (Client *clientInfo: *this->clients_) {
            if (clientInfo->getClientId() != client->getClientId()) {
                this->packetSend_ << clientInfo->getClientId();
                this->packetSend_ << clientInfo->getPlayerName();
                this->packetSend_ << clientInfo->getPosition()->xPosition_;
                this->packetSend_ << clientInfo->getPosition()->yPosition_;
                this->packetSend_ << static_cast<int>(clientInfo->getPosition()->direction_);
            }
        }
        if (this->socket_.send(this->packetSend_, client->getConnection()->ipAddress_, client->getConnection()->port_) != sf::Socket::Done) {
            std::cout << "Packet with initial info failed\n";
        }
    }

}

/**
 * By calling this method you'll start sending data about game to the players.
 * Data are send only when there is any update in game.
 * Like someone moved/fired/died/quit
 */
void Application::sendData() {
    while (this->isRunning_) {
        std::unique_lock<std::mutex> loc(*this->mutex_);
        while (!this->sendDataBool_) {
            this->sendDataCond_->wait(loc);
        }

        for (Client *client: *this->clients_) {
            this->packetSend_.clear();
            if (!client->wasKilled() && !client->getLeft()) {
                this->packetSend_ << (static_cast<int>(STATUS) + 1);
                for (Client *clientInfo: *this->clients_) {
                    if (clientInfo->getClientId() != client->getClientId()) {
                        clientInfo->lockMutex();
                        this->packetSend_ << clientInfo->getClientId();
                        this->packetSend_ << clientInfo->getPosition()->xPosition_;
                        this->packetSend_ << clientInfo->getPosition()->yPosition_;
                        this->packetSend_ << static_cast<int>(clientInfo->getPosition()->direction_);
                        this->packetSend_ << clientInfo->getFired();
                        clientInfo->unlockMutex();
                    }
                }
                if (this->socket_.send(this->packetSend_, client->getConnection()->ipAddress_, client->getConnection()->port_) != sf::Socket::Done) {
                    std::cout << "Packet with someone was killed info failed\n";
                }
            } else if (client->wasKilled() && !client->getLeft()) {
                client->lockMutex();
                client->resetPosition();
                this->packetSend_ << (static_cast<int>(KILLED) + 1);
                this->packetSend_ << client->getClientId();
                this->packetSend_ << client->getPosition()->xPosition_;
                this->packetSend_ << client->getPosition()->yPosition_;
                this->packetSend_ << static_cast<int>(client->getPosition()->direction_);
                this->packetSend_ << client->getKilledBy();
                client->unlockMutex();

                for (Client *clientKilled: *this->clients_) {
                    this->socket_.send(this->packetSend_, clientKilled->getConnection()->ipAddress_,
                                       clientKilled->getConnection()->port_);
                }
            } else if (client->getLeft() && !client->wasScoreSent()) {
                std::cout << "Client " << client->getClientId() << " left\n";
                client->lockMutex();
                this->packetSend_ << (static_cast<int>(PLAYER_QUIT) + 1);
                this->packetSend_ << client->getClientId();
                client->unlockMutex();
                for (Client *clientInform: *this->clients_) {
                    this->socket_.send(this->packetSend_, clientInform->getConnection()->ipAddress_,
                                       clientInform->getConnection()->port_);
                }

                client->setScoreWasSent(true);
                this->packetSend_.clear();
                this->packetSend_ << (static_cast<int>(END) + 1);
                for (Client *clientScore: *this->clients_) {
                    this->packetSend_ << clientScore->getClientId();
                    this->packetSend_ << clientScore->getScore();
                }
                this->socket_.send(this->packetSend_, client->getConnection()->ipAddress_, client->getConnection()->port_);
            }
        }
        for (Client *client: *this->clients_) client->setFired(false);
        this->sendDataBool_ = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

/**
 * By calling this method you'll start receiving data from players.
 * When data are received and saved the sendData() method is called.
 */
void Application::receiveData() {
    sf::Packet packetReceive = sf::Packet{};
    sf::IpAddress ipAddress = sf::IpAddress::Any;
    unsigned short port;
    float tmpX = 0, tmpY = 0;
    int tmpDir, pId;
    bool pFIred;
    int messageType;
    int killer;

    while (this->isRunning_) {
        packetReceive.clear();

        if (this->socket_.receive(packetReceive, ipAddress, port) == sf::Socket::Done) {
            packetReceive >> messageType;
            messageType--;
        }
        if (static_cast<TYPES_MESSAGES>(messageType) == STATUS) {
            packetReceive >> pId;
            packetReceive >> tmpX;
            packetReceive >> tmpY;
            packetReceive >> tmpDir;
            packetReceive >> pFIred;
            for (Client* client : *this->clients_) {
                if (client->getClientId() == pId) {
                    client->lockMutex();
                    client->updatePosition(tmpX, tmpY, static_cast<DIRECTION>(tmpDir));
                    client->setFired(pFIred);
                    client->unlockMutex();
                }
            }
        } else if (static_cast<TYPES_MESSAGES>(messageType) == KILLED) {
            packetReceive >> pId;
            packetReceive >> killer;
            for (Client* client : *this->clients_) {
                if (client->getClientId() == pId) {
                    client->lockMutex();
                    client->killed();
                    client->setKilledBy(killer);
                    client->unlockMutex();
                } else if (client->getClientId() == killer) {
                    client->increaseScore();
                }
            }
        } else if (static_cast<TYPES_MESSAGES>(messageType) == END) {
            packetReceive >> pId;
            for (Client* client : *this->clients_) {
                if (client->getClientId() == pId) {
                    client->lockMutex();
                    client->setLeft(true);
                    client->unlockMutex();
                }
            }
            this->numberOfLeftPlayers_++;
            if (this->clients_->size() == numberOfLeftPlayers_) {
                this->isRunning_ = false;
            }
        }
        std::unique_lock<std::mutex> loc(*this->mutex_);
        this->sendDataBool_ = true;
        this->sendDataCond_->notify_one();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}