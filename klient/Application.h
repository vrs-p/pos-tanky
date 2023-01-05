#pragma once
//
// Created by vrsp on 28. 12. 2022.
//
#include <iostream>
#include <list>
#include <thread>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <condition_variable>
#include <mutex>
#include "Tank.h"

#ifndef KLIENT_APPLICATION_H
#define KLIENT_APPLICATION_H

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


    void render();
    void readClientInput();
    void checkBorders();
    void checkBulletCollision();
    void draw();
    void initializeWindow();
    void connectToServer();
    void waitForGameSettings();

    void communicationWithServer();
    void updatePositionsOfTanks();

    void run(sf::IpAddress ipAddress, int port);

    void sendData();
    void receiveData();

    void printScore();

    sf::RenderWindow* getWindow();

private:
    bool isRunning;
    int numberOfPlayers_;
    bool playerWasKilled;
    int idOfKilledPlayer;
    bool sendDataBool;
    std::mutex* mutex;
    std::condition_variable* sendDataCond;


    Tank* clientTank_;
    std::vector<Tank*>* otherTanks;

    sf::RenderWindow* window_;

    sf::IpAddress ipAddress_;
    int port_;
    sf::UdpSocket socket_;
    sf::Packet packetSend_;
    unsigned short id_;
};


#endif //KLIENT_APPLICATION_H
