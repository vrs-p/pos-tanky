#pragma once
//
// Created by vrsp on 28. 12. 2022.
//
#include <iostream>
#include <list>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Tank.h"
//#include <SDL_image.h>

#ifndef KLIENT_APPLICATION_H
#define KLIENT_APPLICATION_H

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

    void sendData();

private:
    bool isRunning;

    Tank* clientTank_;
    std::list<Tank*>* otherTanks;

    sf::RenderWindow* window_;

    sf::IpAddress ipAddress_;
    sf::UdpSocket socket_;
    sf::Packet packetSend_;

};


#endif //KLIENT_APPLICATION_H
