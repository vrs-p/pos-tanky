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
#include "Client.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class Application {
public:
    Application();
    ~Application();


//    void render();
//    void readClientInput();
//    void checkBorders();
//    void draw();
//    void initializeWindow();

    void run();

    void initializeSocket();
    void initializeGame();
    void waitForClients();
    void communicate();

    void sendData();
    void receiveData();

private:
    bool isRunning;
    std::vector<Client*>* clients_;


    sf::UdpSocket socket_;
    sf::Packet packetSend_;
    sf::Packet packetRecieve_;
    sf::IpAddress ipAddress_;
    unsigned short port_;
    bool clientReadyToPlay_;
};


#endif //SERVER_APPLICATION_H
