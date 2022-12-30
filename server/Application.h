//
// Created by vrsp on 30. 12. 2022.
//

#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H
#include <iostream>
#include <list>
#include <thread>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Tank.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class Application {
public:
    Application();
    ~Application();


    void render();
    void readClientInput();
    void checkBorders();
    void draw();
    void initializeWindow();

    void run();

    void initializeSocket();
    void waitForClients();
    void communicate();

private:
    bool isRunning;

    Tank* clientTank_;
    std::list<Tank*>* otherTanks;
    Tank* otherClientTank_;

    sf::RenderWindow* window_;

    sf::UdpSocket socket_;
    sf::Packet packetSend_;
    sf::Packet packetRecieve_;
    sf::IpAddress ipAddress_;
    unsigned short port_;
    bool clientReadyToPlay_;
};


#endif //SERVER_APPLICATION_H
