//
// Created by vrsp on 30. 12. 2022.
//

#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H
#include <iostream>
#include <list>
#include <thread>
#include <cmath>
#include <SFML/Network.hpp>
#include <mutex>
#include <condition_variable>
#include "../client/Client.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

enum TYPES_MESSAGES {
    STATUS,
    KILLED,
    END,
    PLAYER_QUIT
};

class Application {
public:
    Application();
    ~Application();

    void run();

    void initializeSocket();
    void initializeGame();
    void updateOfTanksPositions();
    void waitForClients();

    void sendData();
    void receiveData();

private:
    bool isRunning_;
    int numberOfLeftPlayers_;
    std::vector<Client*>* clients_;

    bool sendDataBool_;
    std::mutex* mutex_;
    std::condition_variable* sendDataCond_;

    sf::UdpSocket socket_;
    sf::Packet packetSend_;
    sf::Packet packetReceive_;
    sf::IpAddress ipAddress_;
};


#endif //SERVER_APPLICATION_H
